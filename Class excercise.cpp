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
      float vThermistor = (supplyVoltage_ / 1023.0) * sensorValue;
      float vSeries = supplyVoltage_ - vThermistor;
      float thermistorResistance = (vThermistor * seriesResistance_) / vSeries;

      float temperatureK = 1.0 / ((1.0 / nominalTemperatureK_) +
        (log(thermistorResistance / nominalResistance_) / betaCoefficient_));

      return temperatureK - 273.15;
    }

  private:
    int pin_;
    float seriesResistance_;
    float nominalResistance_;
    float nominalTemperatureK_;
    float betaCoefficient_;
    float supplyVoltage_;
};

Thermistor thermistor(A0, R, R_T1, 25.0, B, VCC);

void setup() {
  Serial.begin(9600);
  thermistor.begin();
}

void loop() {
  float temperatureC = thermistor.readTemperatureC();

  Serial.print("Temperature: ");
  Serial.print(temperatureC, 1);
  Serial.println(" °C");

  delay(500);
}
