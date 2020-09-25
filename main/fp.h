#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

int enroll_fp(int id);
bool init_fp();
int getFingerprintEnroll(int id);
int getFingerprintID();
void print_error(int p);
int getNewID();
int getFingerprintToEnroll(int order);
int getImage();
int deleteFingerprint(uint8_t id);
