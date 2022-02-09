#include <ThingSpeak.h>
#include <DHT.h>                                    // Including library for dht
#include <ESP8266WiFi.h>
#include <HCSR04.h>

long unsigned int myChannelNumber = 1647926;
const char *apiKey = "B8FKALA5IXQYUZ4N";            //  Enter your Write API key from ThingSpeak              
const char *ssid =  "Samsung_A50_nm";               // replace with your wifi ssid and wpa2 key
const char *pass =  "99999999";
const char *server = "api.thingspeak.com";

uint8_t LED = D4;
uint8_t trigPin = D5; 
uint8_t echoPin = D6; 

long duration;
int distance;

#define DHTTYPE DHT11 
// #define DHTPIN 1                                //pin where the dht11 is connected 
uint8_t DHTPin = D1; 
DHT dht(DHTPin, DHTTYPE);

WiFiClient  client;

void setup(void) 
{
      
      
     
      delay(100);
      pinMode(DHTPin, INPUT);
      
      pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
      pinMode(echoPin, INPUT); // Sets the echoPin as an Input
      pinMode(LED, OUTPUT);
      Serial.begin(115200);
      dht.begin();
    
      Serial.println("Connecting to ");
      Serial.println(ssid);


      WiFi.begin(ssid, pass);

      while (WiFi.status() != WL_CONNECTED) 
      {
            delay(500);
            Serial.print(".");
      }
      
      Serial.println("");
      Serial.println("WiFi connected\n");
      
      ThingSpeak.begin(client);                    // Initialize ThingSpeak

}



 
void loop() 
{
//   if ((millis() - lastTime) > timerDelay) {
      float hum = dht.readHumidity();
      
      float tem = dht.readTemperature();
      // delay(2000);
      
              
       
      //Print temp and humidity values to serial monitor
      Serial.print("Humidity: ");
      Serial.print(hum);
      Serial.print(" %, Temp: ");
      Serial.print(tem);
      Serial.println(" Celsius");
//      delay(1000);

      long duration, inches, cm;
    
      pinMode(trigPin, OUTPUT);
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      pinMode(echoPin, INPUT);
  
      duration = pulseIn(echoPin, HIGH);
      inches = microsecondsToInches(duration);
      cm = microsecondsToCentimeters(duration);
      Serial.print("Nutrient level: ");
      Serial.print(inches);
      Serial.print("in, ");
      Serial.print(cm);
      Serial.print("cm");
      Serial.print("\n");
    
      // delay(1000); //Delay 2 sec.
      
      if (inches >= 6) 
      { 
        pinMode(LED, OUTPUT);
        digitalWrite(LED, HIGH);
      }
      else 
      {
        pinMode(LED, OUTPUT);
        digitalWrite(LED, LOW);
      }
  
      // delay(1001);
      
      unsigned short cal;
      if(inches > 15){
        cal = 0;  
        }
      else{
        cal = 15 - inches;
        }
      
      // int x = ThingSpeak.setField(1, t);
      // int y = ThingSpeak.setField(2, h);
      Serial.println("...Updating on ThingSpeak...");
      int x = ThingSpeak.writeField(myChannelNumber, 1, tem, apiKey);       //Update in ThingSpeak
      delay(15001);
      int y = ThingSpeak.writeField(myChannelNumber, 2, hum, apiKey);       //Update in ThingSpeak
      delay(15001);
      int z = ThingSpeak.writeField(myChannelNumber, 3, cal, apiKey);       //Update in ThingSpeak
      delay(15001);
      
      if (x==200 && y ==200 && z==200){
        Serial.println("...Updated succesfully on ThingSpeak...\n");
        }
      else{
        Serial.println("Failed to update" + String(x) + String(y) + String(z));
        }
      
       delay(2000);
      
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
