#include <SoftwareSerial.h>
SoftwareSerial Serial1(9,10);//rx,tx

int mld=13, mrd=6, mfd=11;
int ml=12, mr=5, mf=10;
int PWMl=140, PWMr=140, PWMf=150;
int PWM=120;
int sf=0,sr=0,sl=0;
int kp=19, kd=12;
int Error=0, dError=0, pError=0, correction=0;
int Rotations=600;
int n1 = 4;
int vpr=0, vpf=0;
String str="";
char ch="";
int n=0;
int initialPosition = 0;
int value=initialPosition;
int k=0;
const int PinAF = 2;

// Used for reading DT signal
const int PinBF = 3;
const int PinAR = 21, PinBR = 20;
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
  }else{
  vpf--;
  }
  }
   
  void ai1F() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(PinBF)==LOW) {
  vpf--;
  }else{
  vpf++;
  }
  }

void ai0R() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(PinAR)==LOW) {
  vpr++;
  }else{
  vpr--;
  }
  }
   
void ai1R() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(PinBR)==LOW) {
  vpr--;
  }else{
  vpr++;
  }
}

void setup() {
  // put your setup code here, to run once:
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
  
  // Switch is floating so use the in-built PULLUP so we don't need a resistor
  //pinMode(PinSW, INPUT_PULLUP);
  Serial.begin(9600);
  interrupts();
  // Attach the routine to service the interrupts

  attachInterrupt(1,ai0F,RISING);
  attachInterrupt(0,ai1F,RISING);
  attachInterrupt(3,ai0R,RISING);
  attachInterrupt(2,ai1R,RISING);
  Serial1.begin(9600);
  while(!Serial1.available()){
    Serial.println("reading........");
  }
  while(Serial1.available()){
    char ch1 = Serial1.read();
    str = str+ch1;
  }
  Serial.println(str);
}

int mod(int value){
  if(value<0){
    return -1*value;
  }
  return value;
}

void Forward(){
  digitalWrite(mld,HIGH);
  digitalWrite(mrd,HIGH);
//  sr = 0;
//  sl = 0;
//  sf = 0;
//  while(sr<PWM && sl<PWM){
//    sr = map(mod(value-initialPosition),0,Rotations*n1/2,20,PWM);
//    sl = map(mod(value-initialPosition),0,Rotations*n1/2,20,PWM);
//    analogWrite(ml,sl);
//    analogWrite(mr,sr);
//    analogWrite(mf,sf);
//    value = vpr;
//    delay(10);
//  }
  sl=PWMl;
  sr=PWMr;
  sf=0;
  analogWrite(ml,sl);
  analogWrite(mr,sr);
  analogWrite(mf,sf);
}

void Left(){
  digitalWrite(mld,HIGH);
  digitalWrite(mrd,LOW);
  digitalWrite(mfd,HIGH);
//  sr = 0;
//  sl = 0;
//  sf = 0;
//  while(sr<PWM && sl<PWM){
//    sr = map(mod(value-initialPosition),0,Rotations*n1/2,50,PWMr);
//    sl = map(mod(value-initialPosition),0,Rotations*n1/2,50,PWMl);
//    sf = map(mod(value-initialPosition),0,Rotations*n1/2,70,PWMf);
//    analogWrite(ml,sl);
//    analogWrite(mr,sr);
//    analogWrite(mf,sf);
//    value = vpf;
//    delay(10);
//  }
  sl=PWMl;
  sr=PWMr;
  sf=PWMf;
  analogWrite(ml,sl);
  analogWrite(mr,sr);
  analogWrite(mf,sf);
}

void Right(){
  digitalWrite(mld,LOW);
  digitalWrite(mrd,HIGH);
  digitalWrite(mfd,LOW);
//  sr = 0;
//  sl = 0;
//  sf = 0;
//  while(sr<PWM && sl<PWM){
//    sr = map(mod(value-initialPosition),0,Rotations*n1/2,50,PWMr);
//    sl = map(mod(value-initialPosition),0,Rotations*n1/2,50,PWMl);
//    sf = map(mod(value-initialPosition),0,Rotations*n1/2,70,PWMf);
//    analogWrite(ml,sl);
//    analogWrite(mr,sr);
//    analogWrite(mf,sf);
//    value = vpf;
//    delay(10);
//  }
  sl=PWMl;
  sr=PWMr;
  sf=PWMf;
  analogWrite(ml,sl);
  analogWrite(mr,sr);
  analogWrite(mf,sf);
}

void Backward(){
  digitalWrite(mld,LOW);
  digitalWrite(mrd,LOW);
//  sr = 0;
//  sl = 0;
//  sf = 0;
//  while(sr<PWM && sl<PWM){
//    sr = map(mod(value-initialPosition),0,Rotations*n1/2,50,PWM);
//    sl = map(mod(value-initialPosition),0,Rotations*n1/2,50,PWM);
//    analogWrite(ml,sl);
//    analogWrite(mr,sr);
//    analogWrite(mf,sf);
//    value = vpr;
//    delay(10);
//  }
  sr=PWM;
  sl=PWM;
  sf = 0;
  analogWrite(ml,sl);
  analogWrite(mr,sr);
  analogWrite(mf,sf);
}

void Stop(){
//  initialPosition = vpr;
//  if(sf>0){
//    initialPosition = vpf; 
//  }
//  value = initialPosition;
//  while(sr>0 && sl>0 && sf>0){
//    sr = map(mod(value-initialPosition),0,Rotations*n1/2,PWMr,0);
//    sl = map(mod(value-initialPosition),0,Rotations*n1/2,PWMl,0);
//    if(sf>0){
//      sf = map(mod(value-initialPosition),0,Rotations*n1/2,PWMf,0);
//    }
//    analogWrite(ml,sl);
//    analogWrite(mr,sr);
//    analogWrite(mf,sf);
//    delay(10);
//    value = vpr;
//    if(sf>0){
//      value = vpf;
//    }
//  }
  sl=0;
  sr=0;
  sf=0;
  analogWrite(ml,sl);
  analogWrite(mr,sr);
  analogWrite(mf,sf);
}

void loop() {
  ch = str.charAt(k);
  k++;
  n = (int)str.charAt(k);
  n = n-48;
  k++;
  Serial.print(n);
  Serial.println(ch);
  //delay(1000);
  if(ch=='f'){
      initialPosition = vpr;
      value = initialPosition;
      Serial.println("Forward");
      while(mod(value-initialPosition)<Rotations*(n*n1)){
        Forward();
        delay(50);
        Serial.println(mod(value-initialPosition));
        value = vpr;
        //Serial.println(value);
      }
      Stop();
      delay(1000);
  }
      
  if(ch=='r'){
      Serial.println("right");
      initialPosition = vpf;
      value = initialPosition;
      while(mod(value-initialPosition)<(Rotations*(n*n1))){
        Right();
        delay(50);
        //Serial.println(mod(value-initialPosition));
        value = vpf;
      }
      Stop();
      delay(1000);
  }
      
  if(ch=='l'){
      Serial.println("Left");
      initialPosition = vpf;
      value = initialPosition;
      while(mod(value-initialPosition)<Rotations*(n*n1)){
        Left();
        delay(50);
        Serial.println(mod(value-initialPosition));
        value = vpf;
        
      }
      Stop();
      delay(1000);
  }
  if(ch=='b'){
      Serial.println("back");
      initialPosition = vpr;
      value = initialPosition;
      while(mod(value-initialPosition)<Rotations*(n*n1)){
        Backward();
        delay(50);
        Serial.println(mod(value-initialPosition));
        value = vpr;
        Serial.println(value);
      }
      Stop();
      delay(1000);
  }

  if(ch=='e'){
      Stop();
  }
}
