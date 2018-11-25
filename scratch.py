from bs4 import BeautifulSoup
import urllib3.request
import cv2
import numpy as np
import os
import subprocess

import time

ex = -1
ey = -1
sx = -1
sy = -1
k = 0
# mouse callback function
def draw_circle(event, x, y, flags, param):
    global sx, sy, ex, ey, k
    if event == cv2.EVENT_LBUTTONDBLCLK:
        if k == 0:
            sx = x
            sy = y
            k += 1
        elif k == 1:
            ex = x
            ey = y
            k += 1

        cv2.circle(sdio, (x, y), 8, (255, 0, 0), -1)
        print(x, y)


#st = src + " " + dest

start_time = time.time()

cmd = "dijk_path.cpp"
#cap = cv2.VideoCapture('p_vid.mp4')

#while(True):
frame = cv2.imread('finaldrawr.jpg')
#cv2.imwrite('save.jpg', frame)
cv2.circle(frame, (0, 0), 5, (0, 0, 255), -1)
cv2.circle(frame, (583, 0), 5, (0, 0, 255), -1)
cv2.circle(frame, (583, 410), 5, (0, 0, 255), -1)
cv2.circle(frame, (0, 410), 5, (0, 0, 255), -1)

pts1 = np.float32([[0, 0], [583, 0], [583, 410], [0, 410]])
pts2 = np.float32([[0, 0], [300, 0], [300, 300], [0, 300]])
matrix = cv2.getPerspectiveTransform(pts1, pts2)

result = cv2.warpPerspective(frame, matrix, (300, 300))
sdio = result
cv2.namedWindow('image2')
#frame = cv.imread('finaldrawr.jpg')
cv2.setMouseCallback('image2', draw_circle)

while(True):
    cv2.imshow('image2', sdio)
    if cv2.waitKey(20) & 0xFF == 27:
        break

print(sx, sy, ex, ey)
src = ((sy//10)*30) + (sx//10)
dest = ((ey//10)*30) + (ex//10)
st = str(src)+" " + str(dest)
print(st)
##cv2.imshow('Original', frame)
#cv2.imshow('pers', result)
frame = result
hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

lower_red = np.array([30, 150, 50])
upper_red = np.array([255, 255, 180])

mask = cv2.inRange(hsv, lower_red, upper_red)
res = cv2.bitwise_and(frame, frame, mask=mask)

edges = cv2.Canny(frame, 200, 200)
##cv2.imshow('Edges', edges)
#temp = cv2.resize(edges, (300,300))
#temp1 = temp
##cv2.imshow('temp', temp)
kernel = np.ones((20, 20), np.float32)/225
smoothed = cv2.filter2D(edges, -1, kernel)
#cv2.imshow('smoothed', smoothed)
temp = smoothed
temp2 = np.empty((30, 30), dtype=int)
gs = 10
n = 30
for j in range(n):
    for i in range(n):
        img = temp[j*gs:(j+1)*gs, i*gs:(i+1)*gs]
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
            temp[j*gs:(j+1)*gs, i*gs:(i+1)*gs] = 0
            temp2[j, i] = 1
        else:
            temp[j * gs:(j + 1) * gs, i * gs:(i + 1) * gs] = 255
            temp2[j, i] = 0

##cv2.imshow('temp', temp)
g = open('grids.txt', 'w')
g.write(st + "\n")
np.savetxt("grids.txt", temp2)
g.close()


#cv2.imwrite('C:/Users/hp/Desktop/images/edge.jpeg', edges)
#n = 30
#gs = 10
# temp1 = temp
# for i in range(n):
#     for j in range(n):
#         img = temp[j*gs:(j+1)*gs, i*gs:(i+1)*gs]
#         data = np.asarray(img, dtype=int)
#         white = 0
#         black = 0
#         for x in range(gs):
#             for y in range(gs):
#                 if data[x, y] > 150:
#                     white += 1
#                 else:
#                     black += 1
#         if black > 90:
#             temp1[j*gs:(j+1)*gs, i*gs:(i+1)*gs] = 0
#         else:
#             temp1[j * gs:(j + 1) * gs, i * gs:(i + 1) * gs] = 255

##cv2.imshow("grid", temp)


# src = result[b*gs:(b+1)*gs, a*gs:(a+1)*gs]
# result[b * gs:(b+1) * gs, a * gs:(a+1) * gs, :] = [0, 255, 0]


#src[2] = 255
os.system("g++ dijk_path.cpp") #For Compiling
os.system("./a.out")

#cv2.imshow("result", result)
outF = open('myOutFile.txt', 'w')
theFile = open('srcdest.txt', 'r')
arr = []
for val in theFile.read().split():
    arr.append(int(val))
print(arr)
l = len(arr)
r = 0
s = 0
for i in range(l):
    v = arr[i]
    p = v//30
    q = v % 30
    result[p * gs:(p + 1) * gs, q * gs:(q + 1) * gs, :] = [0, 255, 0]
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
    s = q

theFile.close()
outF.close()

e_url = "192.168.0.112/specificArgs?direction="
f = open('myOutFile.txt', 'r')
a = " "
c = 1
for line in f:
    for d in line.split():
        if d != a and c == 1 and a == " ":
            e_url = e_url + d
        elif d != a:
            e_url = e_url + str(c) + "+"
            e_url = e_url + d
            c = 1
        elif d == a:
            c += 1
        a = d
e_url = e_url + str(c)
print(e_url)
http = urllib3.PoolManager()

response = http.request('GET', e_url)
soup = BeautifulSoup(response.data)
print(soup)

cv2.imshow('path', result)
#cv2.imshow('pathed', temp)

print("--- %s seconds ---" % (time.time() - start_time))
#time.sleep(12000)



while(True):
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
#cap.release()
cv2.destroyAllWindows()
