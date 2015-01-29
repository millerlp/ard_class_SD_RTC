// Basic example of using the SdFat library to create and write to a 
// file, and then open and read that file back. 

#include <SdFat.h> // include the SdFat library

SdFat sd; // instantiate the sd card object
SdFile myFile; // instantiate the file object that you will write to

// Define which Arduino pin is the chip select line for the SD card.
// On the Adafruit datalogging shield this value should be 10
const int chipSelect = 10;

void setup() {
 Serial.begin(9600);
 Serial.println("Type any character to start"); 
 while (Serial.read() <= 0) {}  // Arduino waits for your response

  // Initialize SdFat or print a detailed error message and halt
  // Use half speed like the native library.
  // change to SPI_FULL_SPEED for more performance.
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();

  // open the file for write at end like the Native SD library
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
  }
  // if the file opened okay, write to it:
  Serial.print("Writing to test.txt...");
  myFile.println("testing 1, 2, 3.");
  Serial.println("testing 1, 2, 3."); // mirror output to Serial port

  // close the file:
  myFile.close();
  Serial.println("done.");
  
  delay(1000); 
  
  // re-open the file for reading:
  if (!myFile.open("test.txt", O_READ)) {
    sd.errorHalt("opening test.txt for read failed");
  }
  Serial.println("Opening and reading test.txt:");

  // read from the file until there's nothing else in it:
  int data;
  while ((data = myFile.read()) >= 0) Serial.write(data);
  // close the file:
  myFile.close();
}


void loop() {
  // nothing happens after setup
}
