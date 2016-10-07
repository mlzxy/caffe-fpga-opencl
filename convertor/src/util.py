import numpy as np
def tupleToList(tp):
    return [t for t in tp]

empty = np.zeros(0)


def printplus(obj, prefix=""):
    """
    Pretty-prints the object passed in.

    """
    # Dict
    if isinstance(obj, dict):
        for k, v in sorted(obj.items()):
            print u'{2} {0}: {1}'.format(k, v, prefix)

    # List or tuple
    elif isinstance(obj, list) or isinstance(obj, tuple):
        for x in obj:
            print prefix, x

    # Other
    else:
        print prefix, obj