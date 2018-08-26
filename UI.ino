// ============================================
//  UI 
// ============================================
void ui() {
  switch(SimbleeForMobile.screen) {
    case 1:  SimbleeBLE.dualModeBegin(); createScreen0();  break;      
    case 2:  SimbleeBLE.dualModeEnd();   createScreen1();  break;
    default: Serial.print("ui: Uknown screen requested: "); Serial.println(SimbleeForMobile.screen);
  }    
}
void createScreen0() {
  SimbleeForMobile.beginScreen(BLACK, PORTRAIT);  // ---   
    text1 = SimbleeForMobile.drawText( 100, 106, "00",  YELLOW, 120);      // Title  
    
    SimbleeForMobile.drawRect(60, 300, 200, 2, RED); // -----------------  
  commonScreen();      
  SimbleeForMobile.endScreen(); // ---------------------
  SimbleeForMobile.updateValue(select, SELECT);           
}
void createScreen1() {
  SimbleeForMobile.beginScreen(BLACK, PORTRAIT);  // ---   
     
    char *titles1[] = { "Sound On", "Sound Off" };
    select1 = SimbleeForMobile.drawSegment(60, 150, 200, titles1, countof(titles1), GREEN);  // 

  commonScreen();      
  SimbleeForMobile.endScreen(); // ---------------------
  SimbleeForMobile.updateValue(select, SELECT);           
  SimbleeForMobile.updateValue(select1, 1);    
}
void commonScreen() {
   char *titles0[] = { "Auto", "Setting" };
   select = SimbleeForMobile.drawSegment(80, 435, 160, titles0, countof(titles0), GRAY);
}
void ui_event(event_t &event) {  
  if(event.id == select)  { SELECT = event.value; SimbleeForMobile.showScreen(SELECT+1); }    
  if(event.id == select1) { SELECT1 = event.value; SimbleeBLE.dualModeEnd(); } 
}

