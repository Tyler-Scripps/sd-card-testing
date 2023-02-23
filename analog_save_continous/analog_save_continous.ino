#include <ADC.h>
#include <ADC_util.h>
#include <SPI.h>
#include <SD.h>

#define DEBUG true
#define BUTTON_PIN 33
#define DIGITAL_IN_PIN

File myFile;

const int readPin = A0;
ADC *adc = new ADC(); // adc object


unsigned long lastTime = 0;

const unsigned long numReads = 500000;
unsigned long currentReads = 0;

unsigned long endTime;

uint16_t inValue;

unsigned long startTime = 0;
unsigned long startTimeNanos = 0;
bool recording = false;
bool doneRecording = false;
EXTMEM uint16_t data[numReads] = { 0 };
EXTMEM uint16_t times[numReads] = { 0 };
EXTMEM bool outStates[numReads] = { 0 };

// elapsedMicros sinceRecord;
unsigned long deltaTime = 0;

unsigned int longestTime = 0;

// temporary string to store a single line
String tempStr;

uint32_t nanos() {
  return 1.667 * ARM_DWT_CYCCNT;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if(DEBUG) {
    while(!Serial){}
  }

  adc->adc0->setAveraging(0); // set number of averages
  adc->adc0->setResolution(12); // set bits of resolution

  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // change the conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED); // change the sampling speed

  adc->adc0->startContinuous(uint8_t pin)


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
    startTimeNanos = nanos();
    recording = true;
    doneRecording = false;
    currentReads = 0;
    lastTime = nanos();
    adc->adc0->enableInterrupts(adc0_isr);
    adc->adc0->startContinuous(readPin);
  }

  if (doneRecording) {
    endTime = micros();
    if (DEBUG) {
      Serial.println("Ending measurements");
    }
    recording = false;
    String fileName = "test1.csv";
    int fileIter = 1;
    while(SD.exists(fileName.c_str())) {
      fileIter++;
      fileName = "test" + String(fileIter) + ".csv";
    }

    myFile = SD.open(fileName.c_str(), FILE_WRITE);
    // iterate through data array to save to sd card
    for (unsigned long i = 0; i < numReads; i++) {
      tempStr = String(times[i]) + ',' + String(outStates[i]) + ',' + String(data[i]);
      myFile.println(tempStr);
    }
    myFile.print("total time (us): ");
    myFile.println(endTime - startTime);
    myFile.print("Longest measure time: ");
    myFile.println(longestTime);
    myFile.close();
    if(DEBUG) {
      Serial.print("Wrote to: ");
      Serial.println(fileName);
      Serial.print("Total measure time: ");
      Serial.println(endTime - startTime);
      Serial.print("Longest measure time: ");
      Serial.println(longestTime);
      Serial.print("Number of long cycles: ");
      Serial.println(numLongReads);
    }
    doneRecording = false;    
  }

  if (adc->adc0->fail_flag != ADC_ERROR::CLEAR) {
    if(DEBUG) {
      Serial.print("ADC: ");
      Serial.println(getStringADCError(adc->adc0->fail_flag));
    }
  }
}


void adc0_isr(void) {
  // digitalWriteFast(LED_BUILTIN, HIGH);
  data[currentReads] = (uint16_t)adc->adc0->analogReadContinuous();
  times[currentReads] = nanos() - startTime;
  outStates[currentReads] = digitalRead(DIGITAL_IN_PIN);
  currentReads++;
  // Serial.println(currentReads);
  if (currentReads >= numReads) {
    adc->adc0->stopContinuous();
    adc->adc0->disableInterrupts();
    doneRecording = true;
  }
}