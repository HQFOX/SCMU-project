int buzzer = 12;



void setup() {
  // put your setup code here, to run once:
  //pinMode(buzzer,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  tone(buzzer, 450);
  delay(1000);
  noTone(buzzer);
  delay(1000);

}
