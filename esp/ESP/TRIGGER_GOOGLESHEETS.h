#ifndef _TRIGGER_GOOGLESHEETS_H_
#define _TRIGGER_GOOGLESHEETS_H_

#include <stdarg.h>
#include <string.h>

void sheets_initialization();
void Send_Data();
void Google_Sheets_Init( String sheets_gas_id);

String url;
String ID = "ID";


const char* host = "script.google.com";
const int httpsPort = 443;


String GAS_ID;

void Google_Sheets_Init( String sheets_gas_id)
{
	GAS_ID = sheets_gas_id;
}

void Send_Data(unsigned char data)
{
	sheets_initialization();
    
	String url = "/macros/s/" + GAS_ID + "/exec?";
  // may be need to cast data to string
  url = url+ID+"="+data;
		  
	
  
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  #ifdef Debug_Serial_Mon
  Serial.println("request sent");
  #endif
  while (client.connected()) 
  {
	  String line = client.readStringUntil('\n');
	  if (line == "\r") 
	  {
		#ifdef Debug_Serial_Mon  
		Serial.println("headers received");
		#endif
		break;
	  }
  }
  String line = client.readStringUntil('\n');
  
  if (line.startsWith("{\"state\":\"success\"")) 
  {
	#ifdef Debug_Serial_Mon
	Serial.println("esp8266/Arduino CI successfull!");
	#endif
  } 
  else 
  {
	#ifdef Debug_Serial_Mon  
	Serial.println("esp8266/Arduino CI has failed");
	#endif
  }
  
  #ifdef Debug_Serial_Mon
	  Serial.println("reply was:");
	  Serial.println("==========");
	  Serial.println(line);
	  Serial.println("==========");
	  Serial.println("closing connection");
  #endif
	
}
void sheets_initialization()
{
	client.setInsecure(); 
  
  #ifdef Debug_Serial_Mon
	  Serial.print("connecting to ");
	  Serial.println(host);
  #endif
  
  if (!client.connect(host, httpsPort)) 
  {
    #ifdef Debug_Serial_Mon
	Serial.println("connection failed");
	#endif
    return;
  }

}
#endif
