import time
import cv2
import os
img = cv2.imread('test.png')
bwimg = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
cv2.imwrite('test_bw.png',bwimg)