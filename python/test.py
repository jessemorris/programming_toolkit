import ptk
import cv2
# ptk.loginfo("here")
# ptk.loginfo("here also")
# # import time

# # class FakeList():

# #         def __init__(self):
# #             self.x = 1
# #             self.y = 2
# #             self.z = 3

# #         def __iter__(self):
# #             return ptk.common.ClassIterable(self.x, self.y, self.z)

# # fl = FakeList()
# # x,y,z = fl
# # print(x,y,z) 

# def func():
#     ptk.loginfo("Finished")

# ptk.registerExitCallback(func)

# while True:
#     # time.sleep(3)
#     ptk.loginfo_throttle(3, "spinning")
# # ptk.logwarn("spinning")

# def test(a, b, c=5):
#         pass

import inspect
print(cv2.Canny.__doc__)
print(inspect.getargspec(cv2.Canny))

# function_params = ptk.utils.inspect_func_params(cv2.Canny)

# for params in function_params:
#     print(params)