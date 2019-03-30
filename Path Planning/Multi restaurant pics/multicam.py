#from bs4 import BeautifulSoup
import urllib3.request
import cv2
import numpy as np
import os
import subprocess

import time

ex = -1
ey = -1
sx = 140
sy = 33
k = 0
gs = 10
n = 30
dest = 0
paramimg = 0
outF = open('myOutFile.txt', 'w')

# mouse callback function
def draw_circle(event, x, y, flags, param):
    global ex, ey, k, paramimg
    if event == cv2.EVENT_LBUTTONDBLCLK:
        if k == 0:
            ex = x
            ey = y
            k += 1

        cv2.circle(sdio[param -1], (x, y), 8, (255, 0, 0), -1)
        print(param, x, y)
        paramimg = param

def scratch_fun(filename, src, dest, cNo):
    global gs, n
    frame = cv2.imread(filename)
    global OutF

    cv2.imshow('Original' + str(cNo), frame)
    frame = cv2.resize(frame, (300, 300))
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lower_red = np.array([201, 163, 150])
    upper_red = np.array([231, 163, 150])

    mask = cv2.inRange(hsv, lower_red, upper_red)
    res = cv2.bitwise_and(frame, frame, mask=mask)
    edges = cv2.Canny(frame, 200, 200)
    temp = cv2.resize(edges, (300, 300))
    #cv2.imshow('temp + str(cNo)', temp)
    tes = cv2.add(mask, edges)
    kernel = np.ones((20, 20), np.float32)/225
    smoothed = cv2.filter2D(tes, -1, kernel)
    temp = smoothed
    temp2 = np.empty((30, 30), dtype=int)
    #make_grid(temp, temp2)

    for j in range(n):
        for i in range(n):
            img = temp[j * gs:(j + 1) * gs, i * gs:(i + 1) * gs]
            data = np.asarray(img, dtype=int)
            white = 0
            black = 0
            for x in range(gs):
                for y in range(gs):
                    if data[x, y] > 100:
                        white += 1
                    else:
                        black += 1
            if black > 90:
                temp[j * gs:(j + 1) * gs, i * gs:(i + 1) * gs] = 0
                temp2[j, i] = 1
            else:
                temp[j * gs:(j + 1) * gs, i * gs:(i + 1) * gs] = 255
                temp2[j, i] = 0

    cv2.imshow('temp' + str(cNo), temp)
    g = open('grids.txt', 'w')
    stcam = str(src) + " " + str(dest)
    g.write(stcam + "\n")
    np.savetxt("grids.txt", temp2)
    g.close()

    os.system("g++ dijk_path.cpp") #For Compiling
    os.system("./a.out")
    #cv2.imshow("result", result)

    theFile = open('srcdest.txt', 'r')
    arr = []
    for val in theFile.read().split():
        arr.append(int(val))
    print(arr)
    l = len(arr)
    r = 0
    s = 0
    face = arr[0]
    for i in range(1, l):
        v = arr[i]
        p = v//30
        q = v % 30
        frame[p * gs:(p + 1) * gs, q * gs:(q + 1) * gs, :] = [0, 255, 0]
        print("l")
        temp[p * gs:(p + 1) * gs, q * gs:(q + 1) * gs] = 150

        if p == r+1:
            print("R")
            outF.write('R\n')

        if q == s-1:
            outF.write('B\n')
            print("B")

        if q == s+1:
            outF.write('F\n')
            print("F")

        if p == r-1:
            outF.write('L\n')
            print("L")

        print(p, q)
        print(r, s)
        r = p

    cv2.imshow('Path' + str(cNo), frame)
    theFile.close()

#main
start_time = time.time()
sdio = []
cmd = "dijk_path.cpp"
#cap = cv2.VideoCapture('p_vid.mp4')
imgno = []
for i in range(1, 5):
    imgno.append(i)
for i in range(1, 5):
    st = 'multic (' + str(i) + ').jpeg'
    frame = cv2.imread(st)
    img = cv2.resize(frame, (300, 300))
    sdio.append(img)
    cv2.namedWindow('image'+str(i))

    #frame = cv.imread('finaldrawr.jpg')
    cv2.setMouseCallback('image'+ str(i), draw_circle, imgno[i-1])



while(True):
    for i in range(1, 5):
        cv2.imshow('image'+str(i), sdio[i-1])


    if cv2.waitKey(20) & 0xFF == 27:
        break

print(sx, sy, ex, ey)
src = ((sy//10)*30) + (sx//10)
dest = ((ey//10)*30) + (ex//10)
st = str(src)+" " + str(dest)
print(st)

srccam = 0
destcam = paramimg - 1
camrow = 2
camcol = 2

outcam = open('gridscam.txt', 'w')
outcam.write(str(srccam)+" " + str(destcam) + "\n")
camgrid = np.empty((camrow, camcol), dtype=int)
for i in range(camrow):
    for j in range(camcol):
        outcam.write(str(1) + " ")
    outcam.write("\n")
np.savetxt("gridscam.txt", camgrid)
outcam.close()

os.system("g++ cams.cpp") #For Compiling
os.system("./a.out")


outF = open('myOutFile.txt', 'w')
srcCver = src
destCver = dest
theCam = open('camno.txt', 'r')
arr = []
cNo = 0
for val in theCam.read().split():
    arr.append(int(val))
print(arr)
lCam = len(arr)
rCam = 0
sCam = 0
for i in range(1, lCam):
    v = arr[i]
    p = v//camrow
    q = v % camcol

    if p == rCam+1:
        destCver = 854
        srcCver = 44

    if q == sCam-1:
        destCver = 421
        srcCver = 448

    if q == sCam+1:
        destCver = 448
        srcCver = 421

    if p == rCam-1:
        destCver = 44
        srcCver = 854

    print(p, q)
    print(rCam, sCam)

    fileName = 'multic (' + str(cNo + 1) + ').jpeg'
    scratch_fun(fileName, src, destCver, cNo)

    r = p
    s = q
    cNo = v
    src = srcCver

fileName = 'multic (' + str(cNo+1) + ').jpeg'
scratch_fun(fileName, src, dest, cNo)

theCam.close()



while(True):
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break



outF.close()




#cap.release()
cv2.destroyAllWindows()
