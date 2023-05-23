void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
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
  Serial.println('r');
  char msg;
  bool condition = false;
  int intChar = 0;
  float pow10 =10;
  float value = 0;

  int degree=0;
  

  do{
   condition = false;
    do{//waits for there to be something in the serial port
      if (Serial.available()>0){
        condition = true;
      }
    }while (condition == false);
     msg = Serial.read();
      if(msg != 'f'){
        intChar = msg - '0';
        value += intChar*pow(pow10,degree);
        degree +=1;
      } 
  }while(msg != 'f');
  
    // say what you got:
    Serial.print("I recieved: ");
    Serial.println(value);
    timer(500); 
  }
