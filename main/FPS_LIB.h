#ifndef __FINGERPRONT_SENSOR__
#define __FINGERPRONT_SENSOR__


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> INCLUDES LIBRARIES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
#include <Adafruit_Fingerprint.h>
//#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
//#include "keypad.h"


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEFINITIONS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
#define left_btn   A0    // Define pin A0 as left button
#define middle_btn A1    // Define pin A1 as middle button
#define right_btn  A2    // Define pin A2 as right button

// Initialize LCD object
Adafruit_LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

// Define software serial object for communication with fingerprint sensor
SoftwareSerial mySerial(2, 3);        //RX ,TX
// Define software serial object for communication with esp module 
SoftwareSerial commSerial(8,9);       //RX ,TX
#else

// Define hardware serial object for communication with fingerprint sensor
#define mySerial Serial1

#endif

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FUNCTION PROTOTYPES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

void FPS_Setup();
uint8_t readnumber(void);
int getID();
void check_Enroll();
void FPS_Search();
void FPS_Delete();
uint8_t getFingerprintEnroll();
uint8_t getFingerprintID();
int getFingerprintIDez();
uint8_t deleteFingerprint(uint8_t id);

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Initializations <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
// Initialize Adafruit fingerprint sensor object
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FingerPrint Setup <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void FPS_Setup() {
  commSerial.begin(9600);     // Begin communication with external device
  Serial.begin(9600);         // Begin serial communication
  while (!Serial);            // Wait for serial connection
  delay(100);                 // Delay for stability
  
  Serial.println("\n\nFingerprint sensor enrollment");
  // set the data rate for the sensor serial port
  finger.begin(57600);   
  // Verify password with fingerprint sensor (00000000)
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); 
    }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();      // Get parameters from fingerprint sensor
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
  //Intialize Lcd 
  lcd.begin(16, 2);
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Get FingerPrint ID  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
int getID()
{
  int count = 0;
  while(1)
  {
     // Check if left button is pressed
    if((analogRead(left_btn) <= 10)&&(count > 0))
    {
      count--; // decreament
      // Serial.print("Counter = ");
      
      // Serial.println(count);
      lcd.clear();
      lcd.print("Enter your ID:");
      lcd.setCursor(0, 1);
      lcd.print(count);
    }
    else if(analogRead(middle_btn) <= 10)  // Check if middle button is pressed
    {
      count++;   //increament
      // Serial.print("Counter = ");
      
      // Serial.println(count);
      lcd.clear();
      lcd.print("Enter your ID:");
      lcd.setCursor(0, 1);
      lcd.print(count);
    }
    else if(analogRead(right_btn) <= 10)  // Check if right button is pressed
    {
      return count;
    }
    // Delay for debounce
    delay(100); 
  }
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Get FingerPrint ID From serial  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Is ID Unique <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


// Function to check if the entered ID is unique
bool isIDUnique(uint8_t id) {
  // Query the fingerprint sensor to check if the ID already exists
  uint8_t existingID = finger.fingerID;
  if (existingID != 0) {
    return false;  // ID already exists
  }
  return true;  // ID is unique
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Setup For FingerPrint Enroll  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void check_Enroll()
{
  while(1)
  {

    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # you want to save this finger as...");
    lcd.clear();
    lcd.print("Enter your ID:");
    id = getID();   //to get id from user to enroll 
    lcd.print(id);
    // check for valid id 
    if (id == 0) 
    {
    // ID #0 not allowed, try again!
    lcd.clear();
    lcd.print("This ID Not allowed!");
    delay(1000);
    lcd.clear();
    continue;   // to wait for another id
   }
   else
   {
    break;
   }
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  while (! getFingerprintEnroll() ); // wait untill fingerprint enrolled 
  lcd.clear();
  lcd.print("Enrollment done");
  lcd.setCursor(2,1);
  lcd.print("Your ID : ");
  lcd.print(id);
  delay(1000);   
  lcd.clear();  
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Setup For Enrolling FingerPrint <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void FPS_Search()
{
   // Get template count from fingerprint sensor to search according to this number
    finger.getTemplateCount();
  // check for template count
  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data.");
    lcd.clear();
    lcd.print("No fingerprints");
    delay(1000);
    lcd.clear();
  }
  else {
    Serial.println("Waiting for valid finger...");
    lcd.clear();
    lcd.print("Place your FP");
    delay(500);
    lcd.clear();
    lcd.print("Waiting For FP:)");
    Serial.print("Sensor contains");
    Serial.print(finger.templateCount); 
    Serial.println(" templates");
  }
  uint8_t ID_Search = getFingerprintID();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Attendace Done");
  lcd.setCursor(2,1);
  lcd.print("Your ID : ");
  lcd.print(ID_Search);
  delay(1000);   
  lcd.clear();
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Setup For Deleting FingerPrint <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void FPS_Delete()
{
  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
    lcd.clear();
    lcd.print("Enter ID :");
    delay(1000);
  uint8_t id = getID();
  if (id == 0) {
    // ID #0 not allowed, try again!
    lcd.clear();
    lcd.print("This ID Not allowed!");
    delay(1000);
    lcd.clear();
     return;
  }

  Serial.print("Deleting ID #");
  Serial.println(id);

  deleteFingerprint(id);
  
}



/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Setup For Deleting FingerPrint <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
uint8_t getFingerprintEnroll() 
{
  // variable to store status
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); 
  Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.print("1st Image taken:)");
      delay(500);
      lcd.clear();
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      lcd.clear();
      lcd.print("Waiting For FP:)");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.clear();
      lcd.print("Image converted");
      delay(1000);
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

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  lcd.clear();
  lcd.print("Place FP again");
  delay(1000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.print("2nd Image taken:)");
      delay(500);
      lcd.clear();
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      lcd.clear();
      lcd.print("Waiting For FP:)");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("2nd Image converted");
      lcd.clear();
      lcd.print("Image converted");
      delay(1000);
      lcd.clear();
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
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    lcd.clear();
    lcd.print("Prints matched:)");
    delay(3000);
    lcd.clear();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
        lcd.clear();
    lcd.print("FPs not matched:");
    delay(3000);
    lcd.clear();
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Stored");
    delay(3000);
    lcd.clear();
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}
//----------------------------------------------------------------------------------
uint8_t getFingerprintID() {
  while(1)
  {
    uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      delay(500);
      continue;
      break;
      //return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
      //return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
      //return p;
    default:
      Serial.println("Unknown error");
      break;
      //return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      break;
      //return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
      //return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      break;
      //return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      break;
      //return p;
    default:
      Serial.println("Unknown error");
      break;
      //return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    lcd.clear();
    lcd.print("Matched:)");
    delay(3000);
    lcd.clear();
    break;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    break;
    //return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    lcd.clear();
    lcd.print("Not Matched:(");
    delay(3000);
    lcd.clear();
    break;
    //return p;
  } else {
    Serial.println("Unknown error");
    break;
    //return p;
  }
  }
    // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  if(finger.fingerID>0)
  {
    commSerial.write(finger.fingerID);
  }
  return finger.fingerID;
}
//----------------------------------------------------------------------------------
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
//-------------------------------------------------------------------------------
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
      lcd.clear();
    lcd.print("ID#");
    lcd.print(id);
     lcd.print("is deleted");
    delay(1000);
    lcd.clear();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
  }

  return p;
}

#endif