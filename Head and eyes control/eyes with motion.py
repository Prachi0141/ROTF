from RPi import GPIO
from time import sleep

mp2 = 24  # servo pwm motor 2
GPIO.setmode(GPIO.BCM)

GPIO.setup(mp2, GPIO.OUT)
I1 = 20
I2 = 26
GPIO.setup(I1, GPIO.OUT)
GPIO.setup(I2, GPIO.OUT)


def SetAngle(angle):
   pwm=GPIO.PWM(mp2, 50)
   pwm.start(0)
   duty = angle / 18 + 2
   GPIO.output(mp2, True)
   pwm.ChangeDutyCycle(duty)
   sleep(1)


a = float(input("emotion is: "))
i=0
j=0
while j<=500000:
  if a == 1:  # sad
      GPIO.output(I1, GPIO.LOW)
      GPIO.output(I2, GPIO.LOW)
      
      
      while i < 1:
        i=i+1
        
        SetAngle(10)
        sleep(5)
      SetAngle(30)  # 0 of servo is at bottommost point
  
  
  elif a == 0:  # yes  
      
      
      while i < 1:
        i=i+1
        
        SetAngle(60)
    
        SetAngle(10)
    
        SetAngle(30)
  
  
  elif a == 2:  # confusion
      GPIO.output(I1, GPIO.HIGH)
      GPIO.output(I2, GPIO.LOW)
      
      while i < 1:
        i=i+1
        SetAngle(60)
        sleep(4)
        SetAngle(30)
      
  elif a == 3:  # suprise
      GPIO.output(I1, GPIO.LOW)
      GPIO.output(I2, GPIO.HIGH)
      
      
      while i < 1:
        
        i=i+1
        
        SetAngle(70)
    
        SetAngle(30)
  
  j = j + 1

while True:
  GPIO.output(b1, GPIO.HIGH)
  GPIO.output(b2, GPIO.HIGH)

GPIO.cleanup()
