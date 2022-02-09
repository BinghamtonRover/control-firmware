//basic code that will light up LEDs in the buttons when the corresponding button is pressed
//make sure to connect wires correctly 
//pins on buttons with +/- are used for LED, connect - to ground and + to pin on "EXTRA" connector
// bigger pins on buttons are for button, connect one to correct pin on button connector, other pin goes to ground

int LedRed = 6;
int LedYellow = 7;
int LedGreen = 8;
int LEDREDout = 21;    //second pin of extra connector
int LEDYELout = 22;    //third pin of extra connector
int LEDGRNout = 23;    //fourth pin of extra connector


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LedRed, INPUT_PULLUP);
  pinMode(LedYellow, INPUT_PULLUP);
  pinMode(LedGreen, INPUT_PULLUP);
  pinMode(LEDREDout, OUTPUT);
  pinMode(LEDYELout, OUTPUT);
  pinMode(LEDGRNout, OUTPUT);
  
  digitalWrite(LEDREDout, LOW);
  digitalWrite(LEDYELout, LOW);
  digitalWrite(LEDGRNout, LOW);


}

void loop() {

  
  // put your main code here, to run repeatedly:
  int LedRedStatus = digitalRead(LedRed);
  int LedYellowStatus = digitalRead(LedYellow);
  int LedGreenStatus = digitalRead(LedGreen);

  if(LedRedStatus == 0){
    Serial.println("Red button pressed");
    digitalWrite(LEDYELout, LOW);
    digitalWrite(LEDGRNout, LOW);
    digitalWrite(LEDREDout, HIGH);
  }
  else{
      Serial.println(LedRedStatus);
  }

  if(LedYellowStatus == 0){
    Serial.println("Yellow button pressed");
    digitalWrite(LEDREDout, LOW);
    digitalWrite(LEDGRNout, LOW);
    digitalWrite(LEDYELout, HIGH);
  }
  else{
      Serial.println(LedYellowStatus);
  }

  if(LedGreenStatus == 0){
    Serial.println("Green button pressed");
    digitalWrite(LEDYELout, LOW);
    digitalWrite(LEDREDout, LOW);
    digitalWrite(LEDGRNout, HIGH);
  }
  else{
      Serial.println(LedGreenStatus);
  }
 // Serial.println(LedRedStatus);
  //Serial.println(LedYellowStatus, DEC );
  //Serial.println(LedGreenStatus, DEC);

  delay(250);
  
}
