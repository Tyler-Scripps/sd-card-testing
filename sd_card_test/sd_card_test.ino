#include <SPI.h>
#include <SD.h>

File myFile;

unsigned long int startTime = 0;
unsigned long int endTime = 0;

unsigned int numLines = 10000;

void setup() {
  Serial.begin(115200);

  SD.begin(BUILTIN_SDCARD);
  
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }

  myFile = SD.open("test.txt", FILE_WRITE);

  startTime = micros();
  for (unsigned int i = 0; i < numLines; i++) {
    myFile.println("0123456789abdef");
  }
  endTime = micros();

  Serial.print("Time for ");
  Serial.print(numLines);
  Serial.print(" lines: ");
  Serial.print(endTime - startTime);
}

void loop() {
  // put your main code here, to run repeatedly:

}
