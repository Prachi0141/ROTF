from RPi import GPIO
from time import sleep


b1 = 20
b2 = 26

GPIO.setmode(GPIO.BCM)



GPIO.setup(b1, GPIO.OUT)
GPIO.setup(b2, GPIO.OUT)


a = float(input("enter 1234"))
i=0
while i <= 500000:
    if a==1:   
       
      GPIO.output(b1, GPIO.LOW)
      GPIO.output(b2, GPIO.HIGH)

      
    elif a==2:
      GPIO.output(b1, GPIO.LOW)
      GPIO.output(b2, GPIO.LOW)
      
    elif a==3:
      GPIO.output(b1, GPIO.HIGH)
      GPIO.output(b2, GPIO.HIGH)
      
    elif a==4:
      GPIO.output(b1, GPIO.HIGH)
      GPIO.output(b2, GPIO.LOW)
    
    i = i +1
    print(i)

while True:
  GPIO.output(b1, GPIO.HIGH)
  GPIO.output(b2, GPIO.HIGH)
      
GPIO.cleanup()