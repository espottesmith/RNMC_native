#include "solvers.h"
#include "sampler.h"
#include "reaction_network.h"
#include "simulation.h"
#include "dispatcher.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void test_samplers();
void test_serialization();
void run_tests();
void test_disk_speed();
void run_simulation();
void run_test_dispatcher();
