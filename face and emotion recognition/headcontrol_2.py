from RPi import GPIO
from time import sleep
import socket
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 3125
s.bind(('10.42.0.1', port))
s.listen(3)
print('socket is listening')

clk = 17
dt = 18
kd = 0
qmaxi = 18 # speed of neck rotation
qmini = 0   # encoders must be initially at this position ie origin
mp1 = 27 # motor 1 pwm
i1 = 22
i2 = 23
mp2 = 24 # servo pwm motor 2
calib = 25


GPIO.setmode(GPIO.BCM)
GPIO.setup(clk, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(dt, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(calib, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(mp1, GPIO.OUT)
GPIO.setup(i1, GPIO.OUT)
GPIO.setup(mp2, GPIO.OUT)
GPIO.setup(i2, GPIO.OUT)
p=GPIO.PWM(mp1, 100)
p.start(1) 
pwm=GPIO.PWM(mp2, 50)
pwm.start(1)

global clkLastState
global counter
global a
global b
global seek


counter = 0


def my_callback(channel): 
    clkState = GPIO.input(clk)
    dtState = GPIO.input(dt)
    global clkLastState
    global counter
    
    if dtState != clkState:
      counter += 1
    else:
      counter -= 1

GPIO.add_event_detect(17, GPIO.BOTH  , callback=my_callback, bouncetime=1)  # ye background me chalu h

def SetAngle(angle):
    
    duty = angle / 18 + 2
    GPIO.output(mp2, True)
    pwm.ChangeDutyCycle(duty)
    sleep(1)
    GPIO.output(mp2, False)
    pwm.ChangeDutyCycle(0)
    
def motor(x ,prerr):
        q = counter
        qx = x #mapping

        
        if  q >= qx:
          GPIO.output(i1, GPIO.LOW)
          GPIO.output(i2, GPIO.HIGH) #left ghumega
          err = abs( q - qx) / qmaxi - kd * (prerr - ( q - qx))
          p.ChangeDutyCycle(abs(err+10))
          sleep(0.1)
          p.ChangeDutyCycle(0)
          
          

        else:
          GPIO.output(i1, GPIO.HIGH)
          GPIO.output(i2, GPIO.LOW)     #right ghumega
          err = (abs(q - qx) / qmaxi)  -  kd * (prerr - ( q - qx))
          p.ChangeDutyCycle(abs(err+10))     #min duty=10
          sleep(0.1)
          p.ChangeDutyCycle(0)



while True: 

     seek = GPIO.input(calib)
     
     if seek==0:
       break
       
     if counter >= 400:
       break
       
     GPIO.output(i1, GPIO.HIGH)
     GPIO.output(i2, GPIO.LOW)
     p.ChangeDutyCycle(15)
     print(seek)

     
while True: 

     seek = GPIO.input(calib)
     
     if seek==0:
       break
       
       
     GPIO.output(i1, GPIO.LOW)
     GPIO.output(i2, GPIO.HIGH)
     p.ChangeDutyCycle(15)
     print(seek)

  
p.ChangeDutyCycle(0)

counter = 0

while True:
   c, addr = s.accept()
   print('Got connection from', addr)
   string = c.recv(1024)
   print(string)
   list_string = string.split(",")
   X = int(((list_string[0]*600)/640) -300)
   Y = int((list_string[1]*40)/480)
   emotion = str(list_string[2])
   c.close()
   i=0
   b =  X  # -300 to 300
   a =  Y  # 0 to 40
   
   prerr=b
           
   SetAngle(a)
   
   
   
   while i<5:

        zero = GPIO.input(calib)
        
        if zero == 0:
          counter = 0
                
        i=i+1
       
        motor(b ,prerr)
        
        prerr= abs(counter - b)

     
raw_input("Enter anything")


GPIO.cleanup()