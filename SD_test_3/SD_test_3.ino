// SD_test_3
// This version utilizes the Real Time Clock in addition to the SD card.
// This version integrates a TMP36 temperature sensor as well

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
  
  // Read the analog signal from the TMP36 sensor
  int A0value = analogRead(0); // read analog pin 0
  // convert the value into voltage
  float voltage = A0value * 5.0 / 1024.0;
  // convert voltage to temperature, subtracting off the 500mV offset
  float temperatureC = (voltage - 0.5) * 100;
  
  
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
  myFile.println(temperatureC); // write counter value and include newline
  myFile.close(); // close the file when finished writing
  
  // show temperature on the serial monitor
  Serial.println(temperatureC); 
  
  delay(100);
}
