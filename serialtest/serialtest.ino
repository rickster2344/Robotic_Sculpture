#include <ArduinoJson.h>
int numLandmarksn = 33;
int numVariables= 4;
int numDigitsperVar = 5;
//int positions[33][4];
int positions[2][3];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(14400);
}

void loop() {
  // put your main code here, to run repeatedly:
    
  if(Serial.available()>0) {
    DynamicJsonDocument doc(2048);
    String blah = Serial.readStringUntil('\n');
  
    DeserializationError err = deserializeJson(doc, blah.c_str());
    if (err) {
      Serial.println(err.f_str());
      return;
    }

    double x15 = doc[15];
    Serial.println(x15);
  }
}
