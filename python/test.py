import ptk
ptk.loginfo("here")
ptk.loginfo("here also")
# import time

# class FakeList():

#         def __init__(self):
#             self.x = 1
#             self.y = 2
#             self.z = 3

#         def __iter__(self):
#             return ptk.common.ClassIterable(self.x, self.y, self.z)

# fl = FakeList()
# x,y,z = fl
# print(x,y,z) 

def func():
    ptk.loginfo("Finished")

ptk.registerExitCallback(func)

while True:
    # time.sleep(3)
    ptk.loginfo_throttle(3, "spinning")
# ptk.logwarn("spinning")