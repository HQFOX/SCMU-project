#define PotenciometrePin A0
#define MoistureSensorPin A1
#define LedPin 3 //Simulates water level
#define RedLedPin 4 //Warning water level
#define GreenLedPin 2 //Simulates pump on
#define BuzzerPin 12


float RT = 1000;
float R;
int moistureSensorValue = 0;
int potenciometreValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LedPin,OUTPUT);
  pinMode(RedLedPin,OUTPUT);
  pinMode(GreenLedPin, OUTPUT);

}

void loop() {
  
//potenciometre code  
  potenciometreValue = analogRead(PotenciometrePin);
  int percent = map(potenciometreValue, 0, 1023, 0, 100);

  int brightness = map(potenciometreValue, 1 , 1024, 1 , 255);

  float voltage = potenciometreValue * (5.0 / 1023.0);

  R = ( voltage * RT)/5.0;

//moisture sensor code
  moistureSensorValue = analogRead(MoistureSensorPin);
  

  if(percent < 10)
  {
     digitalWrite(RedLedPin, HIGH);
     tone(BuzzerPin, 450);
     delay(100);
     noTone(BuzzerPin);
    
  }
  else
  {
    digitalWrite(RedLedPin,LOW);
    noTone(BuzzerPin);
  }

  if(moistureSensorValue < 10 && percent > 10)
  {
    digitalWrite(GreenLedPin,HIGH);
    //check if moisture changes
  }
  else
  {
    digitalWrite(GreenLedPin, LOW);
  }

  analogWrite(LedPin, brightness);

  Serial.print("Potenciometre Reading " );
  Serial.print(potenciometreValue);
  Serial.print(", Voltage =");
  Serial.print(voltage);
  Serial.print("v, Resistance = ");
  Serial.print(R);
  Serial.print(" ohms ");
  Serial.print(", Percentage ");
  Serial.print(percent);
  Serial.println(" % ");
  Serial.print("Moisture Sensor Reading " );
  Serial.println(moistureSensorValue);
  delay(1000);
}
