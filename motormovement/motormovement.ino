// defines pins numbers
const int stepPin = 5; 
const int dirPin = 4; 
int timeDelay = 1;

unsigned long currenttime;
unsigned long starttime;
int state = 0;


void setup() {
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}
void loop() {
  if (state == 0){
    starttime = micros();
    digitalWrite(dirPin,HIGH);
//    Serial.println(state);
    state = 1;
  }
  
  if (state == 1) {
    currenttime = micros();
//    Serial.println(state);
    digitalWrite(stepPin,HIGH);
    Serial.println(currenttime- starttime);
    if (currenttime - starttime >= timeDelay){
      starttime = currenttime;
      state = 2;
    }
  }

  if (state == 2){
   currenttime = micros();
   digitalWrite(stepPin,LOW);
//   Serial.println(state);
   Serial.println(currenttime- starttime);
   if (currenttime - starttime >= timeDelay){
      starttime = currenttime;
      state = 1;
   }
  }
}
