/*
   -- rx-tx-simple --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.10 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.11.1 or later version;
     - for iOS 1.9.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB_POINT
#include <ESP8266WiFi.h>

#include <RemoteXY.h>
#include <Servo.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "poweredbyme"
#define REMOTEXY_WIFI_PASSWORD "1234567890"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 49 bytes
  { 255,3,0,0,0,42,0,16,31,1,5,32,2,38,59,59,2,26,31,2,
  1,21,18,20,9,2,26,31,31,79,78,0,79,70,70,0,129,0,25,9,
  12,6,17,108,105,103,104,116,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t joystick_x; // from -100 to 100  
  int8_t joystick_y; // from -100 to 100  
  uint8_t switch_light; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_SWITCH_LIGHT 2
#define PIN_ENGINE_FWD 13
#define PIN_ENGINE_BWD 15
#define PIN_SERVO_STEERING 12

#define IS_STEERING_REVERSED false

int SERVO_STEERING_CENTER = 1500;
Servo servo_steering;


void setup() {
  RemoteXY_Init (); 
  pinMode(PIN_SWITCH_LIGHT, OUTPUT);
  pinMode(PIN_ENGINE_FWD, OUTPUT);
  pinMode(PIN_ENGINE_BWD, OUTPUT);
  servo_steering.attach(PIN_SERVO_STEERING);
}

void loop() { 
  RemoteXY_Handler ();
  if (RemoteXY.connect_flag == 1) {
    updateSteering(RemoteXY.joystick_x);
    updateEngine(RemoteXY.joystick_y);
    updateLight(RemoteXY.switch_light);
  } else {
    updateSteering(0);
    updateEngine(0);
    updateLight(0);
  }
}


void updateSteering(int8_t steering_value) {
  servo_steering.writeMicroseconds(SERVO_STEERING_CENTER + 10*((IS_STEERING_REVERSED) ? -1*steering_value : steering_value));
}

void updateEngine(int8_t engine_value) {
  int v = (int) abs(2.55 * engine_value);
  if (engine_value > 1) {
    analogWrite(PIN_ENGINE_FWD, v);
    analogWrite(PIN_ENGINE_BWD, 0);
  } else  if (engine_value < -1) {
    analogWrite(PIN_ENGINE_FWD, 0);
    analogWrite(PIN_ENGINE_BWD, v);
  } else {
    analogWrite(PIN_ENGINE_FWD, 0);
    analogWrite(PIN_ENGINE_BWD, 0);
  }
}

void updateLight(uint8_t switch_value) {
    digitalWrite(PIN_SWITCH_LIGHT, (switch_value==0) ? LOW : HIGH);
}