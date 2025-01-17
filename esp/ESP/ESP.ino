#include "TRIGGER_WIFI.h"               /*Includes ESP8266WiFi.h and WiFiClientSecure.h, just have these two libraries downloaded before*/
#include "TRIGGER_GOOGLESHEETS.h"       /*Library file for Google Sheets, has to be used after Wi-Fi Client Secure declaration, here everything is in Trigger_WIFI.h, so using it after Trigger_WIFI.h*/ 

/**********Google Sheets Definations***********/
                      /*1. The Total no of column depends on how many value you have created in Script of Sheets;2. It has to be in order as per the rows decided in google sheets*/
String Sheets_GAS_ID = "AKfycbzwvxtOAsh-83xyCRGmQNd2yWbsMsPlTF3IKHBs3T0hMmz_51o6ejDyPteyzxwtelDS";                                         /*This is the Sheets GAS ID, you need to look for your sheets id*/

                                                               /*Here No_of_Parameters decides how many parameters you want to send it to Google Sheets at once, change it according to your needs*/
/*********************************************/

void setup() 
{
  Serial.begin(9600);
  while (!Serial);

  WIFI_Connect("Loay tamer","Loay2025");        /*Provide you Wi-Fi SSID and password to connect to Wi-Fi*/
  Google_Sheets_Init (Sheets_GAS_ID );         /*Sets the column name for Google Sheets, the GAS ID, and the No of Parameter we want to send*/        
}

void loop() 
{
  if(Serial.available()>0)
  {
    
    unsigned char id = Serial.read();
    Serial.println(id);
    Send_Data(id);         /*1. This function accepts multiple float parameter, here No_of_Parameters decides how many parameters you want to send to Google Sheets; 2. The values sent should be in order as per the column in Google Sheets*/

    Serial.println();
    delay(5000);
  }                           /*Demo values that has to be sent to google sheets, you can use sensor values*/

                                       /*10 Sec Delay, Here 10 second delay is just used so that we can see the data pushed to sheets one by one
                                                        There is nothing like a minimum delay between two data push*/
}
