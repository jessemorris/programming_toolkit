class ClassIterable():
    """[Helper class to enable objects to be iterated agaist. This is super helpful for Seralizing objects
    which are usualy held as arrays (eg coordinates or numberica values) and this avoids additionally code in order to 
    revtrieve them.
    
    
    Objects intenting to be iterable should return this class in their __iter__(self) function
    eg.
    
    Class FakeList():

        def __init__(self):
            self.x = 1
            self.y = 2
            self.z = 3

        def __iter__(self):
            return ClassIterable(self.x, self.y, self.z)

    fl = FakeList()
    x,y,z = fl
    print(x,y,z) 
    > 1, 2, 3

    ]
    """

    def __init__(self, *args):
        self.__iterator_values = args
        self.__iterator_index = 0

    def __next__(self):
        if self.__iterator_index < len(self.__iterator_values):
            result = self.__iterator_values[self.__iterator_index]
            self.__iterator_index +=1
            return result
        else:
            self.__iterator_index = 0
            raise StopIteration


import inspect

class FunctionParam(object):
    """[Function wrapper to describe a single function argument, containing its
    type, variable name as a string and default value if any]

    Args:
        object ([type]): [description]
    """
    def __init__(self, v_name:str, param_type: inspect.Parameter.kind, default=None):
        self.v_name = v_name
        self.type = param_type
        self.default = default

    def __repr__(self) -> str:
        if self.default:
            return "{} ({}), default={}".format(self.v_name, str(self.type), self.default)
        else:
            return "{} ({})".format(self.v_name, str(self.type))

class FunctionSignature(object):
    


    def __init__(self, signature: inspect.Signature):
        self._params = []
        for param in signature.parameters.items():
            if param[1].default is param[1].empty:
                self._params.append(FunctionParam(param[0], param[1].kind))
            else:
                self._params.append(FunctionParam(param[0], param[1].kind, param[1].default))


    def __iter__(self):
            return ClassIterable(self._params)