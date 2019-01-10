from RPi import GPIO
from time import sleep

clk = 17
dt = 18
kd = 0
b = 150
mp1 = 27 # motor 1 pwm
i1 = 22
i2 = 23
qmaxi = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(clk, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(dt, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(i2, GPIO.OUT)
GPIO.setup(mp1, GPIO.OUT)
GPIO.setup(i1, GPIO.OUT)

p=GPIO.PWM(mp1, 100)
p.start(1) 



global clkLastState
global counter



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


def motor(x ,lastcount):
        q = counter
        qx = x #mapping

        if  q >= qx:
          GPIO.output(i1, GPIO.LOW)
          GPIO.output(i2, GPIO.HIGH) #left ghumega
          err = abs( q - qx) / qmaxi - kd * (prerr - ( q - qx))
          p.ChangeDutyCycle(abs(err+10))
          sleep(0.2)

        else:
          GPIO.output(i1, GPIO.HIGH)
          GPIO.output(i2, GPIO.LOW)     #right ghumega
          err = (abs(q - qx) / qmaxi)  -  kd * (prerr - ( q - qx))
          p.ChangeDutyCycle(abs(err+10))     #min duty=10
          sleep(0.2)


prerr=b
i=0
j=0
k=0
while i<3:
        
     
     
     i=i+1
        
     motor(350 ,prerr)
     
     prerr= abs(counter - b)
     
while j<3:
        
     
     
     j=j+1
        
     motor(-700 ,prerr)
     
     prerr= abs(counter - b)
     
while k<3:
        
     
     
     k=k+1
        
     motor(200 ,prerr)
     
     prerr= abs(counter - b)
     

GPIO.cleanup()