# batteryCharger
Logic to charge a battery based on the voltages defined

# Protocols
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
