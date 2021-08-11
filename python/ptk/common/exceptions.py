class PtkException(Exception):
    """
    Base exception
    """

    def __init__(self, message=None):
        self.message = message
        super().__init__(self.message)

    def __str__(self):
        if self.message:
            return type(self).__name__ + ": " + self.message
        else:
            return type(self).__name__

class LaunchConfigException(PtkException):
    pass