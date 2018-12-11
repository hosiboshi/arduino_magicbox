// ============================================
//  PAYLOAD
// ============================================
#define PINNO 3
#define BEAT 500
   struct payloadStructure { // COM communication packet
     int16_t GX; 
     int16_t GY; 
     int16_t GZ; 
   } __attribute__((packed));
   payloadStructure payloadd;   
   struct payloadStructure2 { // COM communication packet
     int8_t  CC; // (dice)
     int16_t PP; // (gravity)
     int16_t XX; 
     int16_t YY; 
     int16_t ZZ; 
   } __attribute__((packed));
   payloadStructure payloadd2; 
   struct paydvalue { // COM communication packet
     int  dist; 
   } __attribute__((packed));
   paydvalue payd1; 
   struct paydvalue2 { // COM communication packet
     int  dist; 
   } __attribute__((packed));
   paydvalue2 payd2; 
   int16_t ax, ay, az, gx, gy, gz, mx, my, mz;
   //int ggp = 0;
   //byte ast;        // dice   
   //int16_t gst = 0; // gravity  
   byte recvflg = 0;         // flag for receive data dump  
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
              int doremi[8]={262,294,330,349,392,440,494,523};

