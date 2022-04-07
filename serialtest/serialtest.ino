//#include <ArduinoJson.h>
//int numLandmarksn = 33;
//int numVariables= 4;
//int numDigitsperVar = 5;
////int positions[33][4];
//int positions[2][3];

int pos;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(14400);
  Serial.setTimeout(1);
  pinMode(4, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(4) == HIGH){
    Serial.println(1);
    delay(50);
  }
  while(!Serial.available()){}

  while(Serial.available()>0){
    pos = Serial.readString().toInt();
    Serial.println("recieved");
  }
}

//arduino asks for data
//python sends data
//ends

//ard asks for dat
//py recieves message and sends data
//arduino confirms message recieved and sends back to python
//python prints data
