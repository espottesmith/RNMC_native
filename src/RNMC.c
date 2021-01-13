#include "dispatcher.h"

int main(int argc, char **argv) {
  if (argc != 3)
    puts("usage: ./RNMC reaction_network_dir simulation_params");

  else {
    Dispatcher *dp = new_dispatcher(argv[1],
                                    argv[2],
                                    true);
    run_dispatcher(dp);
    free_dispatcher(dp);
  }

  pthread_exit(NULL);
}
