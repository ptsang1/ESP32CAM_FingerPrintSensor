#include "sensor.h"

String ssid = "Wifi Configuration";
String password = "12345678";

int id = 0;
int wrong = 0;
int lock = 0;
bool pressed = false;
Servo myservo;
String state[2] = {"close", "open"};
int angles[2] = {150, 30};
int changeWifi = false;
bool cam = false;

WiFiClient espClient;
PubSubClient client(espClient);

void set_door(bool ipublish) {
  myservo.write(angles[lock]);
  if (ipublish) {
    send_msg(client, "subscribe/door", state[lock]);
  }
  delay(500);
}

void callback(String topic, byte* message, unsigned int length) {
  String _id;
  for (int i = 0; i < length; i++) {
    _id += (char)message[i];
  }
  
  if (topic == "publish/door") {
    if (_id == "close") lock = 0;
    else {
      lock = 1;
      send_msg(client, "subscribe/open_door", "web");
    }
    set_door(false);
  } else if (topic == "publish/enroll") {
    id = _id.toInt();
    enroll_fingerprint();
  } else if (topic == "publish/delete") {
    delete_fingerprint2(_id.toInt());
  } else if (topic == "publish/reng") {
    announce(WARNING_SOUND);
  } else if (topic == "publish/startStreaming") {
    if (cam && startStreaming())
      turn_on_flash();
    else
      send_msg(client, "subscribe/cameraFailed", "1");
  }else if (topic == "publish/stopStreaming") {
    turn_off_flash();
    stopStreaming();
  }else if (topic == "publish/stopCapturing") {
    delay(1000);
    turn_off_flash();
    stopCapturing();
  } else
    id = _id.toInt();
}

bool init_sensors() {
  init_buzzer(buzzer_pin);
  
  if (init_cam()){
    cam = true;
    announce(SUCCESS_SOUND);
  }else{
    announce(WRONG_SOUND);
  }

  _connect_to_wifi(ssid, password);
  announce(SUCCESS_SOUND);

  if (!connect_to_spiffs())
    return false;

  pinMode(button, INPUT);
  init_led();
  init_pir(pir_pin);

  myservo.attach(servo, 5);
  set_door(true);

  if (!init_fp())
    return false;
  announce(SUCCESS_SOUND);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  announce(SUCCESS_SOUND);

  send_msg(client, "publish/ip", WiFi.localIP().toString());
  //  send_msg(client,"subscribe/on", "on");
  announce(SUCCESS_SOUND);
  return true;
}

// Check if photo capture was successful
bool checkPhoto(fs::FS &fs) {
  File f_pic = fs.open(FILE_PHOTO);
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );
}

void take_a_photo() {
  bool starting=false;
  if(cam){
    starting = startCapturing();
    turn_on_flash();
  }
  send_msg(client, "subscribe/send_email", "ok");
  delay(1000);
}

//void take_a_photo(){
//  bool Stop = false;
//  do{
//    turn_on_flash();
//    camera_fb_t* fp_c = capturePhoto();
//    if (fp_c){
//      File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);
//      if(file) {
//        file.write(fp_c->buf, fp_c->len);
//        Stop = true;
//      }
//      file.close();
//      esp_camera_fb_return(fp_c);
//    }
//    else{
//      Serial.println("Camera capture failed");
//      Stop = false;
//    }
//    turn_off_flash();
//  }while(!checkPhoto(SPIFFS));
//  sendPhoto(FILE_PHOTO);
//}

bool connect_to_spiffs() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }
  delay(500);
  Serial.println("SPIFFS mounted successfully");
  return true;
}

int enroll_fingerprint() {
  send_msg(client, "subscribe/announce", "Please put your finger on the sensor to enroll.");
  announce(WAITING_SOUND);
  if (getFingerprintToEnroll(1) < 0) {
    send_msg(client, "subscribe/announce", "Enrolling is stopped. Because you didn't put your finger on the sensor.");
  } else {
    send_msg(client, "subscribe/announce", "Please remove your finger until the next message.");
    announce(WAITING_SOUND);
    if (getImage() != FINGERPRINT_NOFINGER) {
      send_msg(client, "subscribe/announce", "Enrolling is stopped. Because you didn't put your finger out of the sensor.");
    } else {
      send_msg(client, "subscribe/announce", "Please place same finger again.");
      announce(WAITING_SOUND);
      if (getFingerprintToEnroll(2) < 0) {
        send_msg(client, "subscribe/announce", "Enrolling is stopped. Because you didn't put your finger on the sensor.");
      }
      else {
        int res = enroll_fp(id);
        if (res > 1) {
          send_msg(client, "subscribe/announce", "Enrolling your finger is successfull.");
          send_msg(client, "subscribe/enroll", String(id));
          announce(SUCCESS_SOUND);
          return 1;
        }
        send_msg(client, "subscribe/announce", "Enrolling your finger is failed.");
      }
    }
  }
  announce(WRONG_SOUND);
  return 0;
}

int delete_fingerprint() {
  send_msg(client, "subscribe/announce", "Please put your finger on the sensor.");
  announce(WAITING_SOUND);
  int id = getFingerprintID();
  if (id == 1 || id == 2) {
    send_msg(client, "subscribe/announce", "Can't remove this fingerprint.");
    announce(WRONG_SOUND);
    return 0;
  } else if (id < 0) {
    send_msg(client, "subscribe/announce", "Fingerprint dosen't exist.");
    return 0;
  } else {
    int r = deleteFingerprint(id);
    if (r < 0) {
      send_msg(client, "subscribe/announce", "Removing fingerprint is failed.");
      announce(WRONG_SOUND);
      return 0;
    }
  }
  send_msg(client, "subscribe/announce", "Removing fingerprint is successfull.");
  send_msg(client, "subscribe/delete", String(id));
  announce(SUCCESS_SOUND);
  return 0;
}

int delete_fingerprint2(int id) {
  int r = deleteFingerprint(id);
  if (r < 0) {
    send_msg(client, "subscribe/announce", "Removing fingerprint is failed.");
    announce(WRONG_SOUND);
    return 0;
  }

  send_msg(client, "subscribe/announce", "Removing fingerprint is successfull.");
  send_msg(client, "subscribe/delete", String(id));
  announce(SUCCESS_SOUND);
  return 0;
}

void Main() {
  if (!client.connected())
    reconnect(client);
  client.loop();

  if (warning())
    take_a_photo();

  if (pressed && !digitalRead(button)) {
    wrong = 0;
    lock = !lock;
    set_door(true);
    if (lock)
      send_msg(client, "subscribe/open_door", "home");
  }
  pressed = digitalRead(button);

  int p = getFingerprintID();
  if (-p == FINGERPRINT_NOTFOUND) {
    wrong++;
    announce(WRONG_SOUND);
    if (wrong > 4) {
      take_a_photo();
      announce(WARNING_SOUND);
    }
  } else if (p >= 1) {
    wrong = 0;
    if (p == 1)
      enroll_fingerprint();
    else if (p == 2)
      delete_fingerprint();
    else if (p > 2) {
      wrong = 0;
      lock = !lock;
      set_door(true);
      if (lock)
        send_msg(client, "subscribe/open_door", String(p));
      announce(SUCCESS_SOUND);
    }
  }
}
