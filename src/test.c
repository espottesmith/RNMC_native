#include "test.h"

void test_samplers() {
  double inital_propensities[] = {.1, .1, .8, .1, .4, .1, .0};
  Solve *p = new_solve(linear, 42, 7, inital_propensities);
  Solve *q = new_solve(tree, 42, 7, inital_propensities);
  double dt = 0;
  bool result = true;

  for (int i = 0; i < 1000; i++) {
    int p_sample = p->event(p, &dt);
    int q_sample = q->event(q, &dt);
    if (p_sample != q_sample) {
      result = false;
      break;
    }
  }

  free_solve(p);
  free_solve(q);

  if (result)
    puts(ANSI_COLOR_GREEN
         "passed: sampler test"
         ANSI_COLOR_RESET);
  else
    puts(ANSI_COLOR_RED
         "failed: sampler test"
         ANSI_COLOR_RESET);
}

void test_disk_speed() {

  printf("testing disk speed....\n");
  Sampler *samplerp = new_sampler(42);
  unsigned long int length = 100000000;
  char *path = "/tmp/RNMC_speed_test";
  double *array_a = malloc(sizeof(double) * length);
  double *array_b = malloc(sizeof(double) * length);
  unsigned long int i;
  for (unsigned long int i = 0; i < length; i++) {
    array_a[i] = samplerp->generate(samplerp);
  }
  printf("initialized array_a\n");
  int t;
  int status;
  FILE *file = fopen(path, "w");
  t = time(NULL);
  for (i = 0; i < length; i++)
    fprintf(file, "%e\n", array_a[i]);
  t = time(NULL) - t;
  printf("100 million doubles write time %d seconds\n",t);
  fclose(file);

  file = fopen(path, "r");
  t = time(NULL);
  for (i = 0; i < length; i++)
    status = fscanf(file, "%lf\n", array_b + i);
  t = time(NULL) - t;
  printf("100 million doubles read time %d seconds\n",t);
  fclose(file);

  status = system("rm /tmp/RNMC_speed_test");
  free_sampler(samplerp);
  free(array_a);
  free(array_b);

}

// currently segfaults if not run from the correct directory
void test_serialization() {
  char *ronalds_network_dir = "./ronalds_network";
  char *test_network_dir = "./test_network";

  ReactionNetwork *rnp = new_reaction_network(ronalds_network_dir, false);
  reaction_network_to_file(rnp, test_network_dir);
  char *cmd = "diff -r --exclude=simulation_histories ./ronalds_network ./test_network > /dev/null";
  int status = system(cmd);
  // status code 0 means no differences
  // non zero status code means difference found
  if (status)
    puts(ANSI_COLOR_RED
         "failed: file to ReactionNetwork to file test"
         ANSI_COLOR_RESET);
  else
    puts(ANSI_COLOR_GREEN
         "passed: file to ReactionNetwork to file test"
         ANSI_COLOR_RESET);


  ReactionNetwork *rnp_copy = new_reaction_network(test_network_dir, false);

  if (reaction_networks_differ(rnp, rnp_copy)) {
    puts(ANSI_COLOR_RED
         "failed: ReactionNetwork to file to ReactionBetwork test"
         ANSI_COLOR_RESET);
  } else {
    puts(ANSI_COLOR_GREEN
         "passed: ReactionNetwork to file to ReactionBetwork test"
         ANSI_COLOR_RESET);

  }

  free_reaction_network(rnp);
  free_reaction_network(rnp_copy);
  status = system("rm -r ./test_network");
}

void run_test_simulation() {
  char *ronalds_network_dir = "./ronalds_network";
  ReactionNetwork *rnp = new_reaction_network(ronalds_network_dir, false);
  Simulation *sp = new_simulation(rnp, 42, tree);
  run_until(sp, 5);
  simulation_history_to_file(sp);
  char *cmd = "diff ./test_simulation_histories/42 ./ronalds_network/simulation_histories/42 > /dev/null";
  int status = system(cmd);
  // status code 0 means no differences
  // non zero status code means difference found
  if (status)
    puts(ANSI_COLOR_RED
         "failed: seed 42 simulation has changed"
         ANSI_COLOR_RESET);
  else
    puts(ANSI_COLOR_GREEN
         "passed: seed 42 simulation normal"
         ANSI_COLOR_RESET);

  if (sp->step == simulation_history_length(sp->history))
        puts(ANSI_COLOR_GREEN
         "passed: history length agrees with number of steps"
         ANSI_COLOR_RESET);
  else
        puts(ANSI_COLOR_RED
         "failed: history length is different to number of steps"
         ANSI_COLOR_RESET);


  free_simulation(sp);
  free_reaction_network(rnp);

}

void run_test_dispatcher() {
  Dispatcher *dp = new_dispatcher("./ronalds_network","./simulation_params", false);
  run_dispatcher(dp);
  free_dispatcher(dp);

  char *cmd = "diff -r ./test_simulation_histories ./ronalds_network/simulation_histories > /dev/null";
  int status = system(cmd);
  // status code 0 means no differences
  // non zero status code means difference found
  if (status)
    puts(ANSI_COLOR_RED
         "failed: dispatcher produced different result"
         ANSI_COLOR_RESET);
  else
    puts(ANSI_COLOR_GREEN
         "passed: dispatcher behaving correctly"
         ANSI_COLOR_RESET);




}

void run_tests(char *test_materials_dir) {
  int status = chdir(test_materials_dir);
  test_samplers();
  test_serialization();
  run_test_simulation();
  run_test_dispatcher();
}


int main(int argc, char **argv) {
  if (argc != 2)
    puts("usage: ./test test_materials_directory");
  else
    run_tests(argv[1]);

  pthread_exit(NULL);
}
