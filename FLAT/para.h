// ============================================
//  PAYLOAD
// ============================================
   struct payloadStructure { // COM communication packet
     int8_t  CC; // (dice)
     int16_t PP; // (gravity)
     int16_t XX; 
     int16_t YY; 
     int16_t ZZ; 
     int16_t GX; 
     int16_t GY; 
     int16_t GZ;  // 20byte MAX!!
   } __attribute__((packed));
   payloadStructure payloadd;   
   int16_t ax, ay, az, gx, gy, gz, mx, my, mz;
   int16_t ggx[10];
   int16_t ggy[10];
   int16_t ggz[10]; 
   int ggp = 0;
   byte ast;        // dice   
   int16_t gst = 0; // gravity  
   byte recvflg = 0;         // flag for receive data dump  
   uint8_t readBtn = 1;      // Tact Sw     
// ============================================
// GESTURE
// ============================================
/* Color */   int ambD, redD, greenD, blueD; // ambient, Red, Green, Blue
/* Distance*/ int distance  = 0;             // prximity sensor data 
/* Gesture*/  int Gesture   = 0;             // Gesture Pattern
              int GestureGo = 0;             // Gesture Pattern             
              int stayC = 0;      // long stay timer
              int k;              // Gesture
              int Oshot = 0;      // One shot timer
              int Gmask = 0;      // set:GestureGo reset distance < 50   
