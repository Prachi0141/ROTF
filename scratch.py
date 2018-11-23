import cv2
import numpy as np
import os
import subprocess

import time

src = int(input("Enter a number: "))
dest = int(input("Enter a number: "))
#st = src + " " + dest

start_time = time.time()
st = str(src)+" " + str(dest)
print(st)
cmd = "dijk_path.cpp"
#cap = cv2.VideoCapture('p_vid.mp4')

#while(True):
frame = cv2.imread('finaldrawr.jpg')
cv2.imwrite('save.jpg', frame)
cv2.circle(frame, (0, 0), 5, (0, 0, 255), -1)
cv2.circle(frame, (583, 0), 5, (0, 0, 255), -1)
cv2.circle(frame, (583, 410), 5, (0, 0, 255), -1)
cv2.circle(frame, (0, 410), 5, (0, 0, 255), -1)

pts1 = np.float32([[0, 0], [583, 0], [583, 410], [0, 410]])
pts2 = np.float32([[0, 0], [300, 0], [300, 300], [0, 300]])
matrix = cv2.getPerspectiveTransform(pts1, pts2)

result = cv2.warpPerspective(frame, matrix, (300, 300))
cv2.imshow('Original', frame)
cv2.imshow('pers', result)
frame = result
hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

lower_red = np.array([30, 150, 50])
upper_red = np.array([255, 255, 180])

mask = cv2.inRange(hsv, lower_red, upper_red)
res = cv2.bitwise_and(frame, frame, mask=mask)

edges = cv2.Canny(frame, 200, 200)
cv2.imshow('Edges', edges)
#temp = cv2.resize(edges, (300,300))
#temp1 = temp
#cv2.imshow('temp', temp)
kernel = np.ones((25, 25), np.float32)/225
smoothed = cv2.filter2D(edges, -1, kernel)
cv2.imshow('smoothed', smoothed)
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

cv2.imshow('temp', temp)
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

#cv2.imshow("grid", temp)


# src = result[b*gs:(b+1)*gs, a*gs:(a+1)*gs]
# result[b * gs:(b+1) * gs, a * gs:(a+1) * gs, :] = [0, 255, 0]


#src[2] = 255
os.system("g++ dijk_path.cpp") #For Compiling
os.system("./a.out")

cv2.imshow("result", result)
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
    print("l ")
    temp[p * gs:(p + 1) * gs, q * gs:(q + 1) * gs] = 150

    if p == r+1:
        print("Right")
        outF.write('Right\n')

    if q == s-1:
        outF.write('Backward\n')
        print("Backward")

    if q == s+1:
        outF.write('Forward\n')
        print("Forward")

    if p == r-1:
        outF.write('Left\n')
        print("Left")

    print(p, q)
    print(r, s)
    r = p
    s = q

theFile.close()
outF.close()
cv2.imshow('path', result)
cv2.imshow('pathed', temp)
print("--- %s seconds ---" % (time.time() - start_time))

while(True):
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

#cap.release()
cv2.destroyAllWindows()
