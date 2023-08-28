#include <SoftwareSerial.h>
SoftwareSerial GSM_Serial(8, 9);
/*  The Software Serial Library allows the user to use GPIO pins for Serial communication.
 *  This is useful when 
 *    1. There are not enough RX, TX pins on the arduino, i.e., when you have many sensor communicating to the arduino
 *    2. When you don't want to interfere with the Serial communication between the computer and the arduino.
 */

/* ------- Global Variable Declaration ------- */
String text_buffer;
String number_buffer;
int number_count = 0;
bool text_flag = 0;
bool number_flag = 0;

/* ---------- GPIO Port Declaration ---------- */
int led = 13;

void setup()
{
   GSM_Serial.begin(9600);                  // Setting the baud rate of GSM Module  
   Serial.begin(9600);                      // Setting the baud rate of Serial Monitor (Arduino)
   pinMode(led, OUTPUT);                    // Setting the in-built led (Pin 13) as OUTPUT

   // Set the module to receive the message.
   GSM_Serial.println("AT+CNMI=2,2,0,0,0"); // AT Command for the GSM Module to receive a live SMS
}

void loop()
{
   if (GSM_Serial.available())        // to check if the data is being recieved from the GSM Module. it counts number of characters rcd
   {
      char data = GSM_Serial.read();  // store the received data
      
      // Please refer the message syntax present at the bottom, if requrired.
      if (data == '*')                // if starting byte is received,
         text_flag = 1;               // set the text_flag
         
      else if (data == '+')           // to check if the number is starting
         number_flag = 1;             // set the number flag
         
      else if (data == 26)            // if a "CTRL+Z" (ASCII - 26) is identified
      {
         number_buffer = "";          // Reset the number buffer
         number_count = 0;            // Reset the number counter
      }
         
      else if (data == '#')           // if the ending message byte is received,
      {
         text_flag = 0;               // clear the text_flag
         process_sms();               // print the received sms
      }

      /* ASCII Values
       * + - 43
       * 0 - 48
       * 9 - 57
       */
      /* Checking few constraints
       * >> Check the buffer length, don't overflow the buffer (not more than 12 number)
       * >> not to add anything other than numbers
       * Reset the flag if anything isn't satisfied
       */
       
      else if (number_flag && ( !(data == 43 || (data >= 48 && data <= 57)) || number_count >= 12)) //this will be off when the mobile number string reading is over
         number_flag = 0;

      else if (number_flag)           // if the number_flag is set
      {
         number_buffer += data;       // append the data to the buffer
         number_count += 1;           // increment the buffer counter
      }  
      
      else if (text_flag)             // to add the data to the sms buffer
         text_buffer += data;    
   }
}

// Function to process the sms string
void process_sms()
{
   if (text_buffer == "on")           // to switch on the led
   {
      digitalWrite(led, 1);
      send_reply("Switched on");      // Reply with a message saying "Swithced on"
   }
   else if (text_buffer == "off")     // to switch off the led
   {
      digitalWrite(led, 0);
      send_reply("Switched off");     // Reply with a message saying "Switched off"
   }
   text_buffer = "";
}

// Function to reply: Argument: Reply string
void send_reply(String reply_text)
{
   GSM_Serial.println("AT+CMGF=1");                         // AT Command to initialize the GSM Module to send a SMS
   delay(1000);
   GSM_Serial.println("AT+CMGS=\"+"+number_buffer+"\"\r");  // AT Command to set the number to send the SMS
   delay(1000);
   GSM_Serial.println(reply_text);                          // Sending the text
   delay(100);
   GSM_Serial.println((char)26);                            // To stop the text (CTRL + Z)
   delay(100);
   GSM_Serial.println("AT+CNMI=2,2,0,0,0");                 // AT Command for the GSM Module to receive a live SMS
}

/*
 * A MESSAGE SYNTAX:
  ------------------------------------------------
  +CMT: "+918660585878","","18/06/24,23:17:18+22" 
  *message#                                       
  ------------------------------------------------
 */
