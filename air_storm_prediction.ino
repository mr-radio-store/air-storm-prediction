/*
Based on input including air to predict storm

To simulate varying air data and produce graph 
variations on the OLED (instead of using real sensors), 
you can modify the Arduino code to:

***
    Simuate air data inputs
    Pass them into your ML model
    Plot values (e.g., pressure, humidity) as moving graphs
    Display storm prediction result

    mathematical formula (heuristic)

Let's define:

    TT = temperature (°C)

    HH = humidity (%)

    PP = pressure (hPa)

A storm likelihood score SS can be computed as:
S=a×(100−P)+b×H+c×T
S=a×(100−P)+b×H+c×T

***

OLED wire connecction
 Wiring: I2C OLED
OLED Pin	Arduino Uno / Mega
VCC	5V
GND	GND
SDA	A4 (Uno) / 20 (Mega)
SCL	A5 (Uno) / 21 (Mega)

*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// === Fake data simulation ===
float temp = 25.0;
float humidity = 50.0;
float pressure = 1015.0;

int graphY[SCREEN_WIDTH];  // Store graph history

// === Simple ML model ===
float score(float temp, float hum, float press) {
  if (press < 1005 || hum > 80) {
    return 1; // Storm
  }
  return 0; // Clear
}

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  display.clearDisplay();

  // Simulate data
  temp += random(-10, 10) * 0.1;
  humidity += random(-5, 5);
  pressure += random(-15, 15) * 0.1;

  // Clamp values
  if (temp < -10) temp = -10;
  if (temp > 40) temp = 40;
  if (humidity < 10) humidity = 10;
  if (humidity > 100) humidity = 100;
  if (pressure < 980) pressure = 980;
  if (pressure > 1040) pressure = 1040;

  // Predict storm
  int result = score(temp, humidity, pressure);

  // Shift graph left
  for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
    graphY[i] = graphY[i + 1];
  }

  // Map pressure to Y position
  int y = map((int)pressure, 980, 1040, SCREEN_HEIGHT - 1, 16);
  graphY[SCREEN_WIDTH - 1] = y;

  // Draw graph
  for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
    display.drawLine(i, graphY[i], i + 1, graphY[i + 1], SSD1306_WHITE);
  }

  // Show label and result
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("T:");
  display.print(temp, 1);
  display.print("C H:");
  display.print(humidity, 0);
  display.print("% P:");
  display.print(pressure, 0);
  display.println("hPa");

  display.setCursor(0, 8);
  display.setTextSize(1);
  display.print("Status: ");
  if (result == 1) {
    display.setTextSize(2);
    display.setCursor(0, 48);
    display.print("STORM!");
  } else {
    display.setTextSize(2);
    display.setCursor(0, 48);
    display.print("CLEAR ");
  }

  display.display();
  delay(500);
}
