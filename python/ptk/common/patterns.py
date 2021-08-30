from abc import abstractmethod, abstractstaticmethod

class Singleton(type):
    def __init__(cls, name, bases, attrs, **kwargs):
        """[Base metaclass for a Singleton. A singleton ensures that ony one instance of a 
        class is ever instanciated. Not thead safe.
        
        To use implement a class like:
        
        class MySingleton(object, metaclass=Singleton):

            #implement as normal
        
        ]

        Args:
            name ([type]): [description]
            bases ([type]): [description]
            attrs ([type]): [description]
        """
        super().__init__(name, bases, attrs)

        cls._instance = None

    def __call__(cls, *args, **kwargs):
        if cls._instance is None:
            cls._instance = super().__call__(*args, **kwargs)
        return cls._instance



# class Meta(type):
#     @staticmethod 
#     def __call__():
#         return Meta._impl_call()

#     @abstractstaticmethod
#     def _impl_call():
#         pass