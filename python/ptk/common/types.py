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
            return seralizer.ClassIterable(self.x, self.y, self.z)

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