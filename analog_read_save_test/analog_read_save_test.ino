#include <ADC.h>
#include <ADC_util.h>
#include <SPI.h>
#include <SD.h>

#define DEBUG true
#define BUTTON_PIN 33

File myFile;

const int readPin = A0;
ADC *adc = new ADC(); // adc object


unsigned long lastTime = 0;

const unsigned long numReads = 100000;
unsigned long currentReads = 0;

unsigned long endTime;

uint16_t inValue;

unsigned long startTime = 0;
bool recording = false;
bool doneRecording = false;
uint16_t data[numReads] = { 0 };

// temporary string to store a single line
String tempStr;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if(DEBUG) {
    while(!Serial){}
  }

  adc->adc0->setAveraging(1); // set number of averages
  adc->adc0->setResolution(16); // set bits of resolution

  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED); // change the conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED); // change the sampling speed


  digitalWrite(LED_BUILTIN, HIGH);
  if(DEBUG) {
    Serial.begin(115200);
    Serial.println("analog read test");
  }

  if (!SD.begin(BUILTIN_SDCARD)) {
    if(DEBUG) {
      Serial.println("initialization failed!");
    }
    while (1);
  }

  pinMode(A0, INPUT);

  startTime = micros();
  for (unsigned long i = 0; i < numReads; i++) {
    inValue = adc->adc0->analogRead(readPin);
  }
  endTime = micros();

  if(DEBUG) {
    Serial.print("Time for ");
    Serial.print(numReads);
    Serial.print(" reads: ");
    Serial.print(endTime - startTime);
    Serial.println(" microseconds");
  }
}

void loop() {
  // if not recording AND button pressed AND it has been a second since last press start recording
  if(!recording && !digitalRead(BUTTON_PIN) && micros() - startTime > 1000000) {
    if (DEBUG) {
      Serial.println("Starting measurements");
    }
    startTime = micros();
    recording = true;
    doneRecording = false;
    currentReads = 0;
    lastTime = micros();
  }

  // if recording and it has been a microsecond
  if (recording && micros() > lastTime) {
    lastTime = micros();
    data[currentReads] = adc->adc0->analogRead(readPin);
    currentReads++;
    if (currentReads >= numReads) {
      if (DEBUG) {
        Serial.println("Ending measurements");
      }
      recording = false;
      doneRecording = true;

      myFile = SD.open("test.txt", FILE_WRITE);
      // iterate through data array to save to sd card
      for (unsigned long i = 0; i < numReads; i++) {
        tempStr = String(i) + ',' + String(data[i]);
        myFile.println(tempStr);
      }
      myFile.close();
    }
  }
}
