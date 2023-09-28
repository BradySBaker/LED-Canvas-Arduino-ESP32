#ifndef BLUE_FUNCTIONS_H
#define BLUE_FUNCTIONS_H

#include "FastLED.h"
#include "colors.h"
#include <string.h>
#include "ledMatrix.h"
#include <SD.h>
#include <SPI.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#define WIDTH 30
#define HEIGHT 20



bool displayFrame(String dir);
void handleColorChange(char charBuffer[20], bool rain);
void handleBluetooth(std::string data);
void handleDraw(char charBuffer[20]);
void handleAnimPlay();
void handleFrameSave(char charBuffer[20], bool animation);
void handleFrameDelete(char charBuffer[20]);
void handleAnimDelete(char charBuffer[20]);
int readFileCount(String directory);
void sendFileNames(String dir, bool anims);

void getPaletteAndSet(const char* colorName);

COLORS hexToRGB(const char* hexColorString);

void raindrop();

void audioVisualizer(bool pixelFall);

extern CRGB leds[600];

extern COLORS Colors;

extern String playingAnim;

//Raindrop vars
extern bool raining;
extern int raindropAmount;
extern int curColorLength;
extern COLORS paletteColors[6];
extern String background;
extern int animSpeed;
//AV vars
extern bool avActive;

//bluetooth vars
extern BLECharacteristic *pCharacteristic;

#endif