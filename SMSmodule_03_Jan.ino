#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,2);  //Rx,Tx
char bufferdata[64];
int analogvalue=0,flag=0,i=0,k=0,waitingtime=0,x=0;
int lessspeed=0,medspeed=0,highspeed=0;
char phonenumber[15]="9597473478";
void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600); 
  pinMode(13, OUTPUT);// Setting the baud rate of Serial Monitor (Arduino)
  receivesms();
  delay(2000);  
}
void loop()
{  
  if(waitingtime<1000)
  {
    displaydetail();
    waitingtime++;
    delay(50);
  }
  else
  {
    checkspeed();  // wait for a second
    waitingtime=0;
    delay(1000);
  }
     
}
void sendsms(int analogvalue)
{
    mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(200);  // Delay of 1000 milli seconds or 1 second

    mySerial.print("AT+CMGS=\""); // send the SMS number
    while(x<10)
    {
      mySerial.print(phonenumber[x]);
      x++;
    }
    mySerial.print("\"\r");    
    delay(200);
    mySerial.println(analogvalue);// The SMS text you want to send
    delay(200);
    mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(200);
    Serial.println("--SMS SEND--");
    i=0;
    k=0;
    flag=0;
    x=0;
}
void receivesms()
{
  mySerial.print("AT+CMGF=1\r");  // set SMS mode to text
  delay(200);
  mySerial.print("AT+CNMI=2,2,0,0,0\r"); 
  delay(200);
  mySerial.print("AT+CMGR=1\r");
  delay(200); 
}
void displaydetail()
{
  char incoming_char=0;
  
  if(mySerial.available() >0)
  {
    incoming_char=(char)mySerial.read(); //Get the character from the cellular serial port.
    Serial.print(incoming_char); //Print the incoming character to the terminal.
       if(incoming_char=='#')
     {
       flag=1;
     }
     if( flag==1)
     {
               bufferdata[i]=incoming_char; 
               if(i>1)
               {
                phonenumber[k]=bufferdata[i];
                k++;
               }
            i++;    
      }
}

}
void checkspeed()
{
  //Serial.print("Phone Number:");
 // Serial.println(phonenumber);
  analogvalue = analogRead(13);

  if(analogvalue>300)
  {
    if(analogvalue<350)
    {
      if(lessspeed<1){
       Serial.println("1 times");
       sendsms(analogvalue);
       delay(1000);
       lessspeed=lessspeed+1;
     }
    }
   else if((analogvalue>350)&&(analogvalue<450))
   {
     if(medspeed<2){
     Serial.println("2 times");
     sendsms(analogvalue); 
     medspeed=medspeed+1;
     delay(1000);
   }
   }
   else if(analogvalue>450)
   {
    if(highspeed<3){
    Serial.println("3 times");
    sendsms(analogvalue);
    highspeed=highspeed+1;
    delay(1000);
   }
   }
  } 
}
