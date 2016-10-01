Convolution = 'Convolution'
Relu = 'ReLU'
Data = 'Data'
Split = 'Split'
Pooling = 'Pooling'
Accuracy = 'Accuracy'
SoftmaxWithLoss = 'SoftmaxWithLoss'
Padding = 'Padding'
Output = 'Output'
constructor_map = {}


layer_type_list = [Convolution, Relu, Data, Split, Pooling, Accuracy, SoftmaxWithLoss, Padding, Output]

Nop = lambda x, y: None

for t in layer_type_list:
    constructor_map[t] = Nop


def connect(t, c):
    constructor_map[t] = c

def construct(t, arg):
    return constructor_map[t](t, arg)
