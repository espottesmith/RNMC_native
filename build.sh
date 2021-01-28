src_files="./src/reaction_network.c
           ./src/sampler.c
           ./src/simulation.c
           ./src/solvers.c
           ./src/dispatcher.c"

help_message() {
    echo "usage"
    echo "./build test"
    echo "./build RNMC"
    exit 1
}


if [ $# -eq 1 ]; then
    if [ $1 == "test" ]; then
        gcc $src_files ./src/test.c -g -pthread -lgsl -lgslcblas -lm -o test
        exit 0
    fi

    if [ $1 == "RNMC" ]; then
        gcc $src_files ./src/RNMC.c -g -pthread -lgsl -lgslcblas -lm -o RNMC
        exit 0
    fi

    help_message
fi

help_message
