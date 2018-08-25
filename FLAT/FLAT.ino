// ============================================
//  Base design http://simblee.com
//  Language Reference http://arduino.cc/en/Reference/HomePage
//  24kRAM, 128kROM
// ============================================
//  FILE INCLUDE 
// ============================================ 
   #include "Wire.h"                   // I2C 
   #include "FastLED.h"                // RGB LED       
   #include "SimbleeCOM.h"             // COM
   #include "para.h"                   //       
   
// ============================================
//  CONTROL VALIABLES
// ============================================
   CRGB leds[1];                     // RGB LED buffer
   long itime1, Ttime;               // event timer   
   boolean tgll = true;    
   int intv = 100;  
   
// ============================================
//  SETUP
// ============================================
void setup() {
   Serial.begin(115200); 
   Wire.begin();               // SCL:12 SDA:25  
   pinMode( 2, INPUT );        // Voltage
   pinMode( 7, INPUT );        // TACT SW
   pinMode(15, OUTPUT);        // LED  
   FastLED.addLeds<NEOPIXEL, 11>(leds, 1); // FastLED setup for RGB LEDs       
   leds[0] = CRGB(10, 0, 0); FastLED.show();
   SimbleeCOM.begin();     
   Gyrosetup();                // GYRO setup  
   APDS9960setup();          // PROXIMITY setup    
}
// ============================================
//  CORO
// ============================================
void coro() {
    long AX = 0, AY = 0, AZ = 0, GX = 0, GY = 0, GZ = 0; int sn = 10;
    for (int n = 0; n < sn; n++) {
       getMotion9(); AX += ax; AY += ay;  AZ += az; GX += gx; GY += gy; GZ += gz; delayMicroseconds(10);  
    }  ax = AX/sn; ay = AY/sn; az = AZ/sn; gx = GX/sn; gy = GY/sn; gz = GZ/sn;  
    // GRAVITY -----    
      readProximity();
      if(distance > 100 && gst < 8000) { gst += 100; } 
      else { gst -= 100; } if(gst < 0) { gst = 0; }
    // XYZ TRANSITION -----
      ggx[ggp] = gx; ggy[ggp] = gy; ggz[ggp] = gz; ggp++; if(ggp > 9) { ggp = 0; }
      long gggx = 0; long gggy = 0; long gggz = 0;
      for (int n = 0; n < 10; n++) { gggx += ggx[n]; gggy += ggy[n]; gggz += ggz[n]; } 
      gx = gggx/10; gy = gggy/10; gz = gggz/10;
                 
    Serial.print(ax); Serial.print("\t"); Serial.print(ay); Serial.print("\t"); Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t"); Serial.print(gy); Serial.print("\t"); Serial.print(gz); Serial.print("\t");
    Serial.print(ast); Serial.print("\t"); Serial.println(gst);
}
// ============================================
//  SLEEP
// ============================================
int TactCallback(uint32_t button) { Simblee_resetPinWake(button); Simblee_systemReset(); }  
void batteryChk() { // 
  pinMode( 23, INPUT ); Serial.print("sleep"); Serial.println(analogRead(2)); delay(500);
  pinMode( 23, OUTPUT ); digitalWrite(23, 1);  Serial.print("measure"); Serial.println(analogRead(2)); delay(500);
}
void sensorSleep() {
    I2Cwrite8(0x39, 0x80, 0x00);  //APDS9960 Enable Register (0x80)
    // 0 |GEN |PIEN |AIEN |WEN |PEN |AEN |PON(<sleep) |
    I2Cwrite8(0x68,0x6B,0x80);    //MPU9250  PWR_MGMT_1 R/W (0x6B) 
    I2Cwrite8(0x68,0x6B,0x40);    // sleep           
    // H_RESET(<reset)|SLEEP(<sleep)|CYCLE|GYRO_STANDBY|PD_PTAT|CLKSEL[2:0]|
    pinMode( 23, INPUT ); pinMode( 8, INPUT ); pinMode( 21, INPUT ); pinMode( 19, INPUT ); delay(100); 
}
// ============================================
//  LOOP
// ============================================
void loop() {
    if(millis() - itime1 > 40) { itime1 = millis(); //
        getMotion9();
        Serial.print(ax); Serial.print("\t"); Serial.print(ay); Serial.print("\t"); Serial.println(az);
    }
}
void loop333() {
  if(!digitalRead(7)) { if(readBtn) { Ttime = millis(); readBtn = 0; } }       // Switch ON
  else if(!readBtn) { readBtn = 1;
    if(millis() - Ttime > 1000) { // SLEEP mode
       sensorSleep(); leds[0] = CRGB(0, 0, 0); FastLED.show(); digitalWrite(15, 0);      
       Simblee_pinWakeCallback(7, LOW, TactCallback); Simblee_systemOff();     // Sleep!!
    }
    else { // SPIDER向けのコマンド
       payloadd.CC = 0xcc; // command by tact switch
       getMotion9(); payloadd.XX = ax;  payloadd.YY = ay; payloadd.ZZ = az; // int       
       SimbleeCOM.send((char*)&payloadd, sizeof(payloadd));          
    }
  }        
  if(millis() - itime1 > intv) { itime1 = millis();  // 
     coro(); intv = 100; if(gst < 100) intv = 600;   // adjust interval
     payloadd.CC = 0xbb; // flat id
     payloadd.PP = gst; 
     payloadd.XX = ax;  payloadd.YY = ay; payloadd.ZZ = az; // int
     payloadd.GX = gx;  payloadd.GY = gy; payloadd.GZ = gz; // int
     SimbleeCOM.send((char*)&payloadd, sizeof(payloadd));    
     tgll = !tgll; if(tgll) { leds[0] = CRGB(4, 0, 0); } else { leds[0] = CRGB(0, 4, 0); } 
     FastLED.show(); digitalWrite(15, tgll);  
  }
}
// ============================================
//  COM call back
// ============================================
void SimbleeCOM_onReceive(unsigned int esn, const char *payload, int len, int rssi) {
  //memcpy(&payloadd, payload, len); recvflg = 1;  //
  // FLAT 0xbb                 >> for FLY
  //      0xcc(TactSW oneshot) >> for SPIDER (ax/ay/azでcommand化)
  // ROLL DICE(0-15)           >> for WALK
  //      0xaa(TactSW oneshot) >> for WALK test
  // FLY: 0xbb !!
  // SPIDER: 0xcc !!
  // WALK: !0xbb || !0xcc   
}




