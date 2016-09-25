from layer_type import construct
from layer import *
import caffe
import caffe_protobuf as caffe_proto
from google.protobuf.text_format import Merge

class Net:
    def __init__(self, dataname, txt_path, model_path):
        self.net = caffe.Net(txt_path, model_path, caffe.TEST)
        self.net_proto = caffe_proto.NetParameter()
        self.dataname = dataname
        with open(txt_path, 'r') as f:
            Merge(f.read(), self.net_proto)
        self.layers = []
        self.prototxt_dict = {}
        for i in range(len(self.net_proto.layer)):
            self.prototxt_dict[self.net_proto.layer[i].name] = self.net_proto.layer[i]

        for i in range(len(self.net.layers)):
            layer = self.net.layers[i]
            name = self.net._layer_names[i]
            model_param, proto_param = None, None
            try:
                model_param = self.net.blobs[self.fitDataLayer(name, lowercase=True)]
                proto_param = self.prototxt_dict[name]
            except KeyError:
                param = {'blob':None, 'model_param':None, 'proto_param':None}
            l = construct(self.fitDataLayer(layer.type),
                                      {'blob':layer.blobs,
                                       'model_param':model_param,
                                       'proto_param':proto_param})
            if l:
                self.layers.append(l)

    def fitDataLayer(self, type, lowercase=False):
        name = lt.Data if type == self.dataname else type
        return name.lower() if lowercase else name

    def json(self):
        return {
            'num_layers':len(self.layers),
            'layers': [layer.json() for layer in self.layers]
        }