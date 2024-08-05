#include "FPS_LIB.h"
int getOption();

void setup(){
  // initialize fps and lcd and buttons 
  FPS_Setup();
  
}


void loop() {
  // serial and lcd for print the options
  Serial.println("A.Enroll    B.Search.    C.Delete");
  lcd.print("A.Enrol B.Search");
  lcd.setCursor(0, 1); // row zero and column 1 
  lcd.print("C.delete");

  // option = readnumber();
  int option = getOption();
  switch(option)
  {
    case 1 : 
    check_Enroll();
    break;
    case 2 : 
    FPS_Search();
    break;
     case 3 : 
    FPS_Delete();
    break;
    
  }
  delay(3000);
}


int getOption()
{
  while(1)
  {
    if(analogRead(left_btn) <= 10)
    {
      return 1;
    }
    else if(analogRead(middle_btn) <= 10)
    {
      return 2;
    }
    else if(analogRead(right_btn) <= 10)
    {
      return 3;
    }
    delay(100);
  }
}
