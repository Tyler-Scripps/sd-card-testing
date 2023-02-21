#include <ADC.h>
#include <ADC_util.h>

const int readPin = A0;
ADC *adc = new ADC(); // adc object


unsigned long startTime = 0;
unsigned long endTime = 0;

const unsigned long numReads = 500000;

uint16_t inValue;

void setup() {
  while (!Serial) {
  }
  adc->adc0->setAveraging(1); // set number of averages
  adc->adc0->setResolution(16); // set bits of resolution

  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED); // change the conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED); // change the sampling speed


  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  Serial.println("analog read test");

  pinMode(A0, INPUT);

  startTime = micros();
  for (unsigned long i = 0; i < numReads; i++) {
    inValue = adc->adc0->analogRead(readPin);
  }
  endTime = micros();

  Serial.print("Time for ");
  Serial.print(numReads);
  Serial.print(" reads: ");
  Serial.print(endTime - startTime);
  Serial.println(" microseconds");

}

void loop() {
  // put your main code here, to run repeatedly:

}
