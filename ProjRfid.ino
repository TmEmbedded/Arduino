
#include <MFRC522.h>
#include <Servo.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
Servo servo;
#include <BlynkSimpleEsp8266.h>

#define SS_PIN 4
#define RST_PIN 5


char auth[] = "a0a47eb6e3ce472e8371f8b72b14b820";
char ssid[] = "Sarai";
char pass[] = "123456789";

int led = 15;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void setup()
{
  
  Serial.begin(115200);
  servo.attach(2);
  servo.write(0);
  delay(2000);

  Blynk.begin(auth, ssid, pass);SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  
  pinMode(led, OUTPUT);
}

void loop()
{
  Blynk.run(); 

if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "DC 5D A4 BB") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Blynk.virtualWrite(V5,"BRICE");
    servo.write(90);              
    delay(3000);
    servo.write(0);
  }
  else if (content.substring(1) == "EC C1 C6 35") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Blynk.virtualWrite(V5,"SARAH");
    servo.write(90);              
    delay(3000);
    servo.write(0);
    
  }
 
 else   {
    Serial.println(" Access denied");
    Blynk.virtualWrite(V5,"DENIED");
   
   Serial.println();
    //delay(3000);
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(500);               // wait for a second
    
    
  }
  
}
