#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11

const char* ssid = "wifi name";
const char* password = "wifi password";


WiFiServer server(80);


const int DHTPin = 2;

#define airquality  A0

 #define trigger  5
#define echo  4

DHT dht(DHTPin, DHTTYPE);


static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];
static char distance[7];
static char air[7];

void setup() {

  Serial.begin(115200);
  delay(10);

  dht.begin();
  
 pinMode(trigger,OUTPUT);
 pinMode(echo,INPUT);
 pinMode(airquality ,INPUT);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

 
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  

  Serial.println(WiFi.localIP());
}



void loop() {

  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            
 
 
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            float h = dht.readHumidity();
            // Read temperature as Celsius (the default)
            float t = dht.readTemperature();
            // Read temperature as Fahrenheit (isFahrenheit = true)
            float f = dht.readTemperature(true);
            // Check if any reads failed and exit early (to try again).
            if (isnan(h) || isnan(t) || isnan(f)) {
              Serial.println("Failed to read from DHT sensor!");
              strcpy(celsiusTemp,"Failed");
              strcpy(fahrenheitTemp, "Failed");
              strcpy(humidityTemp, "Failed");         
            }
           else{
              // Computes temperature values in Celsius + Fahrenheit and Humidity
              float hic = dht.computeHeatIndex(t, h, false);       
              dtostrf(hic, 6, 2, celsiusTemp);             
              float hif = dht.computeHeatIndex(f, h);
              dtostrf(hif, 6, 2, fahrenheitTemp);         
              dtostrf(h, 6, 2, humidityTemp);
              // You can delete the following Serial.print's, it's just for debugging purposes
             Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.print(" *F");
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Heat index: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.println(" *F");
            }
             long duration, distance;
             digitalWrite(trigger, LOW);  
             delayMicroseconds(2); 
  
             digitalWrite(trigger, HIGH);
             delayMicroseconds(10); 
  
              digitalWrite(trigger, LOW);
              duration = pulseIn(echo, HIGH);
              distance = (duration/2)/34.4;
  
              Serial.print(distance);

              int air = analogRead(airquality);
              Serial.print("air");
              Serial.println(air);
              if(air <= 900){
                Serial.print("FRESH AIR ");
              }
              if (air >=850){
                Serial.print("BAD AIR");
              }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature and humidity
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>ESP8266 - Temperature and Humidity</h1><h3>Temperature in Celsius: ");
            client.println(celsiusTemp);
            client.println("*C</h3><h3>Temperature in Fahrenheit: ");
            client.println(fahrenheitTemp);
            client.println("*F</h3><h3>Humidity: ");
            client.println(humidityTemp);
            client.println("%</h3><h3>");
            client.println("</h3><h3>distance: ");
            client.println(distance);
            client.println("cm</h3><h3>");
            client.println("</h3><h3>Air Quality: ");
            client.println(air);
            client.println("ppm</h3><h3>");
            if(air >=201){
             client.println("BAD AIR </h3><h3>");  
            }
            if(air>=101&& air <=200){
            client.println("MODERATELY AIR </h3><h3>");
            }
           if(air<=100){
            client.println("FRESH AIR </h3><h3>");}
            client.println("</body></html>");     
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
} 



 
