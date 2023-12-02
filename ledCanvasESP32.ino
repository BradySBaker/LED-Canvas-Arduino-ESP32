#include "BlueFunctions.h"

#define LEDPIN 12
const int chipSelect = 5;

SoftwareSerial bluetooth(16, 17);

CRGB leds[WIDTH * HEIGHT];

COLORS Colors(200, 200, 200);

String playingAnim = "~";

bool runningFunction = false;

COLORS hexToRGB(const char* hexColorString) {
  COLORS result;
  unsigned long colorInt = 0;

  for (int i = 0; i < 6; i++) {
    if (hexColorString[i] >= '0' && hexColorString[i] <= '9') {
      colorInt = colorInt + (((long)hexColorString[i] - '0') << ((5 - i) * 4));
    } else if (hexColorString[i] >= 'a' && hexColorString[i] <= 'f') {
      colorInt = colorInt + (((long)hexColorString[i] - 'a' + 10) << ((5 - i) * 4));
    } else if (hexColorString[i] >= 'A' && hexColorString[i] <= 'F') {
      colorInt = colorInt + (((long)hexColorString[i] - 'A' + 10) << ((5 - i) * 4));
    }
  }

  result.r = (colorInt >> 16) & 0xFF;
  result.g = (colorInt >> 8) & 0xFF;
  result.b = colorInt & 0xFF;

  Serial.println(result.r);
  Serial.println(result.g);
  Serial.println(result.b);

  return result;
}
void modifyPaletteColors(const COLORS* palette, int numColors) {
  curColorLength = numColors;
  for (int i = 0; i < numColors; i++) {
    Serial.println(i);
    paletteColors[i] = palette[i];
  }
}

void getPaletteAndSet(const char* colorName) {
    if (strcmp(colorName, "CMPred") == 0) {
      modifyPaletteColors(red, 3);
    } else if (strcmp(colorName, "CMPblue") == 0) {
      modifyPaletteColors(blue, 3);
    } else if (strcmp(colorName, "CMPgreen") == 0) {
      modifyPaletteColors(green, 3);
    } else if (strcmp(colorName, "CMPpurple") == 0) {
      modifyPaletteColors(purple, 3);
    }
}
COLORS paletteColors[6] = {};

// Raindrop Variables
bool raining = false;
int raindropAmount = 5;
int curColorLength = 0;
int animSpeed = 0;
String background = "~";

// AV variables
bool avActive = false;

void setup() { //Setup ---------------------------------------------------------------------------------------
  bluetooth.begin(115200);
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, LEDPIN, GRB>(leds, WIDTH * HEIGHT);

  FastLED.setBrightness(100);

  randomSeed(analogRead(0));

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
  } else {
    Serial.println("SD card intialized");
  }

}

void loop() {
  handleBluetooth();
}
