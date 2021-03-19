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

mkdir -p bin

if [ $# -eq 1 ]; then
    if [ $1 == "test" ]; then
        $CC $src_files ./src/test.c -g -pthread -lgsl -lgslcblas -lm -o bin/test
        exit 0
    fi

    if [ $1 == "RNMC" ]; then
        $CC $src_files ./src/RNMC.c -g -pthread -lgsl -lgslcblas -lm -o bin/RNMC
        exit 0
    fi

    help_message
fi

help_message
