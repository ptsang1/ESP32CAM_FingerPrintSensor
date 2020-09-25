#include "wifi.h"
#include "camera.h"
#include "pir.h"
#include "led.h"
#include "buzzer.h"
#include "fp.h"
#include "email.h"
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "my_mttq.h"
#include "driver/rtc_io.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include <Servo.h>

#define buzzer_pin 14
#define pir_pin 2
#define servo 12
#define button 16
#define FILE_PHOTO "/photo.jpg"
#define mqtt_server "mqtt.eclipse.org"

bool init_sensors();
void take_a_photo();

bool connect_to_spiffs();
int enroll_fingerprint();
int delete_fingerprint();
int delete_fingerprint2(int id);
void Main();
