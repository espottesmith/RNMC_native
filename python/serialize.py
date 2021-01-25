import sys
sys.path.append('./mrnet/src')

import mrnet.network
import mrnet.core.mol_entry
import mrnet.core.reactions
import mrnet.core.rates
sys.modules['pymatgen.reaction_network'] = mrnet.network
sys.modules['pymatgen.entries.mol_entry'] = mrnet.core.mol_entry
sys.modules['pymatgen.reaction_network.reaction'] =  mrnet.core.reactions
sys.modules['pymatgen.reaction_network.reaction_rates'] = mrnet.core.rates

from RNMC import *

import pickle
# loaded using mrnet b113516
ledc_no_hop_file = "./ronalds_network"
with open(ledc_no_hop_file, 'rb') as f:
    ledc_no_hop_reaction_network = pickle.load(f)


if len(sys.argv) != 3:
    print("usage: python serialize.py network_folder param_folder")
    exit()

network_folder = sys.argv[1]
param_folder = sys.argv[2]

initial_state_data = [
    ('./Li.xyz', 1, 30),
    ('./mrnet/test_files/reaction_network_files/EC.xyz',0,30)
    ]

rnsd = ReactionNetworkSerializationData(ledc_no_hop_reaction_network,
                                        initial_state_data,
                                        network_folder,
                                        param_folder,
                                        logging = True)



serialize_reaction_network(rnsd)
serialize_simulation_parameters(seeds=range(1000,10000),
                                number_of_threads=7,
                                time_cutoff=5.0,
                                folder=rnsd.param_folder)

print("run simulation with ../RNMC " + network_folder + " " + param_folder )


