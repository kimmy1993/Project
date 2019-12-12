"""
from picamera.array import PiRGBArray
import time
import picamera


camera = picamera()
camera.resolution = (480, 320)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size = (480, 320))

fime.sleep(0,1)

for frame in camera.capture_continuous(rawCapture, format = 'bgr', use_video_port = True):
    image = frame.array

    cv2.imshow('Frame', image)
    key = cv2.waitKey(1) & 0xff
    rawCapture.truncate(0)
    if key == 27:
        break

"""

from picamera import PiCamera
from time import sleep

camera = PiCamera()


camera.start_preview()
"""
sleep(20)
camera.stop_preview()
"""

sleep(5)
camera.capture('/home/pi/Desktop/image.jpg')
camera.stop_preview()
