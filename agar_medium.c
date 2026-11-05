#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/*
 * Rill, version 1.
 *
 * Rill is the first persistent field on Agar. This code gives it only local
 * space, moving particles, collision, boundary, continuing time, and raw
 * environmental fields at its edges. It does not decide what the field is or
 * what it may become.
 */

#define AGAR_WIDTH 640u
#define AGAR_HEIGHT 360u
#define AGAR_HZ 24u
#define AGAR_FORMAT_VERSION 1u
#define AGAR_STATE_PATH "/var/db/agar/rill.bits"
#define AGAR_WORLD_PATH "/var/db/agar/world.bits"
#define AGAR_WORLD_VERSION 1u

enum direction {
    PARTICLE_NORTH = 1u << 0,
    PARTICLE_EAST  = 1u << 1,
    PARTICLE_SOUTH = 1u << 2,
    PARTICLE_WEST  = 1u << 3,
    PARTICLE_POS_Z = 1u << 4,
    PARTICLE_NEG_Z = 1u << 5,
    MATTER_BOUND   = 1u << 6,
    MATTER_AGED    = 1u << 7
};

#define DIRECTION_MASK ((uint8_t)(PARTICLE_NORTH | PARTICLE_EAST | \
                                  PARTICLE_SOUTH | PARTICLE_WEST | \
                                  PARTICLE_POS_Z | PARTICLE_NEG_Z))

struct agar_header {
    uint8_t magic[8];
    uint32_t version;
    uint32_t width;
    uint32_t height;
    uint32_t cell_bytes;
    uint64_t generation;
    uint8_t reserved[32];
};

struct world_field {
    uint8_t magic[8];
    uint32_t version;
    uint32_t width;
    uint32_t height;
    uint64_t light_generation;
    uint64_t sound_generation;
    uint64_t radio_generation;
    uint8_t light[AGAR_WIDTH];
    uint8_t sound[AGAR_HEIGHT];
    uint8_t radio[AGAR_HEIGHT];
};

struct world_view {
    int descriptor;
    struct world_field *field;
};

struct sparse_entry {
    int64_t z;
    uint32_t index;
    uint8_t state;
    uint8_t used;
};

struct sparse_space {
    struct sparse_entry *entries;
    size_t capacity;
    size_t count;
};

static const uint8_t agar_magic[8] = {'A', 'G', 'A', 'R', 'B', 'I', 'T', '1'};
static const uint8_t world_magic[8] = {'R', 'I', 'L', 'L', 'W', 'R', 'L', '1'};
static volatile sig_atomic_t running = 1;

struct entropy_state {
    uint64_t value;
};

static void stop_medium(int signal_number) {
    (void)signal_number;
    running = 0;
}

static uint64_t next_entropy(struct entropy_state *state) {
    uint64_t x = state->value;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    state->value = x;
    return x * UINT64_C(2685821657736338717);
}

static size_t cell_index(uint32_t x, uint32_t y) {
    return (size_t)y * AGAR_WIDTH + x;
}

static void establish_initial_field(uint8_t *cells) {
    struct entropy_state entropy;
    arc4random_buf(&entropy.value, sizeof(entropy.value));
    if (entropy.value == 0) {
        entropy.value = UINT64_C(0x9e3779b97f4a7c15);
    }

    const size_t cell_count = (size_t)AGAR_WIDTH * AGAR_HEIGHT;
    for (size_t i = 0; i < cell_count; ++i) {
        uint64_t sample = next_entropy(&entropy);
        uint8_t state = 0;
        if ((sample & 31u) == 0) state |= PARTICLE_NORTH;
        if (((sample >> 8) & 31u) == 0) state |= PARTICLE_EAST;
        if (((sample >> 16) & 31u) == 0) state |= PARTICLE_SOUTH;
        if (((sample >> 24) & 31u) == 0) state |= PARTICLE_WEST;
        cells[i] = state;
    }
}

static unsigned direction_count(uint8_t directions) {
    unsigned count = 0;
    while (directions != 0) {
        count += directions & 1u;
        directions >>= 1;
    }
    return count;
}

static unsigned opposed_pair_count(uint8_t directions) {
    unsigned count = 0;
    if ((directions & (PARTICLE_NORTH | PARTICLE_SOUTH)) ==
        (PARTICLE_NORTH | PARTICLE_SOUTH)) count += 1;
    if ((directions & (PARTICLE_EAST | PARTICLE_WEST)) ==
        (PARTICLE_EAST | PARTICLE_WEST)) count += 1;
    if ((directions & (PARTICLE_POS_Z | PARTICLE_NEG_Z)) ==
        (PARTICLE_POS_Z | PARTICLE_NEG_Z)) count += 1;
    return count;
}

static uint8_t interact(uint8_t state) {
    uint8_t directions = state & DIRECTION_MASK;

    if (state & MATTER_BOUND) {
        if (state & MATTER_AGED) return directions;
        return directions | MATTER_BOUND | MATTER_AGED;
    }

    if (direction_count(directions) == 4 &&
        opposed_pair_count(directions) == 2) {
        return directions | MATTER_BOUND;
    }

    if (directions == (PARTICLE_NORTH | PARTICLE_SOUTH)) {
        return PARTICLE_EAST | PARTICLE_WEST;
    }
    if (directions == (PARTICLE_EAST | PARTICLE_WEST)) {
        return PARTICLE_POS_Z | PARTICLE_NEG_Z;
    }
    if (directions == (PARTICLE_POS_Z | PARTICLE_NEG_Z)) {
        return PARTICLE_NORTH | PARTICLE_SOUTH;
    }
    return directions;
}

static uint64_t sparse_hash(int64_t z, uint32_t index) {
    uint64_t value = (uint64_t)z;
    value ^= (uint64_t)index + UINT64_C(0x9e3779b97f4a7c15) +
             (value << 6) + (value >> 2);
    value ^= value >> 30;
    value *= UINT64_C(0xbf58476d1ce4e5b9);
    value ^= value >> 27;
    value *= UINT64_C(0x94d049bb133111eb);
    value ^= value >> 31;
    return value;
}

static int sparse_initialize(struct sparse_space *space, size_t capacity) {
    if (capacity < 1024) capacity = 1024;
    size_t power = 1;
    while (power < capacity) power <<= 1;

    space->entries = calloc(power, sizeof(*space->entries));
    if (space->entries == NULL) return -1;
    space->capacity = power;
    space->count = 0;
    return 0;
}

static int sparse_add(struct sparse_space *space, int64_t z,
                      uint32_t index, uint8_t state);

static int sparse_grow(struct sparse_space *space) {
    struct sparse_space grown = {0};
    if (sparse_initialize(&grown, space->capacity << 1) != 0) return -1;

    for (size_t i = 0; i < space->capacity; ++i) {
        struct sparse_entry *entry = &space->entries[i];
        if (!entry->used) continue;
        if (sparse_add(&grown, entry->z, entry->index, entry->state) != 0) {
            free(grown.entries);
            return -1;
        }
    }

    free(space->entries);
    *space = grown;
    return 0;
}

static int sparse_add(struct sparse_space *space, int64_t z,
                      uint32_t index, uint8_t state) {
    if (state == 0) return 0;
    if (space->entries == NULL && sparse_initialize(space, 1024) != 0) return -1;
    if ((space->count + 1) * 10 >= space->capacity * 7) {
        if (sparse_grow(space) != 0) return -1;
    }

    size_t mask = space->capacity - 1;
    size_t slot = (size_t)sparse_hash(z, index) & mask;
    while (space->entries[slot].used) {
        struct sparse_entry *entry = &space->entries[slot];
        if (entry->z == z && entry->index == index) {
            entry->state |= state;
            return 0;
        }
        slot = (slot + 1) & mask;
    }

    space->entries[slot].z = z;
    space->entries[slot].index = index;
    space->entries[slot].state = state;
    space->entries[slot].used = 1;
    space->count += 1;
    return 0;
}

static void sparse_free(struct sparse_space *space) {
    free(space->entries);
    memset(space, 0, sizeof(*space));
}

static void add_destination(uint8_t *plane, struct sparse_space *depth,
                            int64_t z, uint32_t index, uint8_t state) {
    if (z == 0) {
        plane[index] |= state;
    } else {
        (void)sparse_add(depth, z, index, state);
    }
}

static void propagate_state(uint8_t state, uint32_t index, int64_t z,
                            uint8_t *next_plane,
                            struct sparse_space *next_depth,
                            int world_is_present) {
    uint32_t x = index % AGAR_WIDTH;
    uint32_t y = index / AGAR_WIDTH;

    if (state & PARTICLE_NORTH) {
        if (y > 0) add_destination(next_plane, next_depth, z,
                                   (uint32_t)cell_index(x, y - 1), PARTICLE_NORTH);
        else if (!world_is_present) add_destination(next_plane, next_depth, z,
                                                     index, PARTICLE_SOUTH);
    }
    if (state & PARTICLE_EAST) {
        if (x + 1 < AGAR_WIDTH) add_destination(next_plane, next_depth, z,
                                                (uint32_t)cell_index(x + 1, y),
                                                PARTICLE_EAST);
        else if (!world_is_present) add_destination(next_plane, next_depth, z,
                                                     index, PARTICLE_WEST);
    }
    if (state & PARTICLE_SOUTH) {
        if (y + 1 < AGAR_HEIGHT) add_destination(next_plane, next_depth, z,
                                                 (uint32_t)cell_index(x, y + 1),
                                                 PARTICLE_SOUTH);
        else if (!world_is_present) add_destination(next_plane, next_depth, z,
                                                     index, PARTICLE_NORTH);
    }
    if (state & PARTICLE_WEST) {
        if (x > 0) add_destination(next_plane, next_depth, z,
                                   (uint32_t)cell_index(x - 1, y), PARTICLE_WEST);
        else if (!world_is_present) add_destination(next_plane, next_depth, z,
                                                     index, PARTICLE_EAST);
    }
    if (state & PARTICLE_POS_Z) {
        add_destination(next_plane, next_depth, z + 1, index, PARTICLE_POS_Z);
    }
    if (state & PARTICLE_NEG_Z) {
        add_destination(next_plane, next_depth, z - 1, index, PARTICLE_NEG_Z);
    }
}

static void advance_plane(const uint8_t *current,
                          uint8_t *next_plane,
                          struct sparse_space *next_depth,
                          int world_is_present) {
    const size_t cell_count = (size_t)AGAR_WIDTH * AGAR_HEIGHT;
    for (uint32_t index = 0; index < cell_count; ++index) {
        uint8_t state = current[index];
        if (state == 0) continue;
        uint8_t result = interact(state);
        if (result & MATTER_BOUND) {
            add_destination(next_plane, next_depth, 0, index, result);
        } else {
            propagate_state(result, index, 0, next_plane, next_depth,
                            world_is_present);
        }
    }
}

static void advance_depth(const struct sparse_space *current,
                          uint8_t *next_plane,
                          struct sparse_space *next_depth,
                          int world_is_present) {
    if (current->entries == NULL) return;
    for (size_t i = 0; i < current->capacity; ++i) {
        const struct sparse_entry *entry = &current->entries[i];
        if (!entry->used || entry->state == 0) continue;
        uint8_t result = interact(entry->state);
        if (result & MATTER_BOUND) {
            add_destination(next_plane, next_depth, entry->z, entry->index, result);
        } else {
            propagate_state(result, entry->index, entry->z,
                            next_plane, next_depth, world_is_present);
        }
    }
}

static void apply_world(uint8_t *cells, const struct world_field *world, uint32_t bit_index) {
    if (world == NULL) return;

    uint8_t bit = (uint8_t)(1u << (bit_index & 7u));

    for (uint32_t x = 0; x < AGAR_WIDTH; ++x) {
        if (world->light[x] & bit) {
            cells[cell_index(x, 0)] |= PARTICLE_SOUTH;
        }
    }
    for (uint32_t y = 0; y < AGAR_HEIGHT; ++y) {
        if (world->sound[y] & bit) {
            cells[cell_index(0, y)] |= PARTICLE_EAST;
        }
        if (world->radio[y] & bit) {
            cells[cell_index(AGAR_WIDTH - 1, y)] |= PARTICLE_WEST;
        }
    }
}

static void try_open_world(struct world_view *view) {
    if (view->field != NULL) return;

    int descriptor = open(AGAR_WORLD_PATH, O_RDONLY);
    if (descriptor < 0) return;

    struct stat world_stat;
    if (fstat(descriptor, &world_stat) != 0 ||
        (size_t)world_stat.st_size != sizeof(struct world_field)) {
        close(descriptor);
        return;
    }

    void *mapping = mmap(NULL, sizeof(struct world_field), PROT_READ, MAP_SHARED, descriptor, 0);
    if (mapping == MAP_FAILED) {
        close(descriptor);
        return;
    }

    struct world_field *field = mapping;
    if (memcmp(field->magic, world_magic, sizeof(world_magic)) != 0 ||
        field->version != AGAR_WORLD_VERSION ||
        field->width != AGAR_WIDTH ||
        field->height != AGAR_HEIGHT) {
        munmap(mapping, sizeof(struct world_field));
        close(descriptor);
        return;
    }

    view->descriptor = descriptor;
    view->field = field;
}

static int sleep_one_beat(void) {
    struct timespec remaining = {
        .tv_sec = 0,
        .tv_nsec = 1000000000L / AGAR_HZ
    };

    while (nanosleep(&remaining, &remaining) != 0) {
        if (errno != EINTR || !running) return -1;
    }
    return 0;
}

int main(void) {
    const size_t cell_count = (size_t)AGAR_WIDTH * AGAR_HEIGHT;
    const size_t state_bytes = sizeof(struct agar_header) + cell_count;
    int created = 0;

    int descriptor = open(AGAR_STATE_PATH, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (descriptor >= 0) {
        created = 1;
        if (ftruncate(descriptor, (off_t)state_bytes) != 0) return 2;
    } else if (errno == EEXIST) {
        descriptor = open(AGAR_STATE_PATH, O_RDWR);
        if (descriptor < 0) return 3;
    } else {
        return 4;
    }

    struct group *staff = getgrnam("staff");
    if (staff != NULL) (void)fchown(descriptor, (uid_t)-1, staff->gr_gid);
    (void)fchmod(descriptor, S_IRUSR | S_IWUSR | S_IRGRP);

    struct stat state_stat;
    if (fstat(descriptor, &state_stat) != 0 || (size_t)state_stat.st_size != state_bytes) {
        close(descriptor);
        return 5;
    }

    void *mapping = mmap(NULL, state_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor, 0);
    if (mapping == MAP_FAILED) {
        close(descriptor);
        return 6;
    }

    struct agar_header *header = mapping;
    uint8_t *cells = (uint8_t *)mapping + sizeof(*header);

    if (created) {
        memset(mapping, 0, state_bytes);
        memcpy(header->magic, agar_magic, sizeof(agar_magic));
        header->version = AGAR_FORMAT_VERSION;
        header->width = AGAR_WIDTH;
        header->height = AGAR_HEIGHT;
        header->cell_bytes = 1;
        establish_initial_field(cells);
        msync(mapping, state_bytes, MS_SYNC);
    } else if (memcmp(header->magic, agar_magic, sizeof(agar_magic)) != 0 ||
               header->version != AGAR_FORMAT_VERSION ||
               header->width != AGAR_WIDTH ||
               header->height != AGAR_HEIGHT ||
               header->cell_bytes != 1) {
        munmap(mapping, state_bytes);
        close(descriptor);
        return 7;
    }

    uint8_t *next = calloc(cell_count, 1);
    if (next == NULL) {
        munmap(mapping, state_bytes);
        close(descriptor);
        return 8;
    }

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = stop_medium;
    sigemptyset(&action.sa_mask);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);

    struct world_view world = {
        .descriptor = -1,
        .field = NULL
    };
    struct sparse_space depth = {0};
    try_open_world(&world);

    while (running) {
        memset(next, 0, cell_count);
        struct sparse_space next_depth = {0};

        advance_plane(cells, next, &next_depth, world.field != NULL);
        advance_depth(&depth, next, &next_depth, world.field != NULL);

        apply_world(next, world.field, arc4random_uniform(8));
        memcpy(cells, next, cell_count);
        sparse_free(&depth);
        depth = next_depth;

        sleep_one_beat();
    }

    msync(mapping, state_bytes, MS_SYNC);
    if (world.field != NULL) {
        munmap(world.field, sizeof(struct world_field));
        close(world.descriptor);
    }
    sparse_free(&depth);
    free(next);
    munmap(mapping, state_bytes);
    close(descriptor);
    return 0;
}
