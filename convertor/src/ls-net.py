from __future__ import print_function
import argparse, json, sys
from os.path import realpath, join
from util import printplus


def printLayer(l):
    print("layer - ",l['type'])
    print("    param: ")
    printplus(l['param'], "        ")
    print("    config: ")
    printplus(l['config'],"        ")
    print("         weight_shape: ", l["weight"]["shape"])
    print("         bias_shape: ", l["bias"]["shape"])

def lsNet(net=None, layer=-1):
    for i in range(net['num_layers']):
        if layer == -1:
            printLayer(net['layers'][i])
            print("-----------------------------------------")
        elif layer == i:
            printLayer(net['layers'][i])



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--net", help="network json file", default="")
    parser.add_argument("-l", "--layer", help="layer id", default=-1)
    args = parser.parse_args()
    if args.net == "" :
        print("Please specify required network json file by -n, and config yaml file by -c.")


    with open(realpath(args.net)) as json_file:
        lsNet(net=json.load(json_file), layer=int(args.layer))