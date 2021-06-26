#include <WiFi.h>
#include <FirebaseESP32.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 1. Define the WiFi credentials */
//#define WIFI_SSID "MEO-CF1AA1"
//#define WIFI_PASSWORD "3E94131A26"
//#define WIFI_SSID "MEO-E3DAC7"
//#define WIFI_PASSWORD "073E7C38F1"
#define WIFI_SSID "POCOPHONE"
#define WIFI_PASSWORD "pocophone"


/* 2. Define the API Key */
#define API_KEY "AIzaSyBkXH06Hnv6J8zuHHEnSevoDAd3ioTrgR0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "plantmonitor-20eb1-default-rtdb.europe-west1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "hm.raposo@campus.fct.unl.pt"
#define USER_PASSWORD "medronhos123"


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int count = 0;

const int potenciometerPin = 34;
const int moistureSensorPin = 39;

const int redLed = 22 ;
const int greenLed = 23 ;

const int buzzerPin = 16;

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

int potenciometerValue;

float moistureSensorValue;

int waterLevelPercent;

int moisturePercent;

float voltage = 0;

bool automatic;

bool waterpump = false;

int moistureThreshold = 30;  //value in wich it is worth to send water

int waterLevelThreshold = 10; //value to only allow with manual turn on


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  
  pinMode(redLed,OUTPUT);
  pinMode(greenLed, OUTPUT);


    // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(buzzerPin, ledChannel);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(greenLed,LOW);

Serial.println("-----Read Sensor Data--------");
  potenciometerValue = analogRead(potenciometerPin);
  waterLevelPercent = map(potenciometerValue,0,4095, 0, 100);
  
  Serial.print("Potenciometer Value (Water Level) : ");
  Serial.println(potenciometerValue);

  moistureSensorValue = analogRead(moistureSensorPin);
  moisturePercent = map(moistureSensorValue,0,4095, 0,100);

  Serial.print("Water Level Percentage : ");
  Serial.println(waterLevelPercent);
  
  Serial.print( " Moisture Sensor Value ");
  Serial.println(moistureSensorValue);
  
  Serial.print("Moisture Percentage : ");
  Serial.println(moisturePercent);
  Serial.println("--------------------");

  if(waterLevelPercent < waterLevelThreshold)
  {
    digitalWrite(redLed,HIGH);
    beep(3);
  }
  else{
    digitalWrite(redLed,LOW);
  }
  delay(5000);

  
 // if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  if (Firebase.ready())
  {
    sendDataPrevMillis = millis();

    Serial.printf("Setting Sensor Values in Firebase....");

    Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, "/plants/waterlevel", waterLevelPercent) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, "/plants/medronho1/moisture", moisturePercent) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Getting Values from Firebase....");

    Serial.printf("Get bool... %s\n", Firebase.getBool(fbdo, "/plants/medronho1/waterpump") ? String(fbdo.boolData()).c_str() : fbdo.errorReason().c_str());

    automatic = Firebase.getBool(fbdo, "/plants/medronho1/automaticwatering") ? fbdo.boolData() : false;

    waterpump = Firebase.getBool(fbdo, "/plants/medronho1/waterpump") ? fbdo.boolData() : false;
  }

    if(waterpump)
    {
         Serial.println("Manual Mode On....");

         if(waterLevelPercent > 10)
         {
            turnOnPump();
         }
         else
         {
             Serial.println("Water level low....");
         }
    }
    else
    {
         Serial.println("Manual Mode Off....");
    }

    if(automatic)
    {
         Serial.println("Automatic Mode On....");

         if(waterLevelPercent > 10 && moisturePercent < 30)
         {
            turnOnPump();
         }
         else
         {
             Serial.println("Water level low....");
         }
    }
    else
    {
         Serial.println("Automatic Mode Off....");
    }


  
}


void turnOnPump()
{
    digitalWrite(greenLed,HIGH); //turn on water
    Serial.println("Pump is turn On");
    
     if (Firebase.ready())
    {
        sendDataPrevMillis = millis();
        //Serial.printf("Set bool... %s\n", Firebase.setBool(fbdo, "/plants/medronho1/waterpump", true) ? "ok" : fbdo.errorReason().c_str());
        delay(6000);
        Serial.println("Pump is turned off");
        Serial.printf("Set bool... %s\n", Firebase.setBool(fbdo, "/plants/medronho1/waterpump", false) ? "ok" : fbdo.errorReason().c_str());
        digitalWrite(greenLed,LOW); //turn off water
    }
    else
    {
        delay(60000);
        Serial.println("Pump is turned off (else)");
        digitalWrite(greenLed,HIGH); //turn off water
      
    }
   



}

void beep( int numberOfTimes )
{
  for(int i = 0; i<3;i++)
  {

    ledcWrite(ledChannel, 200);
    delay(200);
    ledcWrite(ledChannel, 0);   
    delay(200);
  }

}
