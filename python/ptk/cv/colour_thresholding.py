import argparse
import cv2
import imutils
import numpy as np
import threading
import time
import sys
import os

from ptk.logging import *
import ptk


_colour_spaces = ["rgb", "hsv"]

def _is_valid_colourspace(value):
    return value in _colour_spaces

class _Image():

    def __init__(self, frame=None):
        if frame is not None:
            image = imutils.resize(frame, width=600)
            self.image = image.copy()
            hsvog = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
            self.hsv = hsvog.copy()
        else:
            self.image = None
            self.hsv = None

class _ColourLoaderBase:

    def __init__(self, colour_space):
        self.colour_space = colour_space
        self.image = _Image()

    def getImage(self):
        if self.image.image is None:
            return None
        else:
            if self.colour_space == "rgb":
                return self.image.image.copy()
            else:
                return self.image.hsv.copy()


class _ColourLoaderVideo(_ColourLoaderBase):

    def __init__(self, colour_space, capture_id):
        super(_ColourLoaderVideo, self).__init__(colour_space)
        self.capture_id = int(capture_id)
        self.realtime_thread = threading.Thread(target=self._capture_realtime)
        self.realtime_thread.start()
        time.sleep(3)
        
    def _capture_realtime(self):
        cap = cv2.VideoCapture(self.capture_id) # video capture source camera (Here webcam of laptop) 
        _,frame = cap.read() # return a single frame in variable `frame`

        while not ptk.is_shutdown():
            ret,frame = cap.read()
            self.image = _Image(frame)

class _ColourLoaderImage(_ColourLoaderBase):
    def __init__(self, colour_space, image_path):
        super(_ColourLoaderImage, self).__init__(colour_space)
        self.image_path = image_path
        frame = cv2.imread(image_path)
        if frame is None:
            raise TypeError("Image {} cannot be loaded".format(image_path))
        self.image = _Image(frame)

def destroyWindows():
    cv2.destroyAllWindows()

def on_mouse_click(event, x, y, flags, image):
    # if event == cv2.EVENT_LBUTTONUP:
    print("[{}, {}]: {}".format(y, x,image[y,x].tolist()))

def on_trackbar_change(position):
    return



def colour_thresholding_main(argv=sys.argv, stdout=sys.stdout, env=os.environ):
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--image", help="path to the image file", default=None)
    parser.add_argument("-r", "--realtime", help="Device id to read from", default=None)
    parser.add_argument("-c", "--colourspace", help="Which colour space to use", default='rgb')
    args = vars(parser.parse_args())

    if args["image"] and args["realtime"]:
        logerror("Cannot load image and use video stream. Select one only.")
        return
    
    colour_space = args["colourspace"]
    if not _is_valid_colourspace(colour_space):
        logerror("Colour space invalid Must be 'rgb' or 'hsv'.")
        return

    if args["image"]:
        loader = _ColourLoaderImage(colour_space, args["image"])
    elif args["realtime"]:
        loader = _ColourLoaderVideo(colour_space, args["realtime"])

    ptk.registerExitCallback(destroyWindows)
    cv2.namedWindow('Image Thresholding')

    if colour_space == "rgb":
        min1 = "Min R"
        min2 = "Min G"
        min3 = "Min B"
        max1 = "Max R"
        max2 = "Max G"
        max3 = "Max B"
    else:
        min1 = "Min H"
        min2 = "Min S"
        min3 = "Min V"
        max1 = "Max H"
        max2 = "Max S"
        max3 = "Max V"

    image_to_use = loader.getImage()
    # Set mouse callback to capture HSV value on click
    cv2.setMouseCallback("Image Thresholding", on_mouse_click, image_to_use)
    cv2.createTrackbar(min1, "Image Thresholding", 0, 255, on_trackbar_change)
    cv2.createTrackbar(min2, "Image Thresholding", 0, 255, on_trackbar_change)
    cv2.createTrackbar(min3, "Image Thresholding", 0, 255, on_trackbar_change)
    cv2.createTrackbar(max1, "Image Thresholding", 0, 255, on_trackbar_change)
    cv2.createTrackbar(max2, "Image Thresholding", 0, 255, on_trackbar_change)
    cv2.createTrackbar(max3, "Image Thresholding", 0, 255, on_trackbar_change)

    lower = np.array([0, 0, 0])
    upper = np.array([255, 255, 255])


    while True:
        image_to_use = loader.getImage()
        # # Get trackbar positions and set lower/upper bounds
        lower[0] = cv2.getTrackbarPos(min1, "Image Thresholding")
        lower[1] = cv2.getTrackbarPos(min2, "Image Thresholding")
        lower[2] = cv2.getTrackbarPos(min3, "Image Thresholding")
        upper[0] = cv2.getTrackbarPos(max1, "Image Thresholding")
        upper[1] = cv2.getTrackbarPos(max2, "Image Thresholding")
        upper[2] = cv2.getTrackbarPos(max3, "Image Thresholding")

        # Create a range mask then erode and dilate to reduce noise
        mask = cv2.inRange(image_to_use, lower, upper)
        mask = cv2.erode(mask, None, iterations=1)
        mask = cv2.dilate(mask, None, iterations=1)

        # Show range mask
        cv2.imshow("Mask", mask)
        cv2.imshow("Image Thresholding", image_to_use)


        if cv2.waitKey(1) & 0xFF == ord("q"):
            print(str(lower[0]) + "," + str(lower[1]) + "," + str(lower[2]) + " " + str(upper[0]) + "," + str(upper[1]) + "," + str(upper[2])) 
            break



    

