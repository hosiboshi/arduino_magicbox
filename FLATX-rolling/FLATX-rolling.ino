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
   CRGB leds[2];                       // RGB LED buffer
   long itime1, itime2, itime3,itime4, Ttime;         // event timer   
   boolean tgll = true; 
   int dist;   
   int rsi;
   int got;
   int recdist;
   int cnt=0;
   int Max=0;
   int Min = 0;
   int tc=0;
   int rflg=0;
   int rcnt1=0;
   int rcnt2=0;
   int Rrflg=0;
   int Rrcnt2=0;
   int Rrcnt1=0;
   int onkai=0;
   int stop=0;
   
// ============================================
//  SETUP
// ============================================
void setup() {
   Serial.begin(115200); 
   Wire.begin();               // SCL:12 SDA:25
   randomSeed(analogRead(0));  
   pinMode( 7, INPUT );        // TACT SW
   pinMode( 4, OUTPUT);        // LED A
   pinMode( 3, OUTPUT);        // LED B
   pinMode( 6, OUTPUT);        // LED C
   pinMode( 9, OUTPUT);        // LED D
   pinMode(15, OUTPUT);        // LED  
   FastLED.addLeds<NEOPIXEL, 11>(leds, 2); // FastLED setup for RGB LEDs       
   leds[0] = CRGB(10, 10, 0); leds[1] = CRGB(10, 10, 0);  FastLED.show();
   SimbleeCOM.begin();     
   Gyrosetup();                // GYRO setup  
   APDS9960setup();          // PROXIMITY setup    
}
// ============================================
//  LOOP
// ============================================

void loop() {
  tc++;
  if(millis() - itime1 > 10) { itime1 = millis();  // 
     getMotion9(); //Max:32767Min:-32768
     //Serial.print(gx); Serial.print("\t"); Serial.print(gy); Serial.print("\t"); Serial.println(gz);
     if(0){
      Min = gx; Serial.println(Min);
     }
     if(gx == 32767){
      rflg=1;
      rcnt2++;
     }
     if(rflg)  rcnt1++;
      if(rcnt1 == 100 || rcnt2 > 40){
        if(rcnt2 > 40){
          Serial.println("A_rotate");
          makeUpsound();
        }else{
          Serial.println("not_rotate");
        }
        rcnt1 = 0;
        rcnt2 = 0;
        rflg=0;
      }
      if(gx == -32768){
      Rrflg=1;
      Rrcnt2++;
     }
     if(Rrflg)  Rrcnt1++;
      if(Rrcnt1 == 100 || Rrcnt2 > 40){
        if(Rrcnt2 > 40){
          Serial.println("B_rotate");
          makeDownsound();
        }else{
          Serial.println("not_rotate");
        }
        Rrcnt1 = 0;
        Rrcnt2 = 0;
        Rrflg=0;
      }
     /*else if( gx+gz < -50000) Serial.println("B_rotate");
     else Serial.println("not"); */
     
     /*payloadd.GX = gx;
     payloadd.GY = gy;
     payloadd.GZ = gz;
     SimbleeCOM.send((const char*)&payloadd, sizeof(payloadd));*/
  }
  if(gx == 32767){ 
    //itime2 = millis();
  }
  //Serial.print(tc); Serial.print("\t");
  //Serial.println(cnt);
  if(!digitalRead(7)) cnt=0;
  
  if(recvflg) { recvflg = 0; }
  if(got){ 
    //Serial.println(rsi);
    got = 0;
  }
}

// ============================================
//  COM call back
// ============================================
void SimbleeCOM_onReceive(unsigned int esn, const char* paydvalue, int len, int rssi) { //Serial.print("len:"); Serial.print(len);
  if(0){
  memcpy(&payloadd, paydvalue, len); 
  got = 1;
  rsi = rssi;
  recdist = payd2.dist;
  Serial.println(recdist);
  //Serial.print(esn);Serial.print("\t");Serial.println(rsi);
  }
}

void makeUpsound(){
  if(onkai == 7){
    onkai = 0;
  }else onkai++;
  tone(3,doremi[onkai],1000);
  digitalWrite(4, tgll);   // B
  digitalWrite(6, tgll);   // C
  digitalWrite(9, tgll);   // D
  randcolor();
  delay(1000);
  digitalWrite(4, 0);   // B
  digitalWrite(6, 0);   // C
  digitalWrite(9, 0);   // D
}
void makeDownsound(){
  if(onkai == 0){
    onkai = 7;
  }else onkai--;
  tone(3,doremi[onkai],1000);
  digitalWrite(4, tgll);   // B
  digitalWrite(6, tgll);   // C
  digitalWrite(9, tgll);   // D
  randcolor();
  delay(1000);
  digitalWrite(4, 0);   // B
  digitalWrite(6, 0);   // C
  digitalWrite(9, 0);   // D
}
void randcolor(){
  int R_t = random(10);
  int G_t = random(10);
  int B_t = random(10);
  leds[0] = CRGB(R_t, G_t, B_t);
  leds[1] = CRGB(R_t, G_t, B_t);
  FastLED.show();
}


