#include "fp.h"

HardwareSerial mySerial(1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

bool init_fp(){
	finger.begin(57600);
	delay(5);
	if (finger.verifyPassword()) {
    return true;
	} 
	return false;
}

int getFingerprintToEnroll(int order){
  int p;
  unsigned long start;
  do{
    start = millis();
    do{
      if (millis() - start > 3000){
        return -p;
      }
      Serial.print(".");
      p = finger.getImage();
    }while(p == FINGERPRINT_NOFINGER);
    
    if(p == FINGERPRINT_OK){
      p = finger.image2Tz(order);
      if(p != FINGERPRINT_OK)
        print_error(p);
    }
  }while(p != FINGERPRINT_OK);
}

int enroll_fp(int id){
  Serial.print("Creating model for #");  
  Serial.println(id);
  
  int p = finger.createModel();
  if (p == FINGERPRINT_OK) {
//    Serial.println("Prints matched!");
//    Serial.print("ID "); 
//    Serial.println(id);

    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
      Serial.println("Stored!");
      return id;
    }
  } 
  return -p;
}

int getFingerprintID() {
	int p = finger.getImage();
	if (p == FINGERPRINT_NOFINGER)
    return 0;
	if (p == FINGERPRINT_OK) {
//		Serial.println("Image taken");
		
		p = finger.image2Tz();
		if(p==FINGERPRINT_OK){
//			Serial.println("Image converted");	
			p = finger.fingerSearch();
			if (p == FINGERPRINT_OK) {
//				Serial.println("Found a print match!");
//				Serial.print("Found ID #"); 
//				Serial.print(finger.fingerID); 
//				Serial.print(" with confidence of "); 
//				Serial.println(finger.confidence); 
				return finger.fingerID;
			}
		}
	}
	print_error(p);
	return -p;
}

void print_error(int p){
	switch (p) {
		case FINGERPRINT_PACKETRECIEVEERR:
			Serial.println("Communication error");
			break;
		case FINGERPRINT_IMAGEFAIL:
			Serial.println("Imaging error");
			break;
		case FINGERPRINT_IMAGEMESS:
			Serial.println("Image too messy");
			break;
		case FINGERPRINT_FEATUREFAIL:
			Serial.println("Could not find fingerprint features");
			break;
		case FINGERPRINT_INVALIDIMAGE:
			Serial.println("Could not find fingerprint features");
			break;
    case FINGERPRINT_NOTFOUND:
      Serial.println("Did not find a match");
      break;
    case FINGERPRINT_BADLOCATION:
      Serial.println("Could not delete in that location");
      break;
    case FINGERPRINT_FLASHERR:
      Serial.println("Error writing to flash");
      break;
		default:
		  Serial.println("Unknown error");
		  break;
	}
}

int getImage(){
  return finger.getImage();
}

int deleteFingerprint(uint8_t id) {
  int p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    return p;
  }
  print_error(p);
  return -p;
}
