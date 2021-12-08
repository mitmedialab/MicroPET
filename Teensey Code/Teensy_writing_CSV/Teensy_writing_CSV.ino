/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file   
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11, pin 7 on Teensy with audio board
 ** MISO - pin 12
 ** CLK - pin 13, pin 14 on Teensy with audio board
 ** CS - pin 4, pin 10 on Teensy with audio board
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
   
 */
 
#include <SD.h>
#include <SPI.h>

File myFile;
const int chipSelect = BUILTIN_SDCARD;

void setup()
{
  Serial.begin(9600);
   while (!Serial) {
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");


  // open the file. 
  myFile = SD.open("savefile.csv", FILE_WRITE);
  if (myFile) {
    Serial.print("Init Writing to file...");

    //Content
    myFile.println("testing 1, 2, 3.");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening file");
  }
  //Close the file
  
}

void loop()
{
  write_to_file();
  delay(1000);
}

void write_to_file()
{
    // open the file. 
  myFile = SD.open("savefile.csv", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to file...");

    //Content
    myFile.println("testing 1, 2, 3.");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening file");
  }

}
