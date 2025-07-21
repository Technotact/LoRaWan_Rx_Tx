#include <Wire.h> 
#include "DHT.h" 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#define DHTTYPE DHT22 
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1); 
unsigned long delayTime; 
uint8_t DHTPin = 6; 
DHT dht(DHTPin, DHTTYPE); 
float Temperature; 
float Humidity; 
float soilTemp; 
 
int vibration = 0; 
 
//int msgCount=0; 
 
#include <OneWire.h> 
#include <DallasTemperature.h> 
#define ONE_WIRE_BUS 5 
OneWire oneWire(ONE_WIRE_BUS);   
DallasTemperature sensors(&oneWire); 
int sm = A2; 
 
const int red = 4; 
const int green = 3; 
const int blue = 8; 
const int buzzer = 7; 
 
#include <SPI.h> 
#include <LoRa.h>  
 
const int AirValue = 6;    
const int WaterValue = 30;   
int value = 0; 
int soilmoisturepercent=0; 
 
  
void setup() { 
  Serial.begin(115200); 
  pinMode(DHTPin, INPUT); 
  pinMode(A2, INPUT); 
  pinMode(red,OUTPUT);
    pinMode(green,OUTPUT); 
  pinMode(blue,OUTPUT); 
  pinMode(buzzer,OUTPUT); 
  sensors.begin(); 
 
   
  while (!Serial)  
  Serial.println("LoRa Sender"); 
  if (!LoRa.begin(433E6)) {  
    Serial.println("Starting LoRa failed!"); 
    while (1); 
  } 
 
  dht.begin(); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.display(); 
  delay(100); 
  display.clearDisplay(); 
  display.display(); 
  display.setTextSize(1.75); 
  display.setTextColor(WHITE); 
 
  display.setCursor(0, 0); 
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setCursor(0, 0);
  display.print(" VERSATILE");
  display.setCursor(0, 17);
  display.print("LONG RANGE");
  display.setCursor(0, 34);
  display.print("     COMM.");
  display.setCursor(0, 51);
  display.print("    SYSTEM");
  display.display(); 
 
  delay(4000); 
 
  display.setCursor(0, 0); 
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setCursor(0, 18); 
  display.print("Booting"); 
  display.setCursor(0, 38); 
  display.print("Up ...."); 
  display.display(); 
 
  delay(3000); 
 
} 
  
void loop() { 
 
  int vibrationpercent = 0; 
  int rainfallpercent = 0;

  Humidity = dht.readHumidity(); 
  Temperature = dht.readTemperature(); 
 
  if (isnan(Humidity) || isnan(Temperature)) { 
    Serial.println(F("Failed to read from DHT sensor!")); 
    return; 
  } 
 
  display.setCursor(0, 0); 
  display.clearDisplay(); 
  display.setTextSize(1); 
  display.setCursor(0, 0); 
  display.print("Air Temperature: "); 
  display.setTextSize(2); 
  display.setCursor(0, 12); 
  display.print(Temperature); 
  display.print(" "); 
  display.setTextSize(1); 
  display.cp437(true); 
  display.write(167); 
  display.setTextSize(2); 
  display.print("C"); 
  display.setTextSize(1); 
  display.setCursor(0, 35); 
  display.print("Air Humidity : "); 
  display.setTextSize(2); 
  display.setCursor(0, 47); 
  display.print(Humidity); 
  display.print(" %"); 
  display.display(); 
  delay(1600); 
 
  sensors.requestTemperatures();  
  soilTemp=sensors.getTempCByIndex(0); 
  Serial.print("Temperature: "); 
  Serial.print(soilTemp); 
  Serial.print((char)176);//shows degrees character 
  Serial.print("C"); 
 
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setCursor(0, 0); 
  display.print("Soil Temp"); 
  display.setTextSize(3); 
  display.setCursor(5, 20); 
  display.print(soilTemp); 
  display.setCursor(102, 27); 
  display.setTextSize(2); 
  display.print("C"); 
  display.display(); 
  delay(1600); 
 
  if((soilTemp)>30) 
  { 
    /* 
    display.clearDisplay();   
    display.setTextSize(2); 
    display.setCursor(0, 20); 
    display.print("  HIGH   "); 
    display.setCursor(0, 40); 
    display.print("  TEMP... "); 
    display.display(); 
    delay(900);*/ 
    for(int i=1;i<=4;i++) 
    { 
      digitalWrite(red,HIGH); 
      digitalWrite(buzzer,HIGH); 
      delay(400); 
      digitalWrite(red,LOW); 
      digitalWrite(buzzer,LOW); 
      delay(400); 
 
    } 
 
  } 
   
 
  value = analogRead(sm); 
  Serial.println(value); 
  soilmoisturepercent = map(value, AirValue, WaterValue, 0, 100); 
  Serial.println(soilmoisturepercent); 
  if(soilmoisturepercent<1) 
  { 
    soilmoisturepercent=1; 
  } 
  Serial.print("SM:"+String(soilmoisturepercent)+"%");
    display.clearDisplay(); 
  display.setTextSize(2); 
  display.setCursor(0, 0); 
  display.print("Soil"); 
  display.setCursor(0, 17); 
  display.print("Moisture :"); 
  display.setTextSize(3); 
  display.setCursor(4, 35); 
  display.print(soilmoisturepercent); 
  display.setCursor(60, 40); 
  display.setTextSize(2); 
  display.print("%"); 
  display.display(); 
  delay(1600); 
 
  vibration=analogRead(A2); 
  Serial.print("Vibration : "); 
  Serial.println(value); 
  vibrationpercent = map(vibration, 50, 1024, 0, 100); 
  Serial.println(vibrationpercent); 
  if(vibrationpercent<1) 
  { 
    vibrationpercent=0; 
  } 
 
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setCursor(0, 0); 
  display.print("Vibration"); 
  display.setCursor(0, 17); 
  display.print("is :"); 
  display.setTextSize(3); 
  display.setCursor(4, 35); 
  display.print(vibrationpercent); 
  display.setCursor(60, 40); 
  display.setTextSize(2); 
  display.print("%"); 
  display.display(); 
  delay(1600); 
 
  if(vibrationpercent>60) 
  { 
    /* 
    display.clearDisplay();   
    display.setTextSize(2); 
    display.setCursor(0, 20); 
    display.print("  HEAVY  ");
    display.setCursor(0, 40); 
    display.print("VIBRATION"); 
    display.display();*/ 
    delay(900); 
    for(int i=1;i<=4;i++) 
    { 
      digitalWrite(blue,HIGH); 
      digitalWrite(buzzer,HIGH); 
      delay(400); 
      digitalWrite(blue,LOW); 
      digitalWrite(buzzer,LOW); 
      delay(400); 
 
    } 
 
  } 
  digitalWrite(green ,HIGH); 
  delay(800); 
   
  LoRa.beginPacket();   
  LoRa.write(Temperature); 
  LoRa.write(Humidity); 
  LoRa.write(soilTemp); 
  LoRa.write(soilmoisturepercent); 
  LoRa.write(vibrationpercent); 
  //LoRa.write(outgoing.length()); 
  LoRa.endPacket(); 
  digitalWrite(green ,LOW); 
  delay(2000); 
  
} 