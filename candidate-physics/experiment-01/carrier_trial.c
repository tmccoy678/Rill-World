#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODE_COUNT 48
#define CARGO_COUNT 29
#define TRACER_COUNT 72

#define DOMAIN_MIN_X 0.0
#define DOMAIN_MAX_X 40.0
#define OPEN_HALF_HEIGHT 7.0
#define THROAT_HALF_HEIGHT 2.25
#define THROAT_CENTER_X 20.0
#define THROAT_WIDTH 4.8

#define INITIAL_CENTER_X 7.5
#define INITIAL_RADIUS_X 3.1
#define INITIAL_RADIUS_Y 4.0

#define DT 0.004
#define FLOW_MAX 0.20
#define FLOW_COUPLING 0.85
#define SPRING_K 3.2
#define BEND_K 0.14
#define WALL_K 8.0
#define NODE_REPULSION_K 1.2
#define CARGO_REPULSION_K 0.75
#define CARGO_NODE_K 1.1
#define TRACER_NODE_K 0.18

struct point {
    double x;
    double y;
};

static struct point nodes[NODE_COUNT];
static struct point cargo[CARGO_COUNT];
static struct point tracers[TRACER_COUNT];
static double rest_length;

static struct point add(struct point a, struct point b) {
    return (struct point){a.x + b.x, a.y + b.y};
}

static struct point subtract(struct point a, struct point b) {
    return (struct point){a.x - b.x, a.y - b.y};
}

static struct point scale(struct point value, double factor) {
    return (struct point){value.x * factor, value.y * factor};
}

static double magnitude(struct point value) {
    return hypot(value.x, value.y);
}

static double half_height(double x) {
    double q = (x - THROAT_CENTER_X) / THROAT_WIDTH;
    double shape = 1.0 - exp(-q * q * q * q);
    return THROAT_HALF_HEIGHT + (OPEN_HALF_HEIGHT - THROAT_HALF_HEIGHT) * shape;
}

static double medium_velocity(double x, double y) {
    double height = half_height(x);
    double ratio = y / height;
    double profile = 1.0 - ratio * ratio;
    return FLOW_MAX * (profile > 0.0 ? profile : 0.0);
}

static void add_wall_response(struct point position, struct point *response) {
    double height = half_height(position.x);
    double clearance = height - fabs(position.y);
    const double margin = 0.28;
    if (clearance < margin) {
        double direction = position.y >= 0.0 ? -1.0 : 1.0;
        response->y += direction * WALL_K * (margin - clearance);
    }
    if (position.x < DOMAIN_MIN_X) {
        response->x += WALL_K * (DOMAIN_MIN_X - position.x);
    } else if (position.x > DOMAIN_MAX_X) {
        response->x -= WALL_K * (position.x - DOMAIN_MAX_X);
    }
}

static void add_pair_repulsion(
    struct point a,
    struct point b,
    double cutoff,
    double strength,
    struct point *response_a,
    struct point *response_b
) {
    struct point delta = subtract(a, b);
    double distance = magnitude(delta);
    if (distance <= 1e-9 || distance >= cutoff) return;
    double amount = strength * (cutoff - distance) / distance;
    struct point response = scale(delta, amount);
    *response_a = add(*response_a, response);
    *response_b = subtract(*response_b, response);
}

static void initialize_nodes(void) {
    const double tau = 2.0 * acos(-1.0);
    for (size_t i = 0; i < NODE_COUNT; ++i) {
        double angle = tau * (double)i / (double)NODE_COUNT;
        nodes[i].x = INITIAL_CENTER_X + INITIAL_RADIUS_X * cos(angle);
        nodes[i].y = INITIAL_RADIUS_Y * sin(angle);
    }
    rest_length = magnitude(subtract(nodes[1], nodes[0]));
}

static void initialize_cargo(void) {
    size_t index = 0;
    for (int row = -3; row <= 3 && index < CARGO_COUNT; ++row) {
        for (int column = -3; column <= 3 && index < CARGO_COUNT; ++column) {
            double x = INITIAL_CENTER_X + 0.62 * (double)column;
            double y = 0.72 * (double)row;
            double ex = (x - INITIAL_CENTER_X) / 2.45;
            double ey = y / 3.25;
            if (ex * ex + ey * ey < 0.88) {
                cargo[index++] = (struct point){x, y};
            }
        }
    }
    if (index != CARGO_COUNT) {
        fprintf(stderr, "cargo initialization failed: %zu\n", index);
        exit(2);
    }
}

static void initialize_tracers(void) {
    for (size_t i = 0; i < TRACER_COUNT; ++i) {
        double x = 0.4 + 0.42 * (double)(i % 12);
        double y = -5.8 + 1.05 * (double)(i / 12);
        tracers[i] = (struct point){x, y};
    }
}

static void apply_node_responses(struct point responses[NODE_COUNT]) {
    for (size_t i = 0; i < NODE_COUNT; ++i) {
        size_t next = (i + 1) % NODE_COUNT;
        struct point delta = subtract(nodes[next], nodes[i]);
        double distance = magnitude(delta);
        if (distance > 1e-9) {
            double amount = SPRING_K * (distance - rest_length) / distance;
            struct point response = scale(delta, amount);
            responses[i] = add(responses[i], response);
            responses[next] = subtract(responses[next], response);
        }
    }

    for (size_t i = 0; i < NODE_COUNT; ++i) {
        size_t previous = (i + NODE_COUNT - 1) % NODE_COUNT;
        size_t next = (i + 1) % NODE_COUNT;
        struct point curvature = add(nodes[previous], nodes[next]);
        curvature = subtract(curvature, scale(nodes[i], 2.0));
        responses[i] = add(responses[i], scale(curvature, BEND_K));
    }

    for (size_t i = 0; i < NODE_COUNT; ++i) {
        responses[i].x += FLOW_COUPLING * medium_velocity(nodes[i].x, nodes[i].y);
        add_wall_response(nodes[i], &responses[i]);
    }

    for (size_t i = 0; i < NODE_COUNT; ++i) {
        for (size_t j = i + 1; j < NODE_COUNT; ++j) {
            if (j == i + 1 || (i == 0 && j == NODE_COUNT - 1)) continue;
            add_pair_repulsion(
                nodes[i], nodes[j], 0.38, NODE_REPULSION_K,
                &responses[i], &responses[j]
            );
        }
    }
}

static void apply_cargo_responses(
    struct point cargo_responses[CARGO_COUNT],
    struct point node_responses[NODE_COUNT]
) {
    for (size_t i = 0; i < CARGO_COUNT; ++i) {
        cargo_responses[i].x += FLOW_COUPLING * medium_velocity(cargo[i].x, cargo[i].y);
        add_wall_response(cargo[i], &cargo_responses[i]);
    }

    for (size_t i = 0; i < CARGO_COUNT; ++i) {
        for (size_t j = i + 1; j < CARGO_COUNT; ++j) {
            add_pair_repulsion(
                cargo[i], cargo[j], 0.70, CARGO_REPULSION_K,
                &cargo_responses[i], &cargo_responses[j]
            );
        }
        for (size_t j = 0; j < NODE_COUNT; ++j) {
            add_pair_repulsion(
                cargo[i], nodes[j], 0.52, CARGO_NODE_K,
                &cargo_responses[i], &node_responses[j]
            );
        }
    }
}

static void advance_tracers(struct point node_responses[NODE_COUNT]) {
    for (size_t i = 0; i < TRACER_COUNT; ++i) {
        struct point response = {
            medium_velocity(tracers[i].x, tracers[i].y), 0.0
        };
        add_wall_response(tracers[i], &response);
        for (size_t j = 0; j < NODE_COUNT; ++j) {
            add_pair_repulsion(
                tracers[i], nodes[j], 0.34, TRACER_NODE_K,
                &response, &node_responses[j]
            );
        }
        tracers[i] = add(tracers[i], scale(response, DT));
    }
}

static bool point_inside(struct point point) {
    bool inside = false;
    size_t j = NODE_COUNT - 1;
    for (size_t i = 0; i < NODE_COUNT; ++i) {
        double yi = nodes[i].y;
        double yj = nodes[j].y;
        bool crosses = (yi > point.y) != (yj > point.y);
        if (crosses) {
            double x_cross = (nodes[j].x - nodes[i].x) *
                (point.y - yi) / (yj - yi) + nodes[i].x;
            if (point.x < x_cross) inside = !inside;
        }
        j = i;
    }
    return inside;
}

static struct point centroid(void) {
    struct point result = {0.0, 0.0};
    for (size_t i = 0; i < NODE_COUNT; ++i) result = add(result, nodes[i]);
    return scale(result, 1.0 / (double)NODE_COUNT);
}

static bool finite_state(void) {
    for (size_t i = 0; i < NODE_COUNT; ++i) {
        if (!isfinite(nodes[i].x) || !isfinite(nodes[i].y)) return false;
    }
    for (size_t i = 0; i < CARGO_COUNT; ++i) {
        if (!isfinite(cargo[i].x) || !isfinite(cargo[i].y)) return false;
    }
    return true;
}

static int parse_steps(int argc, char **argv) {
    int steps = 40000;
    if (argc == 1) return steps;
    if (argc != 3 || strcmp(argv[1], "--steps") != 0) {
        fprintf(stderr, "usage: %s [--steps N]\n", argv[0]);
        exit(64);
    }
    errno = 0;
    char *end = NULL;
    long value = strtol(argv[2], &end, 10);
    if (errno != 0 || end == argv[2] || *end != '\0' || value < 1 || value > 200000) {
        fprintf(stderr, "invalid step count\n");
        exit(64);
    }
    return (int)value;
}

int main(int argc, char **argv) {
    int steps = parse_steps(argc, argv);
    initialize_nodes();
    initialize_cargo();
    initialize_tracers();

    double initial_height = 2.0 * INITIAL_RADIUS_Y;
    for (int step = 0; step < steps; ++step) {
        struct point node_responses[NODE_COUNT] = {{0.0, 0.0}};
        struct point cargo_responses[CARGO_COUNT] = {{0.0, 0.0}};

        apply_node_responses(node_responses);
        apply_cargo_responses(cargo_responses, node_responses);
        advance_tracers(node_responses);

        for (size_t i = 0; i < NODE_COUNT; ++i) {
            nodes[i] = add(nodes[i], scale(node_responses[i], DT));
        }
        for (size_t i = 0; i < CARGO_COUNT; ++i) {
            cargo[i] = add(cargo[i], scale(cargo_responses[i], DT));
        }
        if (!finite_state()) {
            fprintf(stderr, "outcome=numerical_failure step=%d\n", step);
            return 2;
        }
    }

    struct point center = centroid();
    double min_x = nodes[0].x;
    double max_x = nodes[0].x;
    double min_y = nodes[0].y;
    double max_y = nodes[0].y;
    double max_bond = 0.0;
    for (size_t i = 0; i < NODE_COUNT; ++i) {
        size_t next = (i + 1) % NODE_COUNT;
        double bond = magnitude(subtract(nodes[next], nodes[i]));
        if (bond > max_bond) max_bond = bond;
        if (nodes[i].x < min_x) min_x = nodes[i].x;
        if (nodes[i].x > max_x) max_x = nodes[i].x;
        if (nodes[i].y < min_y) min_y = nodes[i].y;
        if (nodes[i].y > max_y) max_y = nodes[i].y;
    }

    size_t retained = 0;
    for (size_t i = 0; i < CARGO_COUNT; ++i) {
        if (point_inside(cargo[i])) retained += 1;
    }
    size_t tracers_passed = 0;
    for (size_t i = 0; i < TRACER_COUNT; ++i) {
        if (tracers[i].x > THROAT_CENTER_X + THROAT_WIDTH) tracers_passed += 1;
    }

    bool connected = max_bond < rest_length * 2.5;
    bool downstream = center.x > THROAT_CENTER_X + THROAT_WIDTH;
    double final_height = max_y - min_y;
    bool recovered = downstream && final_height > initial_height * 0.65;
    const char *outcome = "blocked_or_incomplete";
    if (!connected) outcome = "boundary_rupture";
    else if (downstream && retained >= CARGO_COUNT * 3 / 4 && recovered) outcome = "passage_with_recovery";
    else if (downstream) outcome = "passage_with_change";

    printf("steps=%d\n", steps);
    printf("outcome=%s\n", outcome);
    printf("centroid_x=%.6f centroid_y=%.6f\n", center.x, center.y);
    printf("carrier_width=%.6f carrier_height=%.6f\n", max_x - min_x, final_height);
    printf("max_adjacent_separation=%.6f rest_separation=%.6f\n", max_bond, rest_length);
    printf("carried_retained=%zu/%d\n", retained, CARGO_COUNT);
    printf("dissolved_passed=%zu/%d\n", tracers_passed, TRACER_COUNT);
    return 0;
}
