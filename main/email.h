#define emailSenderAccount    "1753096.17clc@gmail.com"
#define emailSenderPassword   "6959SaNg#@"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32-CAM Photo Captured"

#include "ESP32_MailClient.h"

void sendPhoto(const char* FILE_PHOTO);
void sendCallback(SendStatus msg);
void set_email(String username);
