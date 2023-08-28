#include <SoftwareSerial.h>
SoftwareSerial GSM_Serial(8, 9);

String text_buffer;
bool text_flag = 0;

int led = 13;

void setup()
{
   GSM_Serial.begin(9600);
   Serial.begin(9600);
   GSM_Serial.println("AT+CNMI=2,2,0,0,0");
   pinMode(led, OUTPUT);
}

void loop()
{
   if (GSM_Serial.available())        // to check if the data is being recieved from the GSM Module
   {
      char data = GSM_Serial.read();  // store the received data
      Serial.print(data);
   }
}
