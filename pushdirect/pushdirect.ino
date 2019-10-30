#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

const char* ssid = "";
const char* password = "";
const char* host = "api.pushbullet.com";
const int httpsPort = 443;
const char* PushBulletAPIKEY = ""; 
const char* PushBulletAPIKEY2 = "";

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "2C BC 06 10 0A E0 6E B0 9E 60 E5 96 BA 72 C5 63 93 23 54 B3"; //got it using https://www.grc.com/fingerprints.htm
int sensorValue;
WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(10);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
 }


void loop() {

   sensorValue = analogRead(A0);

   Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  

  Serial.print("Conectando a ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("Conexion fallida");
    return;
  }

   if (sensorValue > 800)

   {

     if (client.verify(fingerprint, host)) {
       Serial.println("Certificados ok");
       } 
       else 
       {
      Serial.println("Erro en certificados");
       }
     String url = "/v2/pushes";
     String messagebody = "{\"type\": \"note\", \"title\": \"PLANTA 1 BAJA HUMEDAD!\", \"body\": \""+String(sensorValue)+"\"}\r\n";
     Serial.print("requesting URL: ");
     Serial.println(url);

     client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + PushBulletAPIKEY + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " +
               String(messagebody.length()) + "\r\n\r\n");
    client.print(messagebody);

    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + PushBulletAPIKEY2 + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " +
               String(messagebody.length()) + "\r\n\r\n");
    client.print(messagebody);

    Serial.println("request enviado");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("HUMEDAD:");
    display.println("");
    display.println(sensorValue);
    display.display();

  //print the response

   while (client.available() == 0);

   while (client.available()) 
    {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    }

    int i;
    for (i=1; i <10800; i++) {

       delay(1000);
      }
  
   }
} 
