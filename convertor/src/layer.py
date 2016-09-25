import layer_type as lt
import numpy as np
from util import *
from collections import defaultdict

class Layer:
    def __init__(self, type, data):
        self.type = type
        self.bias = empty
        self.weight = empty
        self.info = 'Layer {0}'.format(type)
        self.blob = data['blob']
        self.model_param = data['model_param']
        self.proto_param = data['proto_param']
        self.param = defaultdict(lambda:0)

    def json(self):
        return {
            'type': self.type,
            'weight':{
                'shape': tupleToList(self.weight.shape),
                'data': self.weight.flatten().tolist()
            },
            'bias':{
                'shape': tupleToList(self.bias.shape),
                'data': self.bias.flatten().tolist()
            },
            'info':self.info,
            'param':{
                'scale':self.param['scale'],
                'max_pool':self.param['max_pool'],
                'ave_pool': self.param['ave_pool'],
                'stride': self.param['stride'],
                'kernel_size':self.param['kernel_size'],
                'pad': self.param['pad']
            }
        }

class Activation(Layer):
    pass


class Convolution(Layer):
    def __init__(self, type, data):
        Layer.__init__(self, type, data)
        conv_param = self.proto_param.convolution_param
        self.param['kernel_size'] = conv_param.kernel_size[0]
        self.param['stride'] = conv_param.stride[0]
        if len(conv_param.dilation) > 0:
            self.param['dilation'] = conv_param.dilation[0]
        if len(conv_param.pad) > 0:
            self.param['pad'] = conv_param.pad[0]

        self.info = "{0} - Kernel [{1},{1}], Pad {2}, Stride {3} - Output Information: channel {4}, width {5}, height {6}"\
            .format(self.info, self.param['kernel_size'], self.param['pad'], self.param['stride'],
                    self.model_param.channels, self.model_param.width, self.model_param.height)
        self.weight = self.blob[0].data
        self.bias = self.blob[1].data

class Pooling(Layer):
    def __init__(self, type, data):
        Layer.__init__(self, type, data)
        pool = self.proto_param.pooling_param.pool
        pooling_param = self.proto_param.pooling_param
        if pool == pooling_param.AVE:
            self.param['ave_pool'] = 1
        else:
            self.param['max_pool'] = 1
        self.param['stride'] = pooling_param.stride
        self.param['kernel_size'] = pooling_param.kernel_size
        self.info = "{0} - Kernel [{1},{1}], Stride {2} - Output Information: channel {3}, width {4}, height {5}" \
            .format(self.info, self.param['kernel_size'], self.param['stride'],self.model_param.channels, self.model_param.width, self.model_param.height)


class Data(Layer):
    def __init__(self, type, data):
        Layer.__init__(self, type, data)
        self.info = "{0} - Output Information: channel {1}, width {2}, height {3}"\
            .format(self.info, self.model_param.channels, self.model_param.width, self.model_param.height)
        self.param['scale'] = self.proto_param.transform_param.scale



lt.connect(lt.Relu, Activation)
lt.connect(lt.Pooling, Pooling)
lt.connect(lt.Convolution, Convolution)
lt.connect(lt.Data, Data)