volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
int encoderPinB = 2;
int encoderPinA = 3;

void setup() {
  Serial.begin (9600);

  pinMode(encoderPinB, INPUT_PULLUP); // internal pullup input pin 2 
  
  pinMode(encoderPinA, INPUT_PULLUP); // internalเป็น pullup input pin 3
   //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nmbr 2 on most Arduino.
  attachInterrupt(0, ai0, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nmbr 3 on most Arduino.
  attachInterrupt(1, ai1, RISING);
  }
   
  void loop() {
  // Send the value of counter
  if( counter != temp ){
  Serial.println (counter);
  temp = counter;
  }
  }
   
  void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(encoderPinA)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(encoderPinB)==LOW) {
  counter--;
  }else{
  counter++;
  }
  }
