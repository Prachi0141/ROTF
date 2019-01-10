volatile int counter = 0; //This variable will increase or decrease depending on the rotation of encoder
int A = 2, aPin=0;
int B = 3, bPin=1;
int Mode=0;
int initialPosition = 0, thresh = 0;
int ed, ePWM, md=8, mPWM=9;
int es, ms=150;
int bumpf=20, b1=3, bumpr=21, b2=2;
String str = "34";
int b;
void setup() {
  Serial.begin (9600);
  pinMode(ed,OUTPUT);
  pinMode(ePWM,OUTPUT);
  pinMode(md,OUTPUT);
  pinMode(mPWM,OUTPUT);
  
  pinMode(A, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(B, INPUT_PULLUP); // internalเป็น pullup input pin 3
  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(aPin, ai0, RISING);
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(bPin, ai1, RISING);
  attachInterrupt(b1, Bump, CHANGE);
  attachInterrupt(b2, Bump, CHANGE);
}

void Bump(){
  if(digitalRead(bumpf)==HIGH){
    b=0;
  }
  if(digitalRead(bumpr)==HIGH){
    b=0;
  }
  else b=-1;
  
}
  
void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(a)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(b)==LOW) {
  counter--;
  }else{
  counter++;
  }
}

int mod(int Val){
  if(Val>0) return Val;
  return -1*Val;
}

void Up(){
  initialPosition = counter;
  digitalWrite(ed,HIGH);
  analogWrite(ePWM, es);
  while(mod(counter-initialPosition)<thresh){
    
  }
  analogWrite(ePWM,0);
}

void Down(){
  initialPosition = counter;
  digitalWrite(ed,LOW);
  analogWrite(ePWM, es);
  while(mod(counter-initialPosition)<thresh){
    
  }
  analogWrite(ePWM,0);
}

void Forward(){
  digitalWrite(md, HIGH);
  analogWrite(mPWM, ms);
  delay(100);
  while(b==-1){
    
  }
  analogWrite(mPWM, 0);
}

void Backward(){
  digitalWrite(md, LOW);
  analogWrite(mPWM, ms);
  delay(100);
  while(b==-1){
    
  }
  analogWrite(mPWM, 0);
}
int k = 0;
 void loop() {
  Serial.println(digitalRead(20));
  Serial.println(digitalRead(21));
  Mode = int(str.charAt(k))-48;
  k++;
  switch(Mode){
    case 1:
      Up();
      break;
    case 3:
      Forward();
      break;
    case 2:
      Down();
      break;
    case 4:
      Backward();
      break;
    case 5:
      str="";
  }
}
