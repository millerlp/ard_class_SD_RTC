You need the real time clock library and SdFat library.
* RTClib: https://github.com/mizraith/RTClib
* SdFat: https://github.com/greiman/SdFat

Download the ZIP files from each of those pages. 

Extract the RTClib_master.zip file, rename the folder inside from RTClib-master to RTClib.
Place the folder RTClib inside your Arduino/libraries/ directory.

Extract the SdFat-master.zip file, open the SdFat-master folder. Inside find the SdFat folder,
copy that to your Arduino/libraries directory. 

------------------------------
Download the ZIP file for this repository. Extract the zip file, rename the folder inside
from ard_class_SD_RTC-master to ard_class_SD_RTC, and place that folder inside your Arduino/ directory.

Restart the Arduino program so that both new libraries and the sketches can be found by the software. 

-----------------------------
On the Uno, the SPI bus pins used to connect to a SD card adapter are:
* MOSI	11	MOSI corresponds to DI pin on many SD card adapters
* MISO	12	MISO corresponds to DO pin on many SD card adapters
* SCK	13	SCK corresponds to CLK pin on some SD card adapters
* CS	10	CS should match the CS pins on SD card adapters

On the Uno, to talk to a DS1307 or DS3231 Real Time Clock, use the SDA and SCL pins
located near the reset button and USB port. Alternately, you can use 
Analog pins 4 (SDA) and 5 (SCL)

------------------------------
Formatting utility for SD cards: 
https://www.sdcard.org/downloads/formatter_4/ 
