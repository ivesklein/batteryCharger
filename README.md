# batteryCharger
Logic to charge a battery based on the voltages defined

# Protocols

## Program the Attiny85
### Preconfig
- In preferences inside the arduino IDE add: `https://raw.githubusercontent.com/digistump/arduino-boards-index/master/package_digistump_index.json`
- Install the `Digistump AVR Boards` in the boards manager
- Choose board: `Digistump AVR Boards > Digispark (Default - 16.5mhz)`
- Choose Programmer: `Micronucleus`
### Upload
Pin 5 is reset when pulled down so it will interfere with the upload
- Press Upload Code
- Connect usb and power up battery 1 at the same time

## Voltage Sensors Calibration
To calibrate the sensors:
- Add logging to the code to show the raw analog input values
- Feed the input voltage with sifferent voltages and write down the voltage and the input value
- Make a linear regresion and add the A and B values to the code
- To calibrate the output side change the code to set thr output always in HIGH
- Change the voltage on the voltage regulator and write down the voltage and input value for several voltages
-  Make a linear regresion and add the A and B values to the code

## To Calibrate the voltage regulator
- Change the code to output always HIGH
- Connect the input voltage
- Match the voltage regulator voltage with the output voltage
- Connect the output with a current meter
- Start increasing the voltage until it matches the maximum desired current
  - Decrease the current until it starts to change the current values and return to the desired value
- If increasing the voltage does not increase the current then increase the current screw
- Disconnect the output
- Change the voltage to the desired value
