# RNMC

RNMC is a program for simulating reaction networks heavily inspired by [SPPARKS](https://spparks.sandia.gov/). RNMC is designed to run large numbers of simulations of a fixed system (fixed reaction network and fixed initial state) in parallel.

### Dependencies

RNMC depends on [GSL](https://www.gnu.org/software/gsl/) for pseudo random number generation. Make sure that `CPATH`, `LIBRARY_PATH` and `LD_LIBRARY_PATH` are set appropriately if you aren't using a version of GSL bundled with your system.

### Building

- To build the tests, run `./build test`. The tests are run as `./test ./test_materials`.
- To build RNMC, run `./build RNMC`. RNMC is run as `./RNMC reaction_network_dir simulation_params`. After running, the simulation histories will be stored in `./reaction_network_dir/simulation_histories`.

### reaction_network_dir

Inorder to remain indipendent of any particular language wrapper, RNMC ingests reaction networks from a series of files. A folder containing these files is the first argument to `RNMC`
- `number_of_reactions`:
- `number_of_species`:
- `initial_state`: initial state of the simulation.
- `number_of_products`: number of products of each reaction
- `products`: products of each reaction
- `number_of_reactants`: number of reactants of each reaction
- `reactants`: reactants of each reaction
- `rates`: rates for each reaction
- `factor_zero`: rate modifier for reactions of the form (nothing) -> ...
- `factor_two`: rate modifier for reactions of the form A + B -> ...
- `factor_duplicate`: rate modifier for reactions of the form A + A -> ...

See `./test_materials/ronalds_network` for an example (thanks Ronald!)


### simulation_params

folder containing the following files:

- `number_of_seeds`:
- `number_of_threads`:
- `seeds`: list of seeds. `0` is not considered a valid seed, and if a thread receives zero, it will terminate.
- `time_cutoff`: how long to run the simulations (in simulation time)
- `step_cutoff`: how many steps to run the simulation

Note: Only one of `time_cutoff` or `step_cutoff` is needed. if `step_cutoff` is present, it will be used over `time_cutoff`

See `./test_materials/simulation_params` for an example.
