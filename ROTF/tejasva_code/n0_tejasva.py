from RPi import GPIO
from time import sleep
import RpiMotorLib
import subprocess
import socket
import sys
import os
gw = os.popen("ip -4 route show default").read().split()
t = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
t.connect((gw[2], 0))
ipaddr = t.getsockname()[0]
gateway = gw[2]
host = socket.gethostname()
print ("IP:", ipaddr, " GW:", gateway, " Host:", host)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 3125
s.bind((ipaddr, port)) #self IP address
s.listen(3)
print('socket is listening')
mp2 = 3 # servo pwm motor 2
calib = 2
GpioPins = [5, 6, 13, 19]
mymotortest = RpiMotorLib.BYJMotor("MyMotorOne", "Nema")
speed = 0.01
E1 = 20
E2 = 26
IN = 4
GPIO.setmode(GPIO.BCM)
GPIO.setup(calib, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(IN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(mp2, GPIO.OUT)
GPIO.setup(E1, GPIO.OUT)
GPIO.setup(E2, GPIO.OUT)
pwm=GPIO.PWM(mp2, 50)
pwm.start(1)
sleep(0.5)
global a
global b

def SetAngle(angle):
    
    duty = angle / 18 + 2
    GPIO.output(mp2, True)
    pwm.ChangeDutyCycle(duty)
    sleep(1)
    GPIO.output(mp2, False)
    pwm.ChangeDutyCycle(0)
   

def motor(x,y):
  steps = x #mapping
  
  
  if  x >= 0:
  
    mymotortest.motor_run(GpioPins , speed, abs(steps), False, False, "half", .5)
    SetAngle(y)
    
  else:
  
    mymotortest.motor_run(GpioPins , speed, abs(steps), True, False, "half", .5) # inputs. motor_run(GPIOPins, wait, steps, counterclockwise, verbose, steptype, initdelay)
    SetAngle(y)

SetAngle(25)
i=0
while i<10:
  seek = GPIO.input(calib)
  if seek == 0:
    break
  mymotortest.motor_run(GpioPins , speed, 1, False, False, "half", .005)
  
  i=i+1
sleep(0.1)
i=0
while i<20:  
  seek = GPIO.input(calib)
  if seek == 0:
    break
  mymotortest.motor_run(GpioPins , speed, 1, True, False, "half", .005)
  i=i+1
  
#range -8 to 8
k=0
bp = 0

while True:
  print('waiting')
  if GPIO.input(IN) == 1:
     while True:
       print(GPIO.input(IN))
       c, addr = s.accept()
       print('Got connection from', addr)
       string = c.recv(1024)
       print(string)
       list_string = string.split(",")
       X = int(((int(list_string[0])*14)/640) - 7)   # 7 steps to maximum 14 is full range ie 7*2,,,,, 640 * 480 reso
       Y = 40 - int((int(list_string[1])*30)/480 )
       emotion = str(list_string[2])
       
       c.close()
       i=0
       b =  X  # -7 to 7
       a =  Y  # 10 to 40
       print("b,a : ", b,",", a)
    
       if str(list_string[2]) == 'neutral':
         E = 0
       
         GPIO.output(E1, GPIO.HIGH)
         GPIO.output(E2, GPIO.HIGH)
         
         print(1)
         
       elif str(list_string[2]) == 'sad':
         E = 1
         GPIO.output(E1, GPIO.LOW)
         GPIO.output(E2, GPIO.LOW)
       
       elif str(list_string[2]) == 'happy':
         E = 3
         GPIO.output(E1, GPIO.HIGH)
         GPIO.output(E2, GPIO.LOW)
       
       elif str(list_string[2]) == 'confuse':
         E = 2
         GPIO.output(E1, GPIO.LOW)
         GPIO.output(E2, GPIO.HIGH)
    
       motor(b - bp, a)
       
  
        
       GPIO.output(E1, GPIO.HIGH)
       GPIO.output(E2, GPIO.HIGH)
       
       while k == 0:
         p=subprocess.Popen(["python3" , "chatbot.py"])
         k = k + 1
         
         
       bp = b
       
       if  GPIO.input(IN) == 0 :
          i=0
          while i<10:
            seek = GPIO.input(calib)
            if seek == 0:
              break
          
            mymotortest.motor_run(GpioPins , speed, 1, False, False, "half", .005)
          
            
            i=i+1
          
          sleep(0.1)
          
          i=0
          while i<20:  
            seek = GPIO.input(calib)
            if seek == 0:
              break
          
            mymotortest.motor_run(GpioPins , speed, 1, True, False, "half", .005)
          
            i=i+1
          
          
          break
       
    
     
     
raw_input("Enter anything")

GPIO.cleanup()