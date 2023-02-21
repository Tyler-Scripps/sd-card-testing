#include <SPI.h>
#include <SD.h>

File myFile;

unsigned long int startTime = 0;
unsigned long int endTime = 0;

const unsigned int numLines = 10000;

const int chipSelect = 45;

void setup() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  
  if (!SD.begin(BUILTIN_SDCARD)) {
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
  Serial.println(" microseconds");

  myFile.print("Time for ");
  myFile.print(numLines);
  myFile.print(" lines: ");
  myFile.print(endTime - startTime);
  myFile.println(" microseconds");

  myFile.close();

  digitalWrite(LED_BUILTIN, HIGH);  
}

void loop() {
  // put your main code here, to run repeatedly:

}
