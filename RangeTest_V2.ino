#include <XBee.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
/*
This file is used to do range testing, it sends out packets to another xbee and then receives the packet from the xbee 
*/

// XBee's DOUT (TX) is connected to pin 8 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 9 (Arduino's Software TX)
SoftwareSerial serial1(0, 1); // RX, TX
int ledPin = 4;
XBee xbee=XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();
Rx64Response rx64 = Rx64Response();
File myFile;
//Simulate actual packet length
uint8_t payload[] ={'T','e','s','t','i','n','g',' ','P','a','y','l','o','a','d'}; 
Tx16Request tx = Tx16Request(0x0000, payload, sizeof(payload));
uint8_t option = 0;
uint8_t data = 0;
uint8_t rssi = 0;
 
void setup() 
{
  Serial.begin(9600);
  serial1.begin(9600);
  xbee.setSerial(Serial);
  pinMode(ledPin, OUTPUT);
  if (!SD.begin(8)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}
 
void loop() 
{
  xbee.readPacket(100);
  if (xbee.getResponse().isAvailable())
  {
    if(xbee.getResponse().getApiId() == RX_64_RESPONSE || xbee.getResponse().getApiId() == RX_16_RESPONSE)
    { 
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE) 
      {
        digitalWrite(ledPin,HIGH);
        Serial.println("Getting Signal Strength: ");
        xbee.getResponse().getRx16Response(rx16);
        rssi = rx16.getRssi();
        Serial.println(rssi);
        write_rssi_to_file(rssi);
        xbee.send(tx);
        Serial.println(' ');
      } 
      else 
      {
       Serial.println("64");
       xbee.getResponse().getRx64Response(rx64);
       rssi = rx64.getRssi();
       Serial.println(rssi);
      }
    }
  }
delay(1000);
}

void write_rssi_to_file(uint8_t signal_str){
  myFile = SD.open("rssi.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing Signal Strength...");
    myFile.println(signal_str);
    // close the file:
    myFile.close();
    digitalWrite(ledPin,LOW);
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening rssi_values.txt");
  }
}


