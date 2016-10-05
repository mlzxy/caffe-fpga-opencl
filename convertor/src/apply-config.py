import argparse, json, sys
from os.path import realpath, join


def configNet(net=None, config=None):
    for i in range(net['num_layers']):
        net['layers'][i]['config'] = config['layers'][i]['config']
    return net



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--net", help="network json file", default="")
    parser.add_argument("-c", "--config", help="configuration yaml file", default="")
    parser.add_argument("-o", "--output", help="output network json file", default="")
    args = parser.parse_args()
    if args.net == "" or args.config == "":
        print("Please specify required network json file by -n, and config yaml file by -c.")

    output = args.output
    if output == "":
        output = args.net

    with open(realpath(args.net)) as json_file:
        with open(realpath(args.config)) as config_file:
            json_data = configNet(net=json.load(json_file), config=json.load(config_file))
            with open(realpath(output), 'w+') as out_file:
                json.dump(json_data, out_file)