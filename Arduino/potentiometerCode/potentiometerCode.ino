float RT = 10000;
float R;

int pin = A0;
int ledPin = 3;

int potValue;
int percent;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  int potValue = analogRead(pin);
  percent = map(potValue, 0, 1023, 0, 100);

 int brightness = map(potValue, 1 , 1024, 1 , 255);

  float voltage = potValue * (5.0 / 1023.0);

  R = ( voltage * RT)/5.0;

  analogWrite(ledPin, brightness);

  Serial.print("Analog Reading " );
  Serial.print(potValue);
  Serial.print(", Voltage =");
  Serial.print(voltage);
  Serial.print("v, Resistance = ");
  Serial.print(R);
  Serial.print(" ohms ");
  Serial.print(", Percentage ");
  Serial.print(percent);
  Serial.println(" % ");
  delay(100);
  

}
