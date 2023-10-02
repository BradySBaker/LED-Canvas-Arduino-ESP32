#include "BlueFunctions.h"

#define LEDPIN 12
const int chipSelect = 5;

CRGB leds[WIDTH * HEIGHT];

COLORS Colors(200, 200, 200);

String playingAnim = "~";

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;

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

class CharteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0) {
           handleBluetooth(value.c_str());
        }
    }
};

class ServerCallbacks : public BLEServerCallbacks {
public:
  void onConnect(BLEServer* pServer) {
    // Handle the onConnect event (optional)
    Serial.println("BLE client connected");
  }

  void onDisconnect(BLEServer* pServer) {
    // Handle the onDisconnect event
    Serial.println("BLE client disconnected");

    // Perform any necessary cleanup or actions here

    // Optionally restart the ESP32
    ESP.restart();
  }
};


void runBLEServer() {
  BLEDevice::init("LED Panel");
  // Create a BLE server
  pServer = BLEDevice::createServer();
    
  pServer->setCallbacks(new ServerCallbacks());

  // Create a BLE service
  pService = pServer->createService("12345678-1234-5678-1234-56789abcdef0");
  
  // Create a BLE characteristic
  pCharacteristic = pService->createCharacteristic(
    "12345678-1234-5678-1234-56789abcdef1",
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->setCallbacks(new CharteristicCallbacks());

  BLEDescriptor* pStatusDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2902));
  pCharacteristic->addDescriptor(pStatusDescriptor);
  pStatusDescriptor->setValue("Notify");
  delay(100);
  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();
  delay(50);
  Serial.println("BLE advertising started");

}
// Function handlers -------------------------
TaskHandle_t ModeTaskHandle = NULL;

void runMode(void *parameter) {
   while (1) {
      if (avActive) {
         audioVisualizer(true);
      }
      if (playingAnim != "~") {
        handleAnimPlay();
      }
      vTaskDelay(pdMS_TO_TICKS(1));
   }
}

void setup() { //Setup ---------------------------------------------------------------------------------------
  Serial.begin(115200);

  runBLEServer();

  xTaskCreatePinnedToCore(
    runMode,
    "ModeTask",
    10000,
    NULL,
    0,
    &ModeTaskHandle,
    0 
  );
  vTaskStartScheduler();

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
}
