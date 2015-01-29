// SD_test_2
// This version utilizes the Real Time Clock in addition to the SD card.

// Define which Arduino pin is the chip select line for the SD card.
// On the Adafruit datalogging shield this value should be 10
const int chipSelect = 10;

#include <SdFat.h>
#include <SPI.h> // required for RTClib library to work
#include <Wire.h> // required for RTClib library to work
#include <RTClib.h> // https://github.com/mizraith/RTClib
// If you use the DS3231, include the line below, otherwise comment out
#include <RTC_DS3231.h> // From https://github.com/mizraith/RTClib
// If you use the DS1307, include the line below, otherwise comment out
//#include <RTC_DS1307.h> // From https://github.com/mizraith/RTClib

RTC_DS3231 RTC; // uncomment if using the DS3231
//RTC_DS1307 RTC; // uncomment if using the DS1307

SdFat sd;
SdFile myFile;

byte counter = 0; // used to keep count of cycles

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Start the I2C library
  RTC.begin(); // Start the RTC library
  
  // Initialize SdFat or print a detailed error message and halt
  // Use half speed like the native library.
  // change to SPI_FULL_SPEED for more performance.
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();

}

void loop() {
  
  // Read the current time into a DateTime variable
  DateTime now = RTC.now(); 
  
  // open the file for write at end like the Native SD library
  if (!myFile.open("data.csv", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening data.csv for write failed");
  }

  // Write the DateTime value
  myFile.print(now.year(),DEC);
  myFile.print("-");
  myFile.print(now.month(),DEC);
  myFile.print("-");
  myFile.print(now.day(),DEC);
  myFile.print(" ");
  myFile.print(now.hour(),DEC);
  myFile.print(":");
  myFile.print(now.minute(),DEC);
  myFile.print(":");
  myFile.print(now.second(),DEC);
  myFile.print(","); // comma denotes new column in file
  myFile.println(counter); // write counter value and include newline
  myFile.close(); // close the file when finished writing
  
  Serial.println(counter); // show progress on the serial monitor
  counter = counter++; // increment loop counter
  delay(100);
}
