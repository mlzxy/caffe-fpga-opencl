from layer_type import construct, Output as OutputType, Padding as PaddingType
from layer import *
import caffe
import caffe_protobuf as caffe_proto
from google.protobuf.text_format import Merge
from collections import defaultdict as ddict


class Net:
    def __init__(self, dataname, txt_path, model_path, config):
        self.net = caffe.Net(txt_path, model_path, caffe.TEST)
        self.net_proto = caffe_proto.NetParameter()
        self.config = {token[0]:token[1] for token in config}
        self.dataname = dataname
        with open(txt_path, 'r') as f:
            Merge(f.read(), self.net_proto)
        self.layers = []
        self.prototxt_dict = {}
        for i in range(len(self.net_proto.layer)):
            self.prototxt_dict[self.net_proto.layer[i].name] = self.net_proto.layer[i]

        prev_layer = None
        for i in range(len(self.net.layers)):
            layer = self.net.layers[i]
            name = self.net._layer_names[i]
            model_param, proto_param = None, None
            try:
                model_param = self.net.blobs[self.fitDataLayer(name, lowercase=True)]
                proto_param = self.prototxt_dict[name]
            except KeyError:
                pass
            l = construct(self.fitDataLayer(layer.type),
                                      {'blob':layer.blobs,
                                       'model_param':model_param,
                                       'proto_param':proto_param,
                                       'prev':prev_layer})
            if l:
                if l.param['pad'] > 0:
                    lpadding = construct(PaddingType, {'blob':None, 'proto_param':{'pad':l.param['pad']},'model_param': None,'prev':prev_layer})
                    self.layers.append(lpadding)
                prev_layer = l
                self.layers.append(l)

        self.layers.append(construct(OutputType, {
            'blob':None,
            'model_param': None,
            'proto_param': None,
            'prev':prev_layer
        }))

    def fitDataLayer(self, type, lowercase=False):
        name = lt.Data if type == self.dataname else type
        return name.lower() if lowercase else name

    def json(self):
        return {
            'num_layers':len(self.layers),
            'layers': [layer.json() for layer in self.layers],
            'config': self.config,
            'max_output_fm_data_num':max([l.param['output_fm_data_num'] for l in self.layers]),
            'max_input_fm_data_num':max([l.param['input_fm_data_num'] for l in self.layers]),
            'max_weight_data_num':max([l.weight.size for l in self.layers]),
            'max_bias_data_num':max([l.bias.size for l in self.layers])
        }