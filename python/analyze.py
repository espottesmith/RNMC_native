from RNMC import *
import sys
import pickle

sys.path.append('./mrnet/src')


import mrnet.network
import mrnet.core.mol_entry
import mrnet.core.reactions
import mrnet.core.rates


if len(sys.argv) != 3:
    print("usage: python analyze.py network_folder param_folder")
    exit()

network_folder = sys.argv[1]
param_folder = sys.argv[2]

with open(network_folder + "/rnsd.pickle",'rb') as f:
    rnsd = pickle.load(f)

sa = SimulationAnalyser(rnsd, network_folder)


LEDC_index = rnsd.find_index_from_mol_graph('./mrnet/test_files/reaction_network_files/LEDC.xyz',0)
Li2CO3_index = rnsd.find_index_from_mol_graph('./li2co3_0.xyz', 0)
sa.generate_pathway_report(Li2CO3_index)
