import argparse, json, sys
from net import *
from os.path import realpath, join

def convert(model_path='',  output_dir='',  name='', prototxt_path=''):
    print("Parsing caffe model...")
    net = Net(name, realpath(prototxt_path), realpath(model_path))
    print("Finished")
    with open(join(realpath(output_dir),name + '.json'), 'w+') as outfile:
        json.dump(net.json(), outfile)
        print("Dump model data into json file {0}.json".format(name))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--prototxt_path", help="Prototxt of the net to read in and translate", default=".")
    parser.add_argument("-m", "--model_path", help="Model value of the net to read in and translate", default=".")
    parser.add_argument("-o", "--output_dir", help="Output folder for the translated Json", default=".")
    parser.add_argument("-n", "--name", help="output file name", default='OutputNet')
    args = parser.parse_args()
    convert(model_path=args.model_path,
            output_dir=args.output_dir,
            name=args.name,
            prototxt_path=args.prototxt_path)