#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#define size 16
#define keyPadPin A0
#define IDSize 2
char getOneKey();
uint8_t GetID();
int threholds[size] = {166, 146, 205, 339, 138, 191, 304, 132, 177, 272, 977, 710, 559, 460, 124, 246};
char keypad[size] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', '*', '#'};
char options_key[3] = {'A', 'B', 'C'};
char getOneKey()
{
  int flag = 0;
  char keyStr ;
  while(!flag)
  {
    for(int i=0; i<size; i++)
    {
      if((analogRead(keyPadPin) <= (threholds[i]+3)) && (analogRead(keyPadPin) >= (threholds[i]-3)))
      {
        Serial.print("option = ");
        Serial.println(keypad[i]);
        keyStr = keypad[i];
      }
    }
    for(int i=0; i<3; i++)
    {
      if(keyStr == options_key[i])
      {
          flag = 1;
          break;
      }  
    }
  }
  return keyStr;    
}

uint8_t GetID()
{

  char str[IDSize] = "";
  char temp;
  int count = 0;
  Serial.println("waiting for entering two number");
  while(count < IDSize){
    
    for(int i=0; i<size; i++)
    { 
      if((analogRead(keyPadPin) <= (threholds[i]+3)) && (analogRead(keyPadPin) >= (threholds[i]-3)))
      {
          str[count] = keypad[i];
          count++;

          while(analogRead(keyPadPin) < 1000)
          delay(100);
      }  
    }
  }
  uint8_t ID = atoi(str);  
  return ID;
}

#endif