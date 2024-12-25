//#include <DigiKeyboard.h>

// Define pins
#define BATi_PIN A0 // Analog input for battery 1 //P5
#define BATo_PIN A1 // Analog input for battery 2 //P2
#define FLOW_PIN 1  // Digital output for flow control

// Define constants for battery thresholds
const float OVERFLOW_VOLTAGE = 12.8; // Overflow voltage for bat1
const float LOWER_VOLTAGE = 10.0;
//const float NOMINAL_VOLTAGE = 12.4;  // Nominal voltage for bat2

// always connected
//const float OVERFLOW_VOLTAGE = 5.0; // Overflow voltage for bat1
//const float NOMINAL_VOLTAGE = 20.0;  // Nominal voltage for bat2


// Define voltage divider constants
//const float VOLTAGE_DIVIDER_RATIO = 0.2537; // Adjust based on resistor values
const float A_INTERi = 0.022322604532704;
const float B_INTERi = -2.37950305210163;

const float A_INTERo = 0.019877337464237;
const float B_INTERo = -0.17288146309849;

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

// TIMER VARIABLES
unsigned long lastCheckTime = 0;     // Last time the value was checked
unsigned long highStartTime = 0;     // Time when the pin was set HIGH
const unsigned long CHECK_INTERVAL = 3600000*6;  // 1 hour in milliseconds
const unsigned long HIGH_DURATION = 900000;    // 15 minutes in milliseconds

bool pinHigh = false; // Tracks whether the pin is HIGH

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
    // Update battery readings
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

    // Send voltages to computer via keyboard
    //DigiKeyboard.sendKeyStroke(0); // Required for DigiKeyboard to initialize properly
    //DigiKeyboard.print("Bat1: ");
    //DigiKeyboard.print(batiAvgVoltage);
    //DigiKeyboard.print(" V, Bat2: ");
    //DigiKeyboard.print(batoAvgVoltage);
    //DigiKeyboard.println(" V");

    //delay(500);

    // Uncomment this section to control flow based on conditions
    
    if (batiAvgVoltage > OVERFLOW_VOLTAGE) {
        digitalWrite(FLOW_PIN, HIGH); // Open flow
        pinHigh = true;
    } else {
        coldCharging();
    }

    delay(100);
    
}


void coldCharging() {
  unsigned long currentMillis = millis();

  // Check if it's time to check the VALUE
  if (currentMillis - lastCheckTime >= CHECK_INTERVAL) {
    lastCheckTime = currentMillis;

    // Read the VALUE (adjust depending on your input method)
    float value = getAverageRawVoltagei(batiRawSum);

    // If the VALUE is higher than the LIMIT, set the pin HIGH
    if (value > LOWER_VOLTAGE) {
      digitalWrite(FLOW_PIN, HIGH); // Connect flow
      highStartTime = currentMillis;
      pinHigh = true;
    }
  }

  // Check if the pin should be turned off
  if (pinHigh && (currentMillis - highStartTime >= HIGH_DURATION)) {
    digitalWrite(FLOW_PIN, LOW); // Disconnect flow
    pinHigh = false;
  }
}
