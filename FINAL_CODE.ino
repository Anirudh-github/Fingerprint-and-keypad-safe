#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "Adafruit_Fingerprint.h"
#include <Keypad.h>






const int  en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3;
const int i2c_addr = 0x3F;
LiquidCrystal_I2C lcd(0x3F, 16,2);
SoftwareSerial mySerial(12, 13);
boolean flag = true;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Servo name_servo;
int servo_pos = 0;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
}; 
byte rowPins[ROWS] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4}; //connect to the column pinouts of the keypad

char* password = "1234";Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int pos = 0;
void setup()
{
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(3,0);
  Serial.begin(9600);
  setLocked(true);
  name_servo.attach(11);
  name_servo.write(90);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}
void loop()
{
 //lcd.backlight();
 // if(flag)
  //{
    //getFingerprintIDez();

  //}

 if ( getFingerprintIDez() != -1){ 
    Serial.println("ENTER PASSWORD");
    lcd.setCursor(0,0);
    lcd.print("ENTER PASSWORD");
     while(true)
{
    char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.write(key);
    lcd.setCursor(0,1);
    lcd.print(key);
    delay(800);
    lcd.clear();
      
  }
  
  if (key == password[pos])

  {
    pos++;
  }

  if (pos==4)
  {
    setLocked(false);
    lcd.print("ACCESS GRANTED");

  } 
  if (key == '*' || key == '#')
  {
    pos = 0;
    setLocked==(true);
    name_servo.write(90);   
  }
  //delay(250);

}
}
}


void setLocked(int locked)
{
  if(locked)
  {
    name_servo.write(90);
  }
  else
  {
    name_servo.write(15);
  }
}
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  // found a match!//

  
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
return finger.fingerID;
}
 

  
  
