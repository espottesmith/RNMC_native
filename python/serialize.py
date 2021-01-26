import sys
sys.path.append('./mrnet/src')
from mrnet.network.reaction_network import *
from monty.serialization import loadfn
from RNMC import *


if len(sys.argv) != 3:
    print("usage: python serialize.py network_folder param_folder")
    exit()

network_folder = sys.argv[1]
param_folder = sys.argv[2]

# ronalds molecule list doesn't have many high energy intermediate molecules
# this is why it doesn't produce many Li_plus + EC -> LEDC
molecule_entries = loadfn("./ronalds_MoleculeEntry.json")
base_reaction_network = ReactionNetwork.from_input_entries(molecule_entries)
base_reaction_network.build()

concerted_reactions = ReactionNetwork.identify_concerted_rxns_via_intermediates(
    base_reaction_network,
    [e.parameters["ind"] for e in base_reaction_network.entries_list])

reaction_network = ReactionNetwork.add_concerted_rxns(
    base_reaction_network,
    base_reaction_network,
    concerted_reactions)

initial_state_data = [
    ('./Li.xyz', 1, 30),
    ('./mrnet/test_files/reaction_network_files/EC.xyz',0,30)
    ]

rnsd = ReactionNetworkSerializationData(reaction_network,
                                        initial_state_data,
                                        network_folder,
                                        param_folder,
                                        logging = True)



serialize_reaction_network(rnsd)
serialize_simulation_parameters(seeds=range(1000,11000),
                                number_of_threads=7,
                                time_cutoff=5.0,
                                folder=rnsd.param_folder)

print("run simulation with ../RNMC " + network_folder + " " + param_folder )
