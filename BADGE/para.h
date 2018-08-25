// ============================================
//  UI
// ============================================  
  byte text1, select, select1;
// ============================================
//  GYRO
// ============================================  
  float Ax, Ay, Az, Gx, Gy, Gz;    // gyro accel 
// ============================================
//  IO assignment
// ============================================
  CRGB leds[1];                  // RGB LED buffer  
// ============================================
//  VALIABLES
// ============================================
  int got;
  long itime1, itime2, itime3;
  int tgll;
  int popo;
  int COMsndReq = 0;
  int SELECT  = 0;
  int SELECT1 = 0;
  int ss[10]; int sn = 0;
// ============================================
//  COM
// ============================================  
//  struct payloadStructure {  // COM communication packet for send 15byte max
//     int8_t temp; 
//  } __attribute__((packed));
//  payloadStructure payloadd;    
//  struct payloadStructure2 { // COM communication packet for receive 15byte max
//     int8_t temp;
//  } __attribute__((packed));
//  payloadStructure2 payloadd2;   

  struct RGBvalueStruct1 {
    int8_t red, blue, green;
  }__attribute__((packed));
  RGBvalueStruct1 rgbvalue1;

  struct RGBvalueStruct2 {
    int8_t red, blue, green;
  }__attribute__((packed));
  RGBvalueStruct2 rgbvalue2;
    
