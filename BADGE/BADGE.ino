// ============================================
//  Base design http://simblee.com
//  Language Reference http://arduino.cc/en/Reference/HomePage
//  24kRAM, 128kROM
// ============================================
  #include "FastLED.h"           // RGB LED 
  #include "SimbleeForMobile.h"  //
  #include "SimbleeBLE.h"  
  #include "Wire.h"              // I2C    
  #include "para.h"              // parameter
  
// ============================================
//  SETUP
// MOTOR DRIVER PH:D20/EN:D6
// BUZ:D13
// IR receive:D9
// RGB LED:D11
// LED:D15
// SW:D7 (pullup reqired)
// GYRO INT:IO23
// ============================================
void setup() {
  Serial.begin(9600);
  Wire.begin();                    // SCL:12 SDA:25
  FastLED.addLeds<NEOPIXEL, 11>(leds, 1);     // FastLED setup for RGB LEDs      
  leds[0] = CRGB(0, 4, 0); FastLED.show();

  pinMode(13, OUTPUT);             // BUZZ 
  pinMode(15, OUTPUT);             // test LED    
  pinMode(7,  INPUT_PULLUP);       // Tact SW   
  pinMode(6,  OUTPUT); digitalWrite(6, 0);             // EN
  pinMode(20, OUTPUT);             // PH
  pinMode(23, INPUT_PULLUP);       // GYRO INT
  Gyrosetup();                     // GYRO setup  

  // ---- enable dual mode ------------------  
  SimbleeForMobile.deviceName = "TONE";         // device name
  char buf [4]; sprintf (buf, "%02x", SimbleeCOM.getESN());
  SimbleeForMobile.advertisementData = buf;
  SimbleeForMobile.begin();                     // For mobile start
  SimbleeBLE.dualModeBegin();   
}
// ============================================
//  LOOP
// -80, -70, -60, -50
//  5m   4m   3m
// ============================================
void loop33() {
  //SimbleeForMobile.process(); // BLE controller   
  if(millis() - itime2 > 2000) { itime2 = millis(); // 
    digitalWrite(15, tgll); tgll = !tgll;
  }
}
void loop() { 
  SimbleeForMobile.process(); // BLE controller  
  if(millis() - itime1 > 500 && got) { itime1 = millis();    // 
    got = 0;  Serial.println(dist); 
    /*if(SimbleeForMobile.updatable) { 
       int x = 100; if(popo < 10) { x += 25; } else if(popo < 20) { x += 10; }
       SimbleeForMobile.updateValue(text1, popo);  SimbleeForMobile.updateX(text1, x);       
       SimbleeForMobile.updateColor(text1, GREEN); delay(200); SimbleeForMobile.updateColor(text1, YELLOW); 
    } */
  }
  if(millis() - itime2 > 2000) { itime2 = millis(); // 
    if(!SELECT1) {
    for(int n = 1; n < 4; n++) { 
       tone(13, 3600*5/popo+4); delay(80); tone(13, 3600*5/popo-4); delay(80); } noTone(13);
    }
    digitalWrite(15, tgll); tgll = !tgll;
  }
}
// ============================================
//  DUAL MODE CALL BACK
// ============================================
void SimbleeCOM_onReceive(unsigned int esn, const char* payload, int len, int rssi) { //Serial.print("len:"); Serial.print(len);
  memcpy(&payloadd2, payload, len); got = 1; // display req flag
  got = 1; popo = -1*rssi - 35;
  dist = payloaded2.dist;
}
void SimbleeBLE_onDualModeStart() { // COM data transfer
  if(COMsndReq > 0) { SimbleeCOM.send((const char*)&payloadd, sizeof(payloadd)); COMsndReq = 0; }
}
void SimbleeBLE_onDualModeEnd() {}
// ============================================
//  STATE
// ============================================
void SimbleeForMobile_onConnect()    { //SimbleeBLE.dualModeEnd();   
}
void SimbleeForMobile_onDisconnect() { SimbleeBLE.dualModeBegin(); 
}


