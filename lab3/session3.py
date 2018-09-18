from Adafruit_IO import Client
from math import*

aio = Client('CHANGEME', 'CHANGEME')


Fs=8000
f=500
sample=16
a=[0]*sample
for n in range(sample):
	a[n]=sin(2*pi*f*n/Fs)
	aio.send('sinewave', a[n])
