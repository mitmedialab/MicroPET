# Teensy Launch Code
The code provided is for a MicroPET payload using a Teensyduino microcontroller. The code includes several libraries and defines multiple functions and variables for controlling and managing the payload's experiments and sensors.

The code starts by including various libraries such as TimeLib, TimeAlarms, Wire, SPI, Adafruit_Sensor, Adafruit_BME280, Adafruit_AS726x, Adafruit_MCP23X17, and RTClib. These libraries are used to interface with different sensors and modules.

Next, there are function prototypes for 26 different experiment days, numbered from day_1 to day_26. These functions are used to execute specific experiments on each day of the payload's operation.

The experimentArray is an array of function pointers that stores the addresses of each experiment function. This allows the code to easily access and execute the desired experiment based on the current day.

The code then defines various pin mappings and addresses for different components connected to the Teensyduino, such as the base board connectors and MCP23X17 GPIO expanders.

Further in the code, there are implementations of functions for controlling motors, switching IO ports, moving liquids between different chambers, switching collection bags, taking sensor data, initializing the SD card, and saving data to files.

The loop function of the code waits for a serial start command from the user to begin the payload's experiments. Once the command is received, the code calls the day_1 function to start the first day's experiment. After each experiment, the code sets the next alarm to schedule the next experiment based on the defined delay between experiments.

There is also a function for recovering the system after an unscheduled power loss. This function checks if the system was interrupted during an experiment or if it was off for a longer time than the experiment interval. If it can resume the experiment, it sets the next alarm and continues from where it left off.

The rest of the code includes functions for reading data from sensors, saving sensor data to a file, and initializing the SD card and experimental log file. Overall, the code provides the functionality for controlling and managing the MicroPET payload, executing different experiments, collecting sensor data, and saving data to files.

### Base Board Connection
The base board connections are as follows:
- E1VA + E1PA - J10
- E1VB + E1PB - J09
- E2VA + E2PA - J12
- E2VB + E2PB - J11
- The valve board connector J1-6 are the same on the base board

### Dependencies
The code depends on the following libraries:
- TimeLib.h
- TimeAlarms.h
- Wire.h
- SPI.h
- Adafruit_Sensor.h
- Adafruit_BME280.h
- Adafruit_AS726x.h
- Adafruit_MCP23X17.h
- RTClib.h
- SD.h

### Experiment Protocols
The code defines the following experiment protocols:
- day_1()
- day_2()
- day_3()
- day_4()
- ...
- day_26()

The experiment functions are stored in an array called `experimentArray`, which is of size `TOTAL_EXPERIMENTS`.

### Global Variables
- `rtc`: RTC_PCF8523 object to interface with the RTC module.
- `experimentIndex`: uint16_t variable to keep track of the current experiment index.
- `delayTime`: unsigned long variable to store the delay time between experiments.
- `sensorValues`: uint16_t array to store the sensor values read from the AS726x sensor.
- `myFile`: File object to store data in a file.
- `expLogFile`: File object to store experimental log data in a file.
- `systemStateStructVar`: systemStateStruct object to store the system state.

### Main Functions
- `setup()`: Initializes the program by initializing variables, setting up the RTC, SD card, and IO ports, and setting the first alarm.
- `loop()`: Loops indefinitely, checking for a start command via serial communication and executing the corresponding experiment.
- `rtcInit()`: Initializes the RTC module by checking for RTC availability, adjusting the time if necessary, and calibrating the RTC.
- `grabStateFromSD()`: Reads the system state from the SD card if it exists, and returns true if successful.
- `saveStateToSD()`: Saves the system state to the SD card.
- `checkForStartSerialCommand()`: Checks for a start command via serial communication and returns true if it is received.
- `recoverSystemStart()`: Resumes system operation after an unscheduled power loss by checking for hard faults and setting up timers.
- `setNextAlarm()`: Sets the next alarm based on the current experiment index.
- `moveLiquid()`: Moves liquid from one location to another based on the experiment, origin, target, and liquid volume.
- `switchCollection()`: Switches the collection bag for a particular experiment.
- `Taking_Sensor_Data()`: Takes sensor data by reading temperature and spectral values from the BME280 and AS726x sensors.
- `saveExperimentalLog()`: Saves an experimental log message to the experiment log file.
- `init_card()`: Initializes the SD card by checking for availability, opening the save file, and writing the header if necessary.
- `initExpLogFile()`: Initializes the experiment log file by opening it and writing the header.
- `motorSensorBrdCtrl()`: Controls the motors on the sensor board based on the motor number and direction.

### Helper Functions
- `digitalClockDisplay()`: Displays the current time on the serial monitor.
- `printDigits()`: Prints digits with leading zeroes if necessary.
- `tmConvert_t()`: Converts a specified date and time to a time_t format.
