import cv2
from picamera2 import Picamera2

name = 'Matthieu' #replace with your name

cam = Picamera2()
config = cam.create_preview_configuration(main={"size":(512, 304), "format":"RGB888"})
cam.configure(config)
cam.start()
    
img_counter = 0

while True:
    
    image = cam.capture_array("main")
    cv2.imshow("Press Space to take a photo", image)

    k = cv2.waitKey(1)

    if k%256 == 32:
        # SPACE pressed
        img_name = "dataset/"+ name +"/image_{}.jpg".format(img_counter)
        cv2.imwrite(img_name, image)
        print("{} written!".format(img_name))
        img_counter += 1
        
    elif k%256 == 27: # ESC pressed
        print("Escape hit, closing...")
        break
    elif img_counter >= 10 :
        break

cv2.destroyAllWindows()
