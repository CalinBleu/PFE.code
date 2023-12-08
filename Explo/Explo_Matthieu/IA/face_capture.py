import cv2
from picamera import PiCamera
from picamera.array import PiRGBArray
import os
import sys

name = str(sys.argv[1])
parent_dir = str(sys.argv[2])
path = os.path.join(parent_dir, name) 

try :
    os.mkdir(path)
except FileExistsError as error :
    print("INFO :" + format(error))
finally:

    cam = PiCamera()
    cam.resolution = (512, 304)
    cam.framerate = 10
    rawCapture = PiRGBArray(cam, size=(512, 304))
        
    img_counter = 0

    while True:
        for frame in cam.capture_continuous(rawCapture, format="bgr", use_video_port=True):
            image = frame.array
            cv2.imshow("Press Space to take a photo", image)
            rawCapture.truncate(0)
        
            k = cv2.waitKey(1)
            rawCapture.truncate(0)
            if k%256 == 27: # ESC pressed
                break
            elif k%256 == 32:
                # SPACE pressed
                img_name = parent_dir + name +"/image.jpg"
                cv2.imwrite(img_name, image)
                print("{} written!".format(img_name))
                img_counter += 1
                break
        
        if img_counter >= 1:
            cv2.destroyAllWindows()
            result = cv2.imread(img_name)
            while True:
                cv2.imshow("Press ESC to validate and Space to re-capture", result)
                k = cv2.waitKey(0)
                if k == 27:  # ESC key
                    print("Escape hit, closing...")
                    cv2.destroyAllWindows()
                    break
                elif k == 32:  # Space key to re-capture
                    cv2.destroyAllWindows()
                    img_counter = 0
                    break


        if k%256 == 27:
            print("Escape hit, closing...")
            break



    cv2.destroyAllWindows()
