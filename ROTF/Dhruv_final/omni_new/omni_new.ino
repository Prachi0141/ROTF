//#include <SoftwareSerial.h>
//SoftwareSerial Serial1(8,9);//rx,tx
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free  sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

int Delay = 1000;
int mld=7, mrd=34, mfd=35;
int ml=6, mr=12, mf=11;
int pi_in=47, pi_out=48, p_in=50, p_out=8;
float PWMl=140, PWMr=140, PWMf=150;
//int PWM=160;
int sf=0,sr=0,sl=0;
float kpy=5, kdy=2.3, kpx=3.5, kdx=1.8;
float Error=0, dError=0, pError=0, correction=0;
int Rotations=400;
int n1 = 3;
volatile long vpr=0, vpf=0;
String path="";
String retrace="";
char ch="";
int n=0;
int time1 = 0;
int initialPosition = 0;
int angle = 0, base = 0;
int value=initialPosition;
int k=0;
int Mode=0;
const int PinBF = 3;
// Used for reading DT signal
const int PinAF = 2;
const int PinAR = 18, PinBR = 19;
// Used for the push button switch
//const int PinSW = 8;

// Keep track of last rotary value
//long Count = 0;

// ------------------------------------------------------------------
// INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT
// ------------------------------------------------------------------

void ai0F() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(PinAF)==LOW) {
    vpf++;
  }
  else{
    vpf--;
  }
}
   
  void ai1F() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(PinBF)==LOW) {
    vpf--;
  }
  else{
    vpf++;
  }
}

void ai0R() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(PinAR)==LOW) {
    vpr++;
  }
  else{
    vpr--;
  }
}
   
void ai1R() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(PinBR)==LOW) {
    vpr--;
  }
  else{
    vpr++;
  }
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
  while (!Serial); 
   Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
//    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
//    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
//    while (Serial.available() && Serial.read()); // empty buffer
//    while (!Serial.available());                 // wait for data
//    while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        //attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        //dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

  pinMode(p_in, INPUT);
  pinMode(p_out, OUTPUT);
  pinMode(pi_in, INPUT);
  pinMode(pi_out, OUTPUT);
  pinMode(mld, OUTPUT);
  pinMode(mrd, OUTPUT);
  pinMode(mfd, OUTPUT);
  pinMode(ml, OUTPUT);
  pinMode(mr, OUTPUT);
  pinMode(mf, OUTPUT);
  digitalWrite(p_out, HIGH);
  digitalWrite(pi_out, LOW);

  pinMode(PinAF, INPUT_PULLUP);
  pinMode(PinBF, INPUT_PULLUP);
  pinMode(PinAR, INPUT_PULLUP);
  pinMode(PinBR, INPUT_PULLUP);

  attachInterrupt(1,ai0F,RISING);
  attachInterrupt(0,ai1F,RISING);
  attachInterrupt(4,ai0R,RISING);
  attachInterrupt(5,ai1R,RISING);

  Serial2.begin(9600);
  while(!Serial2.available()){
    Serial.println("waiting");
  }
  while(Serial2.available()){
    char ch1 = Serial2.read();
   Serial.print(ch1);
    path = path+ch1;
  }

  Serial3.begin(9600);
  
  Serial.println(path);

  int t = millis();
  while(millis()-t<20000){
    Err();
    Serial.println(millis()-t);
    Serial.print("angle: ");
    Serial.println(angle);
  }
  base = angle;
  
}

void Err(){
  mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            Serial.print("quat\t");
            Serial.print(q.w);
            Serial.print("\t");
            Serial.print(q.x);
            Serial.print("\t");
            Serial.print(q.y);
            Serial.print("\t");
            Serial.println(q.z);
        #endif

        #ifdef OUTPUT_READABLE_EULER
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
//            Serial.print("euler\t");
//            Serial.print(euler[0] * 180/M_PI);
//            Serial.print("\t");
//            Serial.print(euler[1] * 180/M_PI);
//            Serial.print("\t");
//            Serial.println(euler[2] * 180/M_PI);
        #endif

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
//            Serial.print("ypr\t");
//            Serial.print(ypr[0] * 180/M_PI);
//            Serial.print("\t");
//            Serial.print(ypr[1] * 180/M_PI);
//            Serial.print("\t");
//            Serial.println(ypr[2] * 180/M_PI);
        #endif
        angle = ypr[0]*180/M_PI;
    }   
}



int mod(int value){
  if(value<0){
    return -1*value;
  }
  return value;
}

void Forward(){
  digitalWrite(mld,HIGH);
  digitalWrite(mrd,LOW);
//  sr = 0;
//  sl = 0;
//  sf = 0;
//  while(sr<PWMr && sl<PWMl){
//    sr += 20;
//    sl += 20;
//    //sf += 0;
//    analogWrite(ml,sl);
//    analogWrite(mr,sr);
//    analogWrite(mf,sf); 
//    delay(Delay);
//  }

  Err();
  Error = angle-base;
  dError = Error - pError;
  correction = kpy*pError + kdy*dError;
  Serial.print("correction=");
  Serial.println(correction);
  sl=PWMl+correction+15;
  sr=PWMr-correction;
  sf=0;
  analogWrite(mf,mod(sf));
  analogWrite(ml,mod(sl));
  analogWrite(mr,mod(sr));
  Serial.print(sl);
  Serial.print(" ");
  Serial.print(sr);
  Serial.print(" ");
  Serial.println(sf);
  //value = vpf;
}

void Left(){
  digitalWrite(mld,HIGH);
  digitalWrite(mrd,HIGH);
  
  digitalWrite(mfd,LOW);
//  sr = 0;
//  sl = 0;
//  sf = 0;
//  while(sr<PWMr && sl<PWMl){
//    sr += 20;
//    sl += 20;
//    sf += 20;
//    analogWrite(ml,sl);
//    analogWrite(mr,sr);
//    analogWrite(mf,sf); 
//    delay(Delay);
//  }

  Err();
  Error = angle-base;
  dError = Error - pError;
  correction = kpx*pError + kdx*dError;
  Serial.print("Error=");
  Serial.println(Error);
  sl = PWMl+correction-40;
  sr = PWMr+correction-40;
  sf = PWMf-correction-40;
  pError = Error;

  Serial.print(sl);
  Serial.print(" ");
  Serial.print(sr);
  Serial.print(" ");
  Serial.println(sf);
  analogWrite(mf,mod(sf));
  analogWrite(ml,mod(sl));
  analogWrite(mr,mod(sr));
  //value = vpf;
}

void Right(){
  digitalWrite(mld,LOW);
  digitalWrite(mrd,LOW);
  digitalWrite(mfd,HIGH);
//  sr = 0;
//  sl = 0;
//  sf = 0;
//  while(sr<PWMr && sl<PWMl){
//    sr += 20;
//    sl += 20;
//    sf += 20;
//    analogWrite(ml,sl);
//    analogWrite(mf,sf);
//    analogWrite(mr,sr);
//     
//    delay(Delay);
//  }

  Err();
  Error = angle-base;
  dError = Error - pError;
  correction = kpx*pError + kdx*dError;
  
  sl = PWMl-correction-40;
  sr = PWMr-correction-40;
  sf = PWMf+correction-50;
  pError = Error;
  Serial.print("Error=");
  Serial.println(Error);
  Serial.print(sl);
  Serial.print(" ");
  Serial.print(sr);
  Serial.print(" ");
  Serial.println(sf);
  analogWrite(ml,mod(sl));
  analogWrite(mf,mod(sf));
  analogWrite(mr,mod(sr));
  
  //value = vpf;
}

void Backward(){
  digitalWrite(mld,LOW);
  digitalWrite(mrd,HIGH);
//  sr = 0;
//  sl = 0;
//  sf = 0;
//  while(sr<PWMr && sl<PWMl){
//    sr += 20;
//    sl += 20;
//    //sf += 10;
//    analogWrite(ml,sl);
//    analogWrite(mf,sf);
//    analogWrite(mr,sr);
//    delay(Delay);
//  }
  Err();
  Error = angle-base;
  dError = Error - pError;
  correction = kpy*pError + kdy*dError;
  Serial.print("Error=");
  Serial.println(Error);
  sr=PWMr+correction;
  sl=PWMl-correction+20;
  sf = 0;
  Serial.print(sl);
  Serial.print(" ");
  Serial.print(sr);
  Serial.print(" ");
  Serial.println(sf);
  
  analogWrite(ml,mod(sl));
  analogWrite(mf,mod(sf));
  analogWrite(mr,mod(sr));
  
  //value = vpf;
}

void Stop(){
  sr = PWMr;
  sl = PWMl;
  
  while(sr>0 && sl>0){
    sr -= 20;
    sl -= 20;
    if(sf>0){
      sf -= 20;
    }
    analogWrite(ml,sl);
    analogWrite(mf,sf);
    analogWrite(mr,sr);    
    delay(50);
  }
  
  sl=0;
  sr=0;
  sf=0;
  analogWrite(mf,sf);
  analogWrite(ml,sl);
  analogWrite(mr,sr);
  delay(Delay);
}


void invert(String temp){
  int N = temp.length();
  for(int i=0; i<N/2; i++){
    char ch1 = temp[i];
    char ch2 = temp[N-i-1];
    temp[i] = ch2;
    temp[N-i-1] = ch1;
  }
  retrace = temp;
}


void turn(int val){
  if(val==0){
    
    digitalWrite(mld,HIGH); 
    digitalWrite(mrd,HIGH);
    digitalWrite(mfd,HIGH);
    sl = 100;
    sf = 120;
    sr = 100;
    analogWrite(ml, sl);
    analogWrite(mr, sr);
    analogWrite(mf, sf);
    while(mod(angle)>mod(base+4)){
      Err();
      Serial.println(angle-base);
    }
  }
  else if(val<0){
//    val = mod(val);
    digitalWrite(mld,HIGH);
    digitalWrite(mrd,HIGH);
    digitalWrite(mfd,HIGH);
    sl = 100;
    sf = 120;
    sr = 100;
    analogWrite(ml, sl);
    analogWrite(mr, sr);
    analogWrite(mf, sf);
    while(angle-base>val+4){
      Err();
      Serial.println(angle-base);
    }
  }
  else if(val>0){
    digitalWrite(mld,LOW);
    digitalWrite(mrd,LOW);
    digitalWrite(mfd,LOW);
    sl = 100;
    sf = 120;
    sr = 100;
    analogWrite(ml, sl);
    analogWrite(mr, sr);
    analogWrite(mf, sf);
    while(mod(angle-base)<val-4){
      Err();
      Serial.println(mod(angle-base));
    }
  }
  Stop();
//  delay(500);
  Err();
  Serial.println(angle-base);
}




void Retrace(){
  k=0;
  while(Mode==1 || Mode==3){
    if(retrace.charAt(k)==' '){
      k++;
    }
    ch = retrace.charAt(k);
    k++;
    n = (int)retrace.charAt(k);
    n = n-48;  
    k++;
    if((int)retrace.charAt(k)>=48 && (int)retrace.charAt(k)<=57){
      n = n*10 + (int)retrace.charAt(k)-48;
      k++;
    }
    
    if(ch=='F'){
      initialPosition = vpr;
      value = initialPosition;
      Serial.println("Forward");
      while(mod(value-initialPosition)<=Rotations*(n*n1)){
        Forward();
        //delay(30);
        Serial.println(mod(value-initialPosition));
        value = vpr;
        //Serial.println(value);
      }
      Stop();
      //delay(500);
    }
      
    if(ch=='R'){
      sf = PWMf;
      Serial.println("right");
      initialPosition = vpf;
      value = initialPosition;
      while(mod(value-initialPosition)<=Rotations*(n*n1)){
        Right();
        //delay(30);
        Serial.println(mod(value-initialPosition));
        value = vpf;
      }
      Stop();
      //delay(500);
    }
      
    if(ch=='L'){
      sf = PWMf;
      Serial.println("Left");
      initialPosition = vpf;
      value = initialPosition;
      while(mod(value-initialPosition)<=Rotations*(n*n1)){
        Left();
        //delay(30);
        Serial.println(mod(value-initialPosition));
        value = vpf;    
      }
      Stop();
      //delay(500);
    }
    if(ch=='B'){
      Serial.println("back");
      initialPosition = vpr;
      value = initialPosition;
      while(mod(value-initialPosition)<=Rotations*(n*n1)){
        Backward();
        //delay(30);
        Serial.println(mod(value-initialPosition));
        value = vpr;
        //Serial.println(value);
      }
      Stop();
//      delay(500);
    }

    if(ch=='e'){
      Stop();
      Mode++;
      retrace = "";
      k=0;
    }
  }
}


void loop() {
  
  if(path.charAt(k)==' '){
    k++;
  }
  ch = path.charAt(k);
  k++;
  n = (int)path.charAt(k);
  n = n-48;
  if((int)path.charAt(k+1)>=48 && (int)path.charAt(k+1)<=57){
    n = n*10 + (int)path.charAt(k+1)-48;
    k++; 
  }
  
  
  if(ch=='e'){
    invert(retrace);
    retrace = retrace + "e0";
  }
  
  else{
    if(n>9){
      if(ch=='F'){
        retrace = retrace + path.charAt(k) + path.charAt(k-1) + "B";
      }
      if(ch=='B'){
        retrace = retrace + path.charAt(k) + path.charAt(k-1) + "F";
      }
      if(ch=='R'){
        retrace = retrace + path.charAt(k) + path.charAt(k-1) + "L";
      }
      if(ch=='L'){
        retrace = retrace + path.charAt(k) + path.charAt(k-1) + "R";
      }
    }
    else{
      if(ch=='F'){
        retrace = retrace + path.charAt(k) + "B";
      }
      if(ch=='B'){
        retrace = retrace + path.charAt(k) + "F";
      }
      if(ch=='R'){
        retrace = retrace + path.charAt(k) + "L";
      }
      if(ch=='L'){
        retrace = retrace + path.charAt(k) + "R";
      }
    }
  }
  
  k++;
  
  //time1 = millis();
  //Serial.print(ch);
  //Serial.println(n);
  
    if(ch=='F'){
      initialPosition = vpr;
      value = initialPosition;
      Serial.println("Forward");
      while(mod(value-initialPosition)<=Rotations*(n*n1)){
        Forward();
        //delay(30);
        Serial.println(mod(value-initialPosition));
        value = vpr;
        //Serial.println(value);
      }
      Stop();
//      delay(500);
    }
      
    if(ch=='R'){
      sf = PWMf;
      Serial.println("right");
      initialPosition = vpf;
      value = initialPosition;
      while(mod(value-initialPosition)<=Rotations*(n*n1)){
        Right();
        //delay(30);
        Serial.println(mod(value-initialPosition));
        value = vpf;
      }
      Stop();
//      delay(500);
    }
      
    if(ch=='L'){
      sf = PWMf;
      Serial.println("Left");
      initialPosition = vpf;
      value = initialPosition;
      while(mod(value-initialPosition)<=Rotations*(n*n1)){
        Left();
        //delay(30);
        Serial.println(mod(value-initialPosition));
        value = vpf;    
      }
      Stop();
      delay(500);
    }
    if(ch=='B'){
      Serial.println("back");
      initialPosition = vpr;
      value = initialPosition;
      while(mod(value-initialPosition)<=Rotations*(n*n1)){
        Backward();
        //delMay(30);
        Serial.println(mod(value-initialPosition));
        value = vpr;
        //Serial.println(value);
      }
      Stop();
//      delay(500);
    }

    if(ch=='e'){
      Stop();
      //delay(Delay);
      switch(n){
        case 0:
          Serial.println("turn -90");
          turn(-90);
          if(Mode == 2){
            Serial3.println("a");
            while(!Serial3.available()){
              
            }
//            digitalWrite(p_out,LOW);
          }

          if(Mode == 0){
            digitalWrite(pi_out,HIGH);
            Serial.println("a");
            while(true){
              if(digitalRead(pi_in)==HIGH){
                Serial.println("c");
                break;  
              }
              
            }
            digitalWrite(pi_out,LOW);
            Serial.println("b");
          }
          
          turn(0);
          break;
        case 3:
          Serial.println("turn 0");
          
          if(Mode == 2){
            Serial3.println("a");
            while(!Serial3.available()){
              
            }
//            digitalWrite(p_out,LOW);
          }
//            digitalWrite(p_out,LOW);
          

          if(Mode == 0){
            digitalWrite(pi_out,HIGH);
            Serial.println("a");
            while(true){
              if(digitalRead(pi_in)==HIGH){
                Serial.println("c");
                break;  
              }
              
            }
            digitalWrite(pi_out,LOW);
            Serial.println("b");
          }
          break;
        case 2:
          Serial.println("turn 90");
          turn(90);
          if(Mode == 2){
            Serial3.println("a");
            while(!Serial3.available()){
              
            }
//            digitalWrite(p_out,LOW);
          }
          
          if(Mode == 0){
            digitalWrite(pi_out,HIGH);
            Serial.println("a");
            while(true){
              if(digitalRead(pi_in)==HIGH){
                Serial.println("c");
                break;  
              }
              
            }
            digitalWrite(pi_out,LOW);
            Serial.println("b");
          }
          turn(0);
          
          break;
        case 1:
          Serial.println("turn 180");
          turn(178);
          if(Mode == 2){
            Serial3.println("a");
            while(!Serial3.available()){
              
            }
//            digitalWrite(p_out,LOW);
          }

          if(Mode == 0){
            digitalWrite(pi_out,HIGH);
            Serial.println("a");
            while(true){
              if(digitalRead(pi_in)==HIGH){
                Serial.println("c");
                break;  
              }
              
            }
            digitalWrite(pi_out,LOW);
            Serial.println("b");
          }
          turn(0);
          break;
        
      }
      
      Serial.println(retrace);
      Mode++;
      Serial.print("Mode bret");
      Serial.println(Mode);
      Retrace();
      k=0;
      delay(Delay);
      if(Mode>3){
        path = "";
        retrace = "";
      }
    }
}
