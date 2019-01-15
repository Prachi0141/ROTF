volatile int counter = 0;
int bumpf = 20;//44
int bf =3;
int bumpr = 21;//43
int br =2;
int md = 8;//17
int mPWM = 9; //18
int ms = 100;
int b = 0;
int d=0;

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
    }
  if(digitalRead(bumpr)==HIGH){
    b = 0;
  }
 
  
}

void loop() {
  //digitalWrite(md, HIGH);
  //analogWrite(mPWM, ms);
      if(b==0 && d==0)
      {
        //analogWrite(mPWM, 0);
        digitalWrite(md,LOW);
        analogWrite(mPWM, ms);
        Serial.println("B");
        delay(2000);
           d=1; 
           b=1;
           
            
          
    
    }
   
    if(b==0 && d==1){
    
    
        //analogWrite(mPWM, 0);
        digitalWrite(md,HIGH);
        analogWrite(mPWM, ms);
        Serial.println("C");
        delay(2000);
        d=0;
        b=1;
        
    
    }

   
 }
