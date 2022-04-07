void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);
  
}
String value; 
long currT = 0;
long prevT = 0;

void timer(int milis){
  currT = millis();
  prevT = currT;
  while (currT - prevT < milis){
    currT = millis();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("request");
  
  timer(100);
  
  if (Serial.available() > 0) {
    // read the incoming byte:
    value = Serial.readStringUntil("\n");
    value.toInt();

    // say what you got:
    Serial.print("I recieved: ");
    Serial.println(value);
  }

  timer(200);  
}
