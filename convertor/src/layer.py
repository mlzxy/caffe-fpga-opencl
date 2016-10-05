import layer_type as lt
import numpy as np
from util import *
from collections import defaultdict

class Layer:
    def __init__(self, type, data):
        self.type = type
        self.bias = empty
        self.weight = empty
        self.blob = data['blob']
        self.model_param = data['model_param']
        self.proto_param = data['proto_param']
        layer_name = ''
        try:
            if self.proto_param:
                layer_name = ':'+self.proto_param.name
        except:
            pass
        self.info = 'Layer {0}{1}'.format(type, layer_name)
        self.param = defaultdict(lambda:int(0))
        if self.model_param:
            self.param['output_channel'] = self.model_param.channels
            self.param['output_width'] = self.model_param.width
            self.param['output_height'] = self.model_param.height

        self.prev = data['prev']
        if self.prev:
            self.param['input_channel'] = self.prev.param['output_channel']
            self.param['input_width'] = self.prev.param['output_width']
            self.param['input_height'] = self.prev.param['output_height']
            if not self.model_param:  ## activation layer
                self.param['output_channel'] = self.prev.param['output_channel']
                self.param['output_width'] = self.prev.param['output_width']
                self.param['output_height'] = self.prev.param['output_height']
        else:  ## data layer
            self.param['input_channel'] = self.param['output_channel']
            self.param['input_width'] = self.param['output_width']
            self.param['input_height'] = self.param['output_height']

    def fixPadding(self, key='input'):
        self.param[key+'_height'] += (2*self.param['pad'])
        self.param[key+'_width'] += (2*self.param['pad'])
    def json(self):
        self.param['output_fm_data_num'] = self.param['output_channel'] * self.param['output_width'] * self.param['output_height']
        self.param['input_fm_data_num'] = self.param['input_channel'] * self.param['input_width'] * self.param['input_height']
        return {
            'type': self.type,
            'weight':{
                'shape': tupleToList(self.weight.shape),
                'num_dim':len(self.weight.shape),
                'data': self.weight.flatten().tolist(),
                'num_data':self.weight.size
            },
            'bias':{
                'shape': tupleToList(self.bias.shape),
                'num_dim': len(self.bias.shape),
                'data': self.bias.flatten().tolist(),
                'num_data': self.bias.size
            },
            'info':self.info,
            'param':{
                'scale': self.param['scale'],
                'max_pool':self.param['max_pool'],
                'ave_pool': self.param['ave_pool'],
                'stride': self.param['stride'],
                'kernel_size':self.param['kernel_size'],
                'pad': self.param['pad'],
                'dilation': self.param['dilation'],
                'input_channel':self.param['input_channel'],
                'input_width':self.param['input_width'],
                'input_height':self.param['input_height'],
                'output_channel': self.param['output_channel'],
                'output_width': self.param['output_width'],
                'output_height': self.param['output_height'],
                'output_fm_data_num': self.param['output_fm_data_num'],
                'input_fm_data_num': self.param['input_fm_data_num']
            },
            'config':{
                'global_size':[1,1,1],
                'local_size':[1,1,1],
                'offset':[0,0,0]
            }
        }

    def config_json(self):
        return {
            'type':self.type,
            'weight':{
                'shape': tupleToList(self.weight.shape),
            },
            'bias':{
                'shape': tupleToList(self.bias.shape),
            },
            'param':{
                'input_channel':self.param['input_channel'],
                'input_width':self.param['input_width'],
                'input_height':self.param['input_height'],
                'output_channel': self.param['output_channel'],
                'output_width': self.param['output_width'],
                'output_height': self.param['output_height']
            },
            'config':{
                'global_size':[1,1,1],
                'local_size':[1,1,1],
                'offset':[0,0,0]
            }
        }

class Activation(Layer):
    def __init__(self, type, data):
        Layer.__init__(self, type, data)


class Convolution(Layer):
    def __init__(self, type, data):
        Layer.__init__(self, type, data)
        conv_param = self.proto_param.convolution_param
        self.param['kernel_size'] = conv_param.kernel_size[0]
        self.param['stride'] = conv_param.stride[0]
        if len(conv_param.dilation) > 0:
            self.param['dilation'] = conv_param.dilation[0]
        else:
            self.param['dilation'] = 1
        if len(conv_param.pad) > 0:
            self.param['pad'] = conv_param.pad[0]
        self.fixPadding()
        self.info = "{0} - Kernel [{1},{1}], Pad {2}, Stride {3}, Dilation {4} - Output Information: channel {5}, width {6}, height {7}"\
            .format(self.info, self.param['kernel_size'], self.param['pad'], self.param['stride'], self.param['dilation'],
                    self.param['output_channel'], self.param['output_width'], self.param['output_height'])
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
        self.param['pad'] = int(pooling_param.pad)
        self.fixPadding()
        self.param['stride'] = int(pooling_param.stride)
        self.param['kernel_size'] = pooling_param.kernel_size
        self.info = "{0} - Kernel [{1},{1}], Pad {3}, Stride {2} - Output Information: channel {4}, width {5}, height {6}" \
            .format(self.info, self.param['kernel_size'], self.param['stride'], self.param['pad'],
                    self.param['output_channel'], self.param['output_width'], self.param['output_height'])


class Data(Layer):
    def __init__(self, type, data):
        Layer.__init__(self, type, data)
        self.info = "{0} - Output Information: channel {1}, width {2}, height {3}"\
            .format(self.info, self.model_param.channels, self.model_param.width, self.model_param.height)
        self.param['scale'] = self.proto_param.transform_param.scale


class Output(Layer):
    def __init__(self, type, data):
        Layer.__init__(self, type, data)

class Padding(Layer):
    def __init__(self, type, data):
        Layer.__init__(self, type, data)
        self.param['pad'] = self.proto_param['pad']
        self.fixPadding(key='output')
        pass


lt.connect(lt.Relu, Activation)
lt.connect(lt.Pooling, Pooling)
lt.connect(lt.Convolution, Convolution)
lt.connect(lt.Data, Data)
lt.connect(lt.Padding, Padding)
lt.connect(lt.Output, Output)
