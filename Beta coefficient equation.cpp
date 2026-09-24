// NTC-MF52AT Thermistor Reading Code for Arduino Uno R3
const int THERMISTOR_PIN = A0;         // Pin connected to the voltage divider
const float SERIES_RESISTOR = 10000.0;  // The fixed pull-up resistor value (10k ohms)
const float NOMINAL_RESISTANCE = 10000.0; // Thermistor resistance at nominal temp (25C)
const float NOMINAL_TEMPERATURE = 25.0; // Nominal temp in Celsius
const float BETA_COEFFICIENT = 3950.0;  // Standard MF52AT Beta value

void setup() {
  Serial.begin(9600);
  pinMode(THERMISTOR_PIN, INPUT);
}

void loop() {
  int adcValue = analogRead(THERMISTOR_PIN);
  
  // Prevent division by zero if reading fluctuates wildly
  if (adcValue == 1023) adcValue = 1022;
  if (adcValue == 0) adcValue = 1;

  // 1. Calculate the current resistance of the NTC thermistor
  // Formula derived from the voltage divider rule: Vout = Vcc * (R_ntc / (R_pullup + R_ntc))
  float ntcResistance = SERIES_RESISTOR / ((1023.0 / adcValue) - 1.0);

  // 2. Apply Beta Parameter Equation to find absolute temperature in Kelvin
  float steinhart;
  steinhart = ntcResistance / NOMINAL_RESISTANCE;     // (R / Ro)
  steinhart = log(steinhart);                         // ln(R / Ro)
  steinhart /= BETA_COEFFICIENT;                      // 1/B * ln(R / Ro)
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);  // + (1 / To)
  steinhart = 1.0 / steinhart;                        // Invert to get Kelvin

  // 3. Convert absolute Kelvin to user-friendly Celsius and Fahrenheit
  float temperatureC = steinhart - 273.15;
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

  // Print results down the Serial Monitor pipeline
  Serial.print("ADC Value: ");
  Serial.print(adcValue);
  Serial.print("  |  Resistance: ");
  Serial.print(ntcResistance, 1);
  Serial.print(" Ohms  |  Temp: ");
  Serial.print(temperatureC, 1);
  Serial.print(" °C  (");
  Serial.print(temperatureF, 1);
  Serial.println(" °F)");

  delay(1000); // Sample once every second
}
