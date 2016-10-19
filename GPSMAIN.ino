/*
  AUTO PROJECT (c) 2015 BY BONNY, HARVEY
*/
 
#include <SPI.h>
#include <SD.h>
#include <stdio.h>

File myFile;

String buffer;
char ab[80];

void setup() {

  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
  
  Serial2.begin(9600);
  Serial.begin(115200);
  Serial.println("OK");

  Serial.print("Initializing SD card...");
  pinMode(53, OUTPUT);
   
  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    return;
  }
  
  Serial.println("initialization done.");

}

void loop() {
  
  yield();  /* give other loops a chance to run */

  if (Serial2.available() > 0) {
    
    int cbRead;
    
    cbRead = Serial2.readBytesUntil('\n', ab, sizeof(ab));
    
    String buffer = ab;
      
    if (buffer.substring(0,6) == "$GPRMC" || buffer.substring(0,6) == "$GPVTG" || buffer.substring(0,6) == "$GPGGA") {
      
      //Serial.print(buffer.substring(1,buffer.length()-4)+"\r\n");
      String calcHEX = String(checkSum(buffer.substring(1,buffer.length()-4)), HEX);
      calcHEX.toUpperCase();
      calcHEX="00" + calcHEX;
      calcHEX=calcHEX.substring(calcHEX.length()-2,calcHEX.length());
      //Serial.print("Errechneter CRC:"+calcHEX+"[]\r\n");

      String hex = buffer.substring(buffer.length()-3,buffer.length()-1);
      //Serial.print("NMEA CRC:" + hex+"[]\r\n");
      
      if (hex==calcHEX) {
        //Serial.print("Buffer: "+buffer+"\r\n");
        myFile = SD.open("log6.txt", FILE_WRITE);
        myFile.println(buffer);
        myFile.close();
      }
    }    
       
    buffer="";
    //ab[0] = (char)0;
    memset(ab, 0, sizeof(ab));

  }
}

// calculate the checksum:
char checkSum(String theseChars) {
  char check = 0;
  // iterate over the string, XOR each byte with the total sum:
  for (int c = 0; c < theseChars.length(); c++) {
    check = char(check ^ theseChars.charAt(c));
  } 
  // return the result
  return check;
}

