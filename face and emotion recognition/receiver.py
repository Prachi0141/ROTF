from socket import *
import numpy as np
import cv2
import time
host = "10.42.0.1"
port = 5000
buf = 32768*32768
addr = (host, port)
fName = 'img.jpg'
timeOut = 0.005
def foo():
    while True:
        s = socket(AF_INET, SOCK_DGRAM)
        s.bind(addr)

        try:
            data, address = s.recvfrom(buf)
            f = open(data, 'wb')

            data, address = s.recvfrom(buf)
        except:
            f = open("h.txt", 'wb')
            print "err"            

        try:
            while(data):
                f.write(data)
                s.settimeout(timeOut)
                data, address = s.recvfrom(buf)
        except timeout:
            f.close()
            s.close()
        image = cv2.imread(fName)
        try:
            cv2.imshow('recv', image)
        except:
            continue
        #time.sleep(1)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

if __name__ == '__main__':
    foo()
cv2.destroyAllWindows()
