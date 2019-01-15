volatile int counter = 0;
int bumpf = 20;//44
int bf =3;
int bumpr = 21;//43
int br =2;
int md = 8;//17
int mPWM = 9; //18
int ms = 100;
int b = -1;

void setup() {
  Serial.begin(9600);
  pinMode(bumpf, INPUT_PULLUP);
  pinMode(bumpr, INPUT_PULLUP);
  attachInterrupt(bf, Bump, CHANGE);
  attachInterrupt(br, Bump, CHANGE);
  pinMode(md, OUTPUT);
  pinMode(mPWM, OUTPUT);
  
  
  // put your setup code here, to run once:

}
void Bump(){
  if(digitalRead(bumpf)==HIGH){
    b = 0;
    Serial.println("Done");
  }
  if(digitalRead(bumpr)==HIGH){
    b = 0;
    Serial.println("Done");
  }
  else b = -1;
 Serial.println(b);
}

void loop() {
  digitalWrite(md, HIGH);
  analogWrite(mPWM, ms);
  delay(100);
  while(b==-1){
    
  }
  analogWrite(mPWM, 0);
  delay(500);
  digitalWrite(md, LOW);
  analogWrite(mPWM, ms);
  b = -1;
  while(b== -1){
    
  }
  analogWrite(mPWM, 0);
  delay(500);
  b = -1;


}
