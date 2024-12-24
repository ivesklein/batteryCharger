// Define pins
#define BATi_PIN A0 // Analog input for battery 1 //P5
#define BATo_PIN A1 // Analog input for battery 2 //P2
#define FLOW_PIN 1  // Digital output for flow control

// Define constants for battery thresholds
const float OVERFLOW_VOLTAGE = 13.0; // Overflow voltage for bat1
const float NOMINAL_VOLTAGE = 12.4;  // Nominal voltage for bat2

// always connected
//const float OVERFLOW_VOLTAGE = 5.0; // Overflow voltage for bat1
//const float NOMINAL_VOLTAGE = 20.0;  // Nominal voltage for bat2

// Define voltage divider constants
//const float VOLTAGE_DIVIDER_RATIO = 0.2537; // Adjust based on resistor values
// f(x) = Ax + B
// x = analog input reaading
// y = calculated voltage
const float A_INTERi = 0.022322604532704; // linear regresion curve for input voltage
const float B_INTERi = -2.37950305210163; // linear regresion curve for input voltage

const float A_INTERo = 0.019877337464237; // linear regresion curve for output voltage
const float B_INTERo = -0.17288146309849; // linear regresion curve for output voltage

// Define constants for averaging
#define BUFFER_SIZE 64 // Number of readings to average

// Circular buffers for raw voltage readings
int batiRawBuffer[BUFFER_SIZE] = {0};
int batoRawBuffer[BUFFER_SIZE] = {0};
int bufferIndexi = 0; // Tracks the current position in the bat1 buffer
int bufferIndexo = 0; // Tracks the current position in the bat2 buffer

// Sum of buffer values for quick averaging
long batiRawSum = 0;
long batoRawSum = 0;

// Function to convert ADC reading to actual voltage
float convertRawToVoltagei(int raw) {
    return raw * A_INTERi + B_INTERi;
}
float convertRawToVoltageo(int raw) {
    return raw * A_INTERo + B_INTERo;
}

// Function to update the buffer and sum
void updateBuffer(int* buffer, long& sum, int& bufferIndex, int newValue) {
    // Subtract the value being replaced from the sum
    sum -= buffer[bufferIndex];
    // Replace it with the new value
    buffer[bufferIndex] = newValue;
    // Add the new value to the sum
    sum += newValue;

    // Increment buffer index (circular)
    bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;
}

// Function to calculate the average raw value
float getAverageRawVoltagei(long sum) {
    return convertRawToVoltagei(sum / (float)BUFFER_SIZE);
}
float getAverageRawVoltageo(long sum) {
    return convertRawToVoltageo(sum / (float)BUFFER_SIZE);
}


// Function to update Battery 1 readings
void updateBatteryi() {
    int batiRaw = analogRead(BATi_PIN); // Read raw analog value
    updateBuffer(batiRawBuffer, batiRawSum, bufferIndexi, batiRaw);
}

// Function to update Battery 2 readings
void updateBatteryo() {
    int batoRaw = analogRead(BATo_PIN); // Read raw analog value
    updateBuffer(batoRawBuffer, batoRawSum, bufferIndexo, batoRaw);
}

void setup() {
    pinMode(FLOW_PIN, OUTPUT); // Set FLOW_PIN as output
    digitalWrite(FLOW_PIN, LOW); // Ensure flow is initially disconnected
    //digitalWrite(FLOW_PIN, HIGH); // Open flow
}

void loop() {
    // Update battery readings using prime numbers as delay to minimize harmonics reading
    delay(7);
    updateBatteryi();
    updateBatteryo();    
    delay(11);
    updateBatteryi();
    updateBatteryo();
    delay(13);
    updateBatteryi();
    updateBatteryo();
    delay(17);
    updateBatteryi();
    updateBatteryo();
    delay(19);
    updateBatteryi();
    updateBatteryo();
    delay(23);
    updateBatteryi();
    updateBatteryo();
    delay(29);
    updateBatteryi();
    updateBatteryo();
    delay(31);
    updateBatteryi();
    updateBatteryo();
  
    // Calculate average voltages
    float batiAvgVoltage = getAverageRawVoltagei(batiRawSum);
    float batoAvgVoltage = getAverageRawVoltageo(batoRawSum);

    // Holy Logic
    if (batiAvgVoltage > OVERFLOW_VOLTAGE && batoAvgVoltage < NOMINAL_VOLTAGE) {
        digitalWrite(FLOW_PIN, HIGH); // Open flow
    } else {
        digitalWrite(FLOW_PIN, LOW); // Disconnect flow
    }

    delay(100);    
}
