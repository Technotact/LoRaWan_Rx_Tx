#include <SPI.h> 
#include <LoRa.h>  
#include <LiquidCrystal_I2C.h> 
#include <Servo.h> 
LiquidCrystal_I2C lcd(0X27, 20, 4); 
 
byte Temperature; 
byte Humidity; 
byte soilTemp; 
byte soilmoisturepercent; 
byte vibrationpercent; 
Servo servo; 
int val = 0; 
const int red = 4; 
const int green = 3; 
const int blue = 5; 
const int buzzer = 6; 
  
void setup() { 
  Serial.begin(115200); 
  pinMode(red,OUTPUT); 
  pinMode(green,OUTPUT); 
  pinMode(blue,OUTPUT); 
  pinMode(buzzer,OUTPUT); 
 
  servo.attach(7); 
  servo.write(110); 
 
  lcd.init(); 
  lcd.begin(20,4); 
  lcd.backlight(); 
  lcd.clear(); 
   
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("___________VERSATILE"); 
  lcd.setCursor(0,1); 
  lcd.print("_________LONG  RANGE"); 
  lcd.setCursor(0,2); 
  lcd.print("_______COMMUNICATION"); 
  lcd.setCursor(0,3); 
  lcd.print("______________SYSTEM"); 
  delay(5000); 
 
  lcd.clear(); 
  lcd.setCursor(0,1); 
  lcd.print("STARTING  UP"); 
  lcd.setCursor(0,2); 
  lcd.print("the SYSTEM ........."); 
  delay(4000); 
   
  while (!Serial); 
  Serial.println("LoRa Receiver"); 
  if (!LoRa.begin(433E6)) {  
    Serial.println("Starting LoRa failed!"); 
    while (1); 
  } 
} 
  
void loop() { 
   
  digitalWrite(green ,HIGH); 
  int packetSize = LoRa.parsePacket(); 
  digitalWrite(green ,LOW); 
  if (packetSize == 0) return; 
   
  Temperature=LoRa.read(); 
  Humidity=LoRa.read(); 
  soilTemp=LoRa.read(); 
  soilmoisturepercent=LoRa.read(); 
  vibrationpercent=LoRa.read(); 
 
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("AIR  TEMPERATURE");  
 
  lcd.setCursor(0,1); 
  lcd.print("------>"); 
  lcd.setCursor(8,1); 
  lcd.print(String(Temperature)); 
  lcd.setCursor(11,1); 
  lcd.print(char(176));
  lcd.setCursor(12,1); 
  lcd.print("C"); 
  lcd.setCursor(0,2); 
  lcd.print("SOIL TEMPERATURE"); 
  lcd.setCursor(0,3); 
  lcd.print("------>"); 
  lcd.setCursor(8,3); 
  lcd.print(String(soilTemp)); 
  lcd.setCursor(11,3); 
  lcd.print(char(176)); 
  lcd.setCursor(12,3); 
  lcd.print("C"); 
  delay(2500); 
 
  lcd.clear(); 
  lcd.setCursor(0,0); 
  lcd.print("HUMIDITY  -> ");  
  lcd.setCursor(13,0); 
  lcd.print(String(Humidity)); 
  lcd.setCursor(16,0); 
  lcd.print("%"); 
  lcd.setCursor(0,1); 
  lcd.print("VIBRATION -> ");  
  lcd.setCursor(13,1); 
  lcd.print(String(vibrationpercent)); 
  lcd.setCursor(16,1); 
  lcd.print("%"); 
  lcd.setCursor(0,2); 
  lcd.print("SOIL MOISTURE-> "); 
  lcd.setCursor(16,2); 
  lcd.print(String(soilmoisturepercent)); 
  lcd.setCursor(19,2); 
  lcd.print("%"); 
 
  delay(2500); 
 
  if((soilTemp)>30) 
  { 
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Soil / Rock"); 
    lcd.setCursor(0,1); 
    lcd.print("Temperature value is"); 
    lcd.setCursor(5,2); 
    lcd.print(String(soilTemp)); 
    lcd.setCursor(8,2); 
    lcd.print(char(176));
      lcd.setCursor(9,2); 
    lcd.print("C"); 
    lcd.setCursor(0,3); 
    lcd.print("--HIGH TEMP ALERT---"); 
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
  if((vibrationpercent)>60) 
  { 
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Soil / Rock");  
 
    lcd.setCursor(0,1); 
    lcd.print("Vibration is around"); 
    lcd.setCursor(5,2); 
    lcd.print(String(vibrationpercent)); 
    lcd.setCursor(8,2); 
    lcd.print("%"); 
    lcd.setCursor(0,3); 
    lcd.print("HIGH VIBRATION ALERT"); 
    servo.write(0); 
    for(int i=1;i<=4;i++) 
    { 
      digitalWrite(blue,HIGH); 
      digitalWrite(buzzer,HIGH); 
      delay(400); 
      digitalWrite(blue,LOW); 
      digitalWrite(buzzer,LOW); 
      delay(400); 
    } 
  }else{ 
    servo.write(110); 
  }   
    
}
