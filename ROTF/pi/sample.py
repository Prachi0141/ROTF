
import cv2
import urllib
import numpy as np

url = 'http://192.168.225.38:5000/shot.jpg'
face_cascade = cv2.CascadeClassifier('/home/shreeyash/opencv/data/haarcascades/haarcascade_frontalface_default.xml')

while True:
    
    imgResp = urllib.urlopen(url)
    imgNp = np.array(bytearray(imgResp.read()), dtype = np.uint8)
    frame = cv2.imdecode(imgNp, -1)

    try:
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray)

    #print(img)

        for (x,y,w,h) in faces:
            cv2.rectangle(frame, (x,y), (x+w, y+h), (255,0,0), 2)
            c_x = x+(w/2)
            c_y = y+(h/2)
            print("centre : (",c_x,",",c_y,")")
            send = {'x':c_x,'y':c_y}
            print(send)
        cv2.imshow('frame',frame)
        
            
    except:
        print("No image found")
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
        

cv2.destroyAllWindows()     