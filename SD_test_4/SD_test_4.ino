// SD_test_4
// This version utilizes the Real Time Clock in addition to the SD card.
// This version integrates a TMP36 temperature sensor as well


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

// Define which Arduino pin is the chip select line for the SD card.
// On the Adafruit datalogging shield this value should be 10
const int chipSelect = 10;

SdFat sd; // instantiate the sd card object
SdFile myFile; // instantiate the file object that you will write to

// Declare initial name for output files written to SD card
char filename[] = "LOGGER00.CSV";

// Create a global temperature variable
float temperatureC = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Start the I2C library
  RTC.begin(); // Start the RTC library
  
  // Initialize SdFat or print a detailed error message and halt
  // Use half speed like the native library.
  // change to SPI_FULL_SPEED for more performance.
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();
  
  // Read a time off the Real Time Clock
  DateTime initTime = RTC.now();
  // Call the initFileName function to create a new output file
  initFileName(initTime);

}

void loop() {
  
  // Read the current time into a DateTime variable
  DateTime now = RTC.now(); 
  
  // Read the analog signal from the TMP36 sensor
  int A0value = analogRead(0); // read analog pin 0
  // convert the value into voltage
  float voltage = A0value * 5.0 / 1024.0;
  // convert voltage to temperature, subtracting off the 500mV offset
  temperatureC = (voltage - 0.5) * 100;

  writeToSD(now); // call the writeToSD function
  
  // show temperature on the serial monitor
  Serial.println(temperatureC); 
  delay(200);
}


//------------------------------------------------------------------------------
// initFileName - a function to create a filename for the SD card based
// on the 2-digit year, month, and day. The character array 'filename'
// was defined as a global variable at the top of the sketch
void initFileName(DateTime time1) {
	// Insert 2-digit year, month, and date into filename[] array
	// decade (2014-2000) = 14/10 = 1
	filename[0] = ((time1.year() - 2000) / 10) + '0'; 
	// year  (2014-2000) = 14%10 = 4
	filename[1] = ((time1.year() - 2000) % 10) + '0'; 
	if (time1.month() < 10) {
		filename[2] = '0';
		filename[3] = time1.month() + '0';
	} else if (time1.month() >= 10) {
		filename[2] = (time1.month() / 10) + '0';
		filename[3] = (time1.month() % 10) + '0';
	}
	if (time1.day() < 10) {
		filename[4] = '0';
		filename[5] = time1.day() + '0';
	} else if (time1.day() >= 10) {
		filename[4] = (time1.day() / 10) + '0';
		filename[5] = (time1.day() % 10) + '0';
	}
	// Next change the counter on the end of the filename
	// (digits 6+7) to increment count for files generated on
	// the same day. This shouldn't come into play
	// during a normal data run, but can be useful when 
	// troubleshooting.
	for (uint8_t i = 0; i < 100; i++) {
		filename[6] = i / 10 + '0';
		filename[7] = i % 10 + '0';
		filename[8] = '.';
		filename[9] = 'c';
		filename[10] = 's';
		filename[11] = 'v';
		if (!sd.exists(filename)) {
			// when sd.exists() returns false, this block
			// of code will be executed to open the file
			if (!myFile.open(filename, O_RDWR | O_CREAT | O_AT_END)) {
			}
			break; // Break out of the for loop when the
			// statement if(!myFile.exists())
			// is finally false (i.e. you found a new file name to use).
		} // end of if(!sd.exists())
	} // end of file-naming for loop
	// Write a header line to the new SD file
	myFile.println(F("DateTime,TempC"));
	// Update the file's creation date, modify date, and access date.
	myFile.timestamp(T_CREATE, time1.year(), time1.month(), time1.day(), 
			time1.hour(), time1.minute(), time1.second());
	myFile.timestamp(T_WRITE, time1.year(), time1.month(), time1.day(), 
			time1.hour(), time1.minute(), time1.second());
	myFile.timestamp(T_ACCESS, time1.year(), time1.month(), time1.day(), 
			time1.hour(), time1.minute(), time1.second());
	myFile.close();

} // end of initFileName function

//-----------------------------------------------------------------
// writeToSD function
// Opens the current file, appends data to the end of the file and
// closes the file. 
void writeToSD(DateTime time2) {
	// Reopen myFile. If opening fails, notify the user
	if (!myFile.isOpen()) {
		if (!myFile.open(filename, O_RDWR | O_CREAT | O_AT_END)) {
			sd.errorHalt("opening data.csv for write failed");
		}
	}
	myFile.print(time2.year(), DEC);
  	myFile.print(F("-"));
  	myFile.print(time2.month(), DEC);
  	myFile.print(F("-"));
  	myFile.print(time2.day(), DEC);
  	myFile.print(F(" "));
  	myFile.print(time2.hour(), DEC);
  	myFile.print(F(":"));
        if (time2.minute() < 10) {
           myFile.print(F("0")); // add a leading zero to minutes < 10 
        }
  	myFile.print(time2.minute(), DEC);
  	myFile.print(F(":"));
        if (time2.second() < 10) {
           myFile.print(F("0")); // add a leading zero to seconds < 10
        }
  	myFile.print(time2.second(), DEC);
  	myFile.print(F(","));
		
	// Write out temperature
	myFile.println(temperatureC);
	// If the seconds value is 30, update the file modified timestamp
	if (time2.second() % 30 == 0){
	    myFile.timestamp(T_WRITE, time2.year(),time2.month(),
                time2.day(),time2.hour(),time2.minute(),time2.second());
	}
        myFile.close(); // close the file when finished writing
}

