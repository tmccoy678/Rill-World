#include <assert.h>
#include <stdint.h>

#define main rill_runtime_main
#include "../agar_medium.c"
#undef main

int main(void) {
    const uint8_t planar_opposition =
        PARTICLE_NORTH | PARTICLE_EAST |
        PARTICLE_SOUTH | PARTICLE_WEST;

    assert(interact(DIRECTION_MASK) == 0);
    assert(interact(DIRECTION_MASK | MATTER_BOUND | MATTER_AGED) == 0);

    assert(interact(PARTICLE_NORTH | PARTICLE_SOUTH) ==
           (PARTICLE_EAST | PARTICLE_WEST));
    assert(interact(PARTICLE_EAST | PARTICLE_WEST) ==
           (PARTICLE_POS_Z | PARTICLE_NEG_Z));
    assert(interact(PARTICLE_POS_Z | PARTICLE_NEG_Z) ==
           (PARTICLE_NORTH | PARTICLE_SOUTH));

    assert(interact(planar_opposition) ==
           (planar_opposition | MATTER_BOUND));
    assert(interact(planar_opposition | MATTER_BOUND) ==
           (planar_opposition | MATTER_BOUND | MATTER_AGED));
    assert(interact(planar_opposition | MATTER_BOUND | MATTER_AGED) ==
           planar_opposition);

    return 0;
}
