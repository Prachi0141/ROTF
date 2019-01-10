//#include <SoftwareSerial.h>
//SoftwareSerial Serial1(8,9);//rx,tx
#include<MPU9250.h>


MPU9250 IMU;
long Time;

int16_t gyro[3] = {0, 0, 0};
int16_t acc[3] = {0, 0, 0};
float gyroBias[3]  = {0, 0, 0};
float accelBias[3] = {0, 0, 0};
int dt;


double x = 0.0, y = 0.0;
double cosine , sine;
double offset;
double angle = 0.00;


int Delay = 200;
int mld=7, mrd=12, mfd=10;
int ml=6, mr=13, mf=11;
float PWMl=140, PWMr=140, PWMf=150;
//int PWM=160;
int sf=0,sr=0,sl=0;
float kpy=25, kdy=12, kpx=10 ,kdx=4.5;
float Error=0, dError=0, pError=0, correction=0;
int Rotations=600;
int n1 = 2;
volatile long vpr=0, vpf=0;
String str="R3 F3 L3 B3e0";
char ch="";
int n=0;
int initialPosition = 0;
int value=initialPosition;
int k=0;
const int PinBF = 18;
// Used for reading DT signal
const int PinAF = 19;
const int PinAR = 2, PinBR = 3;
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
  pinMode(mld, OUTPUT);
  pinMode(mrd, OUTPUT);
  pinMode(mfd, OUTPUT);
  pinMode(ml, OUTPUT);
  pinMode(mr, OUTPUT);
  pinMode(mf, OUTPUT);

  pinMode(PinAF, INPUT_PULLUP);
  pinMode(PinBF, INPUT_PULLUP);
  pinMode(PinAR, INPUT_PULLUP);
  pinMode(PinBR, INPUT_PULLUP);

  attachInterrupt(5,ai0F,RISING);
  attachInterrupt(4,ai1F,RISING);
  attachInterrupt(1,ai0R,RISING);
  attachInterrupt(0,ai1R,RISING);
  
//  Serial1.begin(9600);
//  while(!Serial1.available()){
//    Serial.println("waiting");
//  }
//  while(Serial1.available()){
//    char ch1 = Serial1.read();
//   // Serial.print(ch1);
//    str = str+ch1;
//  }
//  Serial.println(str);
//  delay(500);

  IMU.calibrateMPU9250(gyroBias, accelBias);
//  Time = micros;
  
}

void Err(){
  IMU.readGyroData(gyro);
  dt = micros() - Time;
  angle = angle + (gyro[2]-gyroBias[2])*dt/131.0/1000000.0;
  Time = micros();
//  IMU.readAccelData(acc);
  Serial.print("angle= ");
  Serial.println(angle);
//delay(500);
//  Serial.print("offset= ");
//  Serial.println(offset);
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
  Time = micros();
  Err();
  Error = angle*10;
  dError = Error - pError;
  correction = kpy*pError + kdy*dError;
  Serial.print("correction=");
  Serial.println(correction);
  sl=PWMl+correction;
  sr=PWMr-correction;
  sf=0;
  analogWrite(mf,sf);
  analogWrite(ml,sl);
  analogWrite(mr,sr);
  
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
  Time = micros;
  Err();
  Error = angle*10;
  dError = Error - pError;
  correction = kpx*pError + kdx*dError;
  Serial.print("Error=");
  Serial.println(Error);
  sl = PWMl+correction-4;
  sr = PWMr+correction-40;
  sf = PWMf-correction-45;
  pError = Error;
  analogWrite(mf,sf);
  analogWrite(ml,sl);
  analogWrite(mr,sr);
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
  Time = micros();
  Err();
  Error = angle*10;
  dError = Error + pError;
  correction = kpx*pError + kdx*dError;
  
  sl = PWMl-correction-40;
  sr = PWMr-correction-40;
  sf = PWMf+correction-40;
  pError = Error;
  Serial.print("Error=");
  Serial.println(Error);
  analogWrite(ml,sl);
  analogWrite(mf,sf);
  analogWrite(mr,sr);
  
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
  Time = micros();
  Err();
  Error = angle*10;
  dError = Error + pError;
  correction = kpy*pError + kdy*dError;
  
  sr=PWMr+correction;
  sl=PWMl-correction;
  sf = 0;
  analogWrite(ml,sl);
  analogWrite(mf,sf);
  analogWrite(mr,sr);
  
  //value = vpf;
}

void Stop(){
//  while(sr>0 && sl>0 && sf>0){
//    sr -= 20;
//    sl -= 20;
//    if(sf>0){
//      sf -= 20;
//    }
//    analogWrite(ml,sl);
//    analogWrite(mf,sf);
//    analogWrite(mr,sr);    
//    delay(50);
//  }
  
  sl=0;
  sr=0;
  sf=0;
  analogWrite(mf,sf);
  analogWrite(ml,sl);
  analogWrite(mr,sr);
}

void loop() {
  
  if(str.charAt(k)==' '){
    k++;
  }
  ch = str.charAt(k);
  k++;
  n = (int)str.charAt(k);
  n = n-48;
  k++;
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
      delay(500);
  }
      
  if(ch=='R'){
      sf = PWMf;
      Serial.println("right");
      initialPosition = vpf;
      value = initialPosition;
      while(mod(value-initialPosition)<=(Rotations*(n*n1))){
        Right();
        //delay(30);
        Serial.println(mod(value-initialPosition));
        value = vpf;
      }
      Stop();
      delay(500);
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
        //delay(30);
        Serial.println(mod(value-initialPosition));
        value = vpr;
        //Serial.println(value);
      }
      Stop();
      delay(500);
  }

  if(ch=='e'){
      Stop();
      str="";
  }
}
