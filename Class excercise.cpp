//Thermometer with thermistor
// ## CHECK ACTUAL setup resistance
//These values are in the datasheet
#define R_T1 10000.0  // Resistance from datasheet at temperature 25°C
#define B 3950.0      // Beta value from datasheet
#define VCC 5.0       // Supply voltage
#define R 10000.0     // Resistor connected in series with thermistor

class Thermistor {
  public:
    Thermistor(int pin, float seriesResistance, float nominalResistance,
               float nominalTemperatureC, float betaCoefficient,
               float supplyVoltage = VCC)
      : pin_(pin),
        seriesResistance_(seriesResistance),
        nominalResistance_(nominalResistance),
        nominalTemperatureK_(nominalTemperatureC + 273.15),
        betaCoefficient_(betaCoefficient),
        supplyVoltage_(supplyVoltage) {}

    void begin() {
      pinMode(pin_, INPUT);
    }

    float readTemperatureC() {
      int sensorValue = analogRead(pin_);
      // Read the analog value from the thermistor
      // Calculate the voltage across the thermistor
      // Calculate the resistance of the thermistor
      // Calculate the temperature in Kelvin using the Steinhart-Hart equation
      // Convert Kelvin to Celsius
      // Steinhart-Hart equation: 1/T = 1/T0 + (1/B) * ln(R/R0)
      // Where T is the temperature in Kelvin, T0 is the nominal temperature in Kelvin,
      // B is the beta coefficient, R is the resistance of the thermistor, and R0 is the nominal resistance.
      // Calculate the voltage across the thermistor
      // Calculate the resistance of the thermistor
      // Calculate the temperature in Kelvin using the Steinhart-Hart equation
      // Convert Kelvin to Celsius by returning temperatureK - 273.15
      float vThermistor = (supplyVoltage_ / 1023.0) * sensorValue;
      float vSeries = supplyVoltage_ - vThermistor;
      float thermistorResistance = (vThermistor * seriesResistance_) / vSeries;

      float temperatureK = 1.0 / ((1.0 / nominalTemperatureK_) +
        (log(thermistorResistance / nominalResistance_) / betaCoefficient_));
      // log = natural logarithm (base e)
      // nominalResistance is the resistance of the thermistor at the nominal temperature (25°C) which is the resistance value from the datasheet (R_T1)
      // betaCoefficient is the beta value from the datasheet (B)
      // nominalTemperatureK is the nominal temperature in Kelvin (25°C + 273.15)
      return temperatureK - 273.15;
    }

  private:
    int pin_;
    float seriesResistance_;
    float nominalResistance_;
    float nominalTemperatureK_;
    float betaCoefficient_;
    float supplyVoltage_;
    // The pin number where the thermistor is connected
    // The resistance of the series resistor connected in series with the thermistor
    // The nominal resistance of the thermistor at the nominal temperature (25°C)
    // The nominal temperature in Kelvin (25°C + 273.15)
    // The beta coefficient of the thermistor
    // The supply voltage for the thermistor circuit
};

Thermistor thermistor(A0, R, R_T1, 25.0, B, VCC);

void setup() {
  Serial.begin(9600);
  thermistor.begin();
  // .begin() initializes the thermistor by setting the pin mode to INPUT
}

void loop() {
  float temperatureC = thermistor.readTemperatureC();

  Serial.print("Temperature: ");
  Serial.print(temperatureC, 1);
  Serial.println(" °C");

  delay(500);
}
