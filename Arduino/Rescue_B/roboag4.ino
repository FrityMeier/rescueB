

/* ************************************************
// Timer
*/
unsigned long startMillis;
unsigned long millisToWait;
void timerStart(unsigned long dauerInMillis){
  millisToWait = dauerInMillis;
  startMillis = millis();
}
boolean timerStopped(){
  if( millis() - startMillis >= millisToWait ){
    return true;
  }else{
    return false;
  }
}

/* ************************************************
// Servo
*/
#include <Servo.h> 
Servo myservo[4];
int servoPin[]={-1,-1,-1,-1};

void servoWrite(int pos, int pin){
  // welcher Servo haengt an dem Pin?
  for(int n=0; n<4; n++){
    if(servoPin[n] == pin){
      myservo[n].write(pos);
      return;
    }     
  }
  // er hat also keinen Servo gefunden
  // mal nach einem freien Slot suchen
  for(int n=0; n<4; n++){
    if(servoPin[n]<0){
      // freier Slot gefunden
      servoPin[n]=pin;
      myservo[n].attach(pin);
      myservo[n].write(pos);
      return;
    } 
  }
}


/* ////////////////////////////////////////////
// Motoren
*/

void motorLinksVor(int speed) { OnFwd(2, speed); }
void motorRechtsVor(int speed){ OnFwd(1, speed); }
void motorLinksZur(int speed) { OnRev(2, speed); }
void motorRechtsZur(int speed){ OnRev(1, speed); }

void fahrZur(int speed){  OnRev(3, speed); }
void fahrVor(int speed){  OnFwd(3, speed); }

void dreheLinks(int speed){
  OnFwd(1, speed);
  OnRev(2, speed);
}

void dreheRechts(int speed){
  OnFwd(2, speed);
  OnRev(1, speed);
}

void stop(){
  Off(3);
}



void OnFwd(int m, int v){
  // bei negativen Werten "OnRev" aufrufen
  if(v < 0) {
    OnRev(m, -v);
    return;
  }

  // PINS als Ausgaenge setzen
  DDRD |= B11110000;
  DDRB |= B00000011;

  // den Wert auf "100" deckeln
  if(v > 100) {
    v = 100;
  }
  v = map(v, 0, 100, 0, 255);

  // Motor A
  if(m==1 || m==3){
    digitalWrite(4, LOW); //vrA1
    digitalWrite(9, HIGH);//vrA2    
    analogWrite(5, v); //enA
  }

  // Motor B
  if(m==2 || m==3){
    digitalWrite(7, LOW);//vrB1
    digitalWrite(8, HIGH);//vrB2    
    analogWrite(6, v);
  }    
}

void OnRev(int m, int v)
{
  // bei negativen Werten "OnFwd" aufrufen
  if(v < 0) {
    OnFwd(m, -v);
    return;
  }

  // PINS als Ausgaenge setzen
  DDRD |= B11110000;
  DDRB |= B00000011;

  // den Wert auf "100" deckeln
  if(v > 100) {
    v = 100;
  }

  v = map(v, 0, 100, 0, 255);

  // Motor A
  if(m==1 || m==3){
    digitalWrite(4, HIGH);
    digitalWrite(9, LOW);    
    analogWrite(5, v);
  }

  // Motor B
  if(m==2 || m==3){
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);    
    analogWrite(6, v);
  }    
}


//schaltet den Motor aus. Die R�der "blockieren"
void Off(int m)  {
  // PINS als Ausgaenge setzen
  DDRD |= B11110000;
  DDRB |= B00000011;

  //Motor A
  if (m == 1  ||  m == 3) {
    digitalWrite(4, HIGH);
    digitalWrite(9, HIGH);
    analogWrite(5, 255);
  }

  //Motor B
  if (m==2 || m==3) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    analogWrite(6, 255);
  }
}

/* *************************************************************
// Ultraschallsensor
*/
// der US-Sensor liegt auf dem Pin
int usRead(int pin)
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration;
  int mm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(15);
  digitalWrite(pin, LOW);
  delayMicroseconds(20);
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH, 20000ul);
  mm = duration / 6; //microsecondsToCentimeters(duration);
  if(mm <= 0) mm = 999;
  //Serial.print(mm);
  //Serial.print(";");
  //Serial.print(pin);
  //Serial.println("u");
  return mm;
}


boolean buttonRead(int pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
  return digitalRead(pin); 
}


int ir30Read(int pin){
  int result;
  int val = analogRead(pin);
  if (val < 3){
    result = 999; // invalid value
  }else{
    // v = 24570.0/analogRead(0)+6.0;
    float ret = (24570.0 /((float)val))+6.0;// - 3.0)) - 4.0;
    if(ret > 300){
      result = 999;
    }
    else if(ret < 30)
    {
      result = 0;
    }
    else
    {
      result = ret;
    }
  }
  //Serial.print(result);
  //Serial.print(";");
  //Serial.print(pin);
  //Serial.println("i");
  return result;
} 


// Servomotoren
volatile long countRight;
volatile long countLeft;

void motorRightFwdSteps(long steps){
  countRight = steps;
  motorRechtsVor(90);
}

void motorLeftFwdSteps(long steps){
  countLeft = steps;
  motorLinksVor(90);
}

void fahrVorSteps(long steps)
{
  countLeft = steps;
  countRight = steps;
  OnFwd(3, 80);  
  while(countLeft>0 || countRight>0)
  {
    if(countLeft>countRight){
      OnFwd(2, 100);
      OnFwd(1, 60);
    }
    else if(countLeft < countRight)
    {
      OnFwd(1, 100);
      OnFwd(2, 60);
    }
    else
    {
      OnFwd(3, 80); 
    }
  }

}

void stepper_begin(){
  attachInterrupt(0, leftStepper, RISING);
  attachInterrupt(1, rightStepper, RISING);
}

void leftStepper(){
  countLeft--;
}

void rightStepper(){
  countRight--;
  if(countRight % 90 == 0) countRight--;
}
  
