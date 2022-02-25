const int tempSensor=A0;

void setup() {
  // put your setup code here, to run once:

  pinMode(tempSensor, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    float temp_reading=analogRead(tempSensor);    // reads in sensor data
    float interim = temp_reading * (3.3/1023.0) *1000; //converts from teensy input to millivolts
    float temperature= (5.506-sqrt(sq(-5.506)+4*.00176*(870.6-interim)))/(2*(-.00176)) +30;  // formula given by TI to calculate temperature from millivolts reading over entire temperature range
   
    Serial.print("Temperature in C: ");
    Serial.println(temperature);

    delay(500); // prints every half second

}
