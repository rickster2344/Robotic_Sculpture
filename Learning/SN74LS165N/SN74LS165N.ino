//Define connections to SN74LS165N
//https://iamzxlee.wordpress.com/2014/05/13/74hc165-8-bit-parallel-inserial-out-shift-register/
//https://iamzxlee.wordpress.com/2014/02/28/arduino-piano/ 
//
//PL pin 1
int load = 7;
//C2 (used as clock enable) pin 15
int clockEnPin = 4;
//Q7 pin7 
int dataIn= 5;
//CP in 2
int clockIn = 6;


void setup() {
  // serial monitor setup
  Serial.begin(9600);

  //Connections setup
  pinMode(load, OUTPUT);
  pinMode(clockEnPin, OUTPUT);
  pinMode(clockIn, OUTPUT);
  pinMode(dataIn, INPUT);
}

void loop() {
  //write pulse to load pin (bring data off parallel bus and onto shift register) 
  digitalWrite(load, LOW);
  delayMicroseconds(5);
  digitalWrite(load, HIGH);
  delayMicroseconds(5);

  //Get data from chip
  digitalWrite(clockIn, HIGH);
  digitalWrite(clockEnPin, LOW);//Active low
  byte first = shiftIn(dataIn, clockIn, MSBFIRST); //get data, least significant bit first
  byte second = shiftIn(dataIn, clockIn, MSBFIRST);
  digitalWrite(clockEnPin, HIGH);//DeActivate

  //Pring to Serial monitor
  Serial.print("pin States:\r\n");
  Serial.print(first, BIN); //print incoming byte in binary
  Serial.print(", ");
  Serial.println(second, BIN);
  delay(300);
  
}
