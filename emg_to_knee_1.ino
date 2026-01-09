/*
  MyoWare 2.0 Signal Grapher

  This sketch reads the analog output from a MyoWare 2.0 Muscle Sensor
  connected to analog pin A0 and sends the data to the computer.
  
  You can view the data as a real-time graph using the Arduino IDE's
  Serial Plotter (Tools > Serial Plotter).

  Hardware Connections:
  - MyoWare Sensor -> Link Shield -> TRS Cable -> Arduino Shield (A0)
  - Arduino Shield stacked on SparkFun RedBoard Plus
  - RedBoard Plus connected to computer via USB
*/

void setup() {
  // Initialize serial communication at 115200 bits per second.
  // This is the speed at which data is sent to the computer.
  Serial.begin(115200);

  // The following line is optional but helpful. It waits for you to open
  // the Serial Monitor or Plotter before the sketch starts sending data.
  while (!Serial); 
  
  Serial.println("MyoWare 2.0 Signal Grapher Initialized");
}

void loop() {  
  // Read the raw analog value from the sensor on pin A0.
  // This will be a number between 0 (0 volts) and 1023 (5 volts).
  int sensorValue = analogRead(A0);

  // Print the sensor value to the serial port. The Serial Plotter will
  // use this stream of numbers to draw the graph.
  Serial.println(sensorValue);

  // A small delay is added to prevent overwhelming the serial port
  // and to make the graph easier to read.
  delay(50);
}