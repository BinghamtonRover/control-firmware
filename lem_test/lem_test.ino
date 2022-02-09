void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);

}

void loop() {

  int cur1input = analogRead(A10);  // reads in input from all current sensors
  int cur2input = analogRead(A11);
  int cur3input = analogRead(A12);
  int maincurinput = analogRead(A13);

  float cur1Volt = cur1input * (5.0 / 122);   // conversion rate found for use with 15k ohm resistor
  float cur2Volt = cur2input * (5.0 / 122);
  float cur3Volt = cur3input * (5.0 / 122);
  float maincurVolt = maincurinput * (5.0 / 122);
  
  // print out the value you read:
  Serial.println(char(cur1Volt) + "OCUR1");
  Serial.println(char(cur2Volt) + "OCUR2");
  Serial.println(char(cur3Volt) + "OCUR3");
  Serial.println(char(maincurVolt) + "MAIN OCUR");
  delay(250); 
}
