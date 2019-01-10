volatile int counter = 0; //This variable will increase or decrease depending on the rotation of encoder
int a = 2, aPin=0;
int b = 3, bPin=1;
int Mode=0;
int initialPosition = 0, thresh1 = 5800, thresh2 = 12500;
int ed=8, ePWM=7, md=6, mPWM=5;
int photo=48;
int es=180, ms=200;
int bumpf=21, bumpr=20;
int p_in=24, p_out=50;
String str = "";
int k =0;
//int b;
int Delay = 5000;
int cnt = 0;

void initz(){
while(!Serial3.available() || Serial3.read()!='a'){        
  }
  k=0;
  delay(1000);
  str = "24156341567";
  Serial.println("Loop Break");
  
}

void setup() {
  Serial.begin (9600);
  Serial3.begin(9600);
  pinMode(ed,OUTPUT);
  pinMode(ePWM,OUTPUT);
  pinMode(md,OUTPUT);
  pinMode(mPWM,OUTPUT);
  pinMode(photo, INPUT);
  pinMode(p_in, INPUT_PULLUP);
  pinMode(p_out, OUTPUT);
  
  digitalWrite(p_out, LOW);

  
  pinMode(a, INPUT_PULLUP); // internal pullup input pin 2 
  pinMode(b, INPUT_PULLUP); // internalเป็น pullup input pin 3
  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(aPin, ai0, RISING);
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(bPin, ai1, RISING);

}

void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(b)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(a)==LOW) {
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
//  initialPosition = counter;
  digitalWrite(ed,HIGH);
  analogWrite(ePWM, es);
  while(mod(counter-initialPosition)>40){
    Serial.println(counter);
  }
  analogWrite(ePWM,0);
}

void Down1(){
  initialPosition = counter;
  digitalWrite(ed,LOW);
  analogWrite(ePWM, es);
  while(mod(counter-initialPosition)<thresh1){
    Serial.println(counter);
  }
  analogWrite(ePWM,0);
}

void Down2(){
  initialPosition = counter;
  digitalWrite(ed,LOW);
  analogWrite(ePWM, es);
  while(mod(counter-initialPosition)<thresh2){
    Serial.println(counter);
  }
  analogWrite(ePWM,0);
}

void Forward1(){
  digitalWrite(md, HIGH);
  analogWrite(mPWM, ms);
  delay(100);
  while(digitalRead(photo)==LOW){
    
  }
  analogWrite(mPWM, 0);
}

void Forward2(){
  digitalWrite(md, HIGH);
  analogWrite(mPWM, ms);
  delay(1000);
  while(digitalRead(bumpf)==LOW){
    
  }
  analogWrite(mPWM, 0);
  delay(Delay);
}

void Backward(){
  digitalWrite(md, LOW);
  analogWrite(mPWM, ms);
  delay(100);
  while(digitalRead(bumpr)==LOW){
    
  }
  analogWrite(mPWM, 0);
}

void loop() {

if(cnt < 1)
{
  initz();  

  cnt++;
  }
//if(digitalRead(p_in)==1){
//  k=0;
//    str = "24156341567";
//    Serial.println(str);
//  }
  
  digitalWrite(p_out, LOW);
  Mode = int(str.charAt(k))-48;
  k++;
  switch(Mode){
    case 1:   
      Serial.println("up");
      Up();
      break;
    case 2:
      Serial.println("down1");
      Down1();
//      Serial.println("down2");
      break;
    case 3:
      Serial.println("down2");
      Down2();    
    case 4:
      Serial.println("Forward1");
      Forward1();
      break;
    case 5:
      Serial.println("Forward2");
      Forward2();
      break;
    case 6:
      Serial.println("Backward");
      Backward();
      break;
    case 7:
      str="";
      Serial3.println("a");
      break;  
  }
}
