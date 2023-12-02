#ifndef BLUE_FUNCTIONS_H
#define BLUE_FUNCTIONS_H

#include <SoftwareSerial.h>
#include "FastLED.h"
#include "colors.h"
#include <string.h>
#include "ledMatrix.h"
#include <SD.h>
#include <SPI.h>


#define WIDTH 30
#define HEIGHT 20



bool displayFrame(String dir);
void handleColorChange(char charBuffer[20], bool rain);
void handleBluetooth();
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

extern SoftwareSerial bluetooth;

extern COLORS Colors;

extern String playingAnim;

extern bool runningFunction;

//Raindrop vars
extern bool raining;
extern int raindropAmount;
extern int curColorLength;
extern COLORS paletteColors[6];
extern String background;
extern int animSpeed;
//AV vars
extern bool avActive;

#endif