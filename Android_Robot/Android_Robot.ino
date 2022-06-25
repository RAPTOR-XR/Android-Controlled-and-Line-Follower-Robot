#include <QTRSensors.h>
#define enB 5
#define enA 6
#define in1 10
#define in2 9
#define in3 8
#define in4 7

QTRSensors qtr;

const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];
boolean mode = LOW;
int sensorFlag = LOW;
const byte interruptPin = 2;
long unsigned int lastPress;
int debounceTime = 40;
boolean ledState = LOW;
unsigned int ledTime = 0;

void SensorCal(){
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5}, SensorCount);
  qtr.setEmitterPin(3);
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  for (uint16_t i = 0; i < 400; i++){
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(9600);
  for (uint8_t i = 0; i < SensorCount; i++){
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  for (uint8_t i = 0; i < SensorCount; i++){
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}

void SensorData(){
  uint16_t position = qtr.readLineBlack(sensorValues);
  for (uint8_t i = 0; i < SensorCount; i++){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);
  delay(250);
  if(ledTime>6){
    ledState=!ledState;
    ledTime=0;
    }
  digitalWrite(LED_BUILTIN, ledState);
  ledTime++;
}

void forward(){
  analogWrite(enA,255);
  analogWrite(enB,255);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void back(){
  analogWrite(enA,255);
  analogWrite(enB,255);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void right(){
  analogWrite(enA,255);
  analogWrite(enB,255);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void left(){
  analogWrite(enA,255);
  analogWrite(enB,255);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void off(){
  analogWrite(enA,0);
  analogWrite(enB,0);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void motorTest(){
  forward();
  Serial.println("Forward");
  delay(1000);
  back();
  Serial.println("Back");
  delay(1000);
  right();
  Serial.println("Right");
  delay(1000);
  left();
  Serial.println("Left");
  delay(1000);
  off();
  Serial.println("Stop");
  delay(1000);
}

void setup(){
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeMode, FALLING);
}

void loop(){
  if(mode==LOW){
    if(Serial.available()>0){
      int inbyte= Serial.read();
    
      switch(inbyte){
        case '0':
        Serial.println("Left");
        left();
        delay(1000);
        break;
        case '1':
        Serial.println("Right");
        right();
        delay(1000);
        break;
        case '2':
        Serial.println("Forward");
        forward();
        delay(1000);
        break;
        case '3':
        Serial.println("Back");
        back();
        delay(1000);
        break;
        case '4':
        Serial.println("Stop");
        off();
        default:
        break;      
      }
    }  
  }
  else if(mode==HIGH){
    digitalWrite(LED_BUILTIN, LOW);     
      if(sensorFlag==LOW){
      Serial.println("IR Test");
      SensorCal();
      sensorFlag=HIGH;
      }else{
        SensorData();
      }
  }
}

void changeMode(){
  if((millis()-lastPress) > debounceTime){
    lastPress = millis();
    if(digitalRead(interruptPin) == 0){
       mode=!mode;
    }
    else if(digitalRead(interruptPin) == 1){
      mode=mode;
    }  
 }
}
