/* Example for analogContinuousRead
*  It measures continuously the voltage on pin A9,
*  Write v and press enter on the serial console to get the value
*  Write c and press enter on the serial console to check that the conversion is taking place,
*  Write t to check if the voltage agrees with the comparison in the setup()
*  Write s to stop the conversion, you can restart it writing r.
*/

#include <ADC.h>
#include <ADC_util.h>

const int readPin = A0; // ADC0
ADC *adc = new ADC(); // adc object

uint16_t analogValue;
const unsigned long numReads = 5;
volatile unsigned long currentReads = 0;
unsigned long startTime = 0;
unsigned long endTime = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(readPin, INPUT);

  Serial.begin(115200);

  while (!Serial) {
  }
  Serial.println("continuous read test");

  ///// ADC0 ////
  // reference can be ADC_REFERENCE::REF_3V3, ADC_REFERENCE::REF_1V2 (not for Teensy LC) or ADC_REFERENCE::REF_EXT.
  //adc->adc0->setReference(ADC_REFERENCE::REF_1V2); // change all 3.3 to 1.2 if you change the reference to 1V2

  adc->adc0->setAveraging(16); // set number of averages
  adc->adc0->setResolution(16); // set bits of resolution

  // it can be any of the ADC_CONVERSION_SPEED enum: VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED_16BITS, HIGH_SPEED or VERY_HIGH_SPEED
  // see the documentation for more information
  // additionally the conversion speed can also be ADACK_2_4, ADACK_4_0, ADACK_5_2 and ADACK_6_2,
  // where the numbers are the frequency of the ADC clock in MHz and are independent on the bus speed.
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_LOW_SPEED); // change the conversion speed
  // it can be any of the ADC_MED_SPEED enum: VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED or VERY_HIGH_SPEED
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED); // change the sampling speed

  // always call the compare functions after changing the resolution!
  // adc->adc0->enableCompare(1.0/3.3*adc->getMaxValue(), 0); // measurement will be ready if value < 1.0V
  // adc->adc0->enableCompareRange(1.0*adc->adc0->getMaxValue()/3.3, 2.0*adc->adc0->getMaxValue()/3.3, 0, 1); // ready if value lies out of [1.0,2.0] V

  // If you enable interrupts, notice that the isr will read the result, so that isComplete() will return false (most of the time)
  adc->adc0->enableInterrupts(adc0_isr);

  Serial.println("adc is set up");
  digitalWriteFast(LED_BUILTIN, LOW);
  delay(500);

  startTime = micros();
  adc->adc0->startContinuous(readPin);
}

void loop() {
  Serial.println("test");
  Serial.println(currentReads);
  
  if (currentReads >= numReads) {
    noInterrupts();
    endTime = micros();

    Serial.print("Time for ");
    Serial.print(numReads);
    Serial.print(" reads: ");
    Serial.print(endTime - startTime);
    Serial.println(" microseconds");

    while(true) {}
  }
  if(adc->adc0->fail_flag != ADC_ERROR::CLEAR) {
    Serial.print("ADC0: "); Serial.println(getStringADCError(adc->adc0->fail_flag));
  }
  delay(10);
  digitalWriteFast(LED_BUILTIN, HIGH);
}

void adc0_isr(void) {
  // digitalWriteFast(LED_BUILTIN, HIGH);
  analogValue = (uint16_t)adc->adc0->analogReadContinuous();
  currentReads++;
  if (currentReads >= numReads) {
    adc->adc0->stopContinuous();
  }
}
