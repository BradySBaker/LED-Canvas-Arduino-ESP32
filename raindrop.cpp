#include "BlueFunctions.h"

//Variables
int raindropSpeed = 5;
int splashAmmount = 2;
int randomnessIndex = 0;
int buttonState = 0;
int backgroundNum = 1;
int buttonHeld = false;
int buttonHeldTime = 0;
int brightness = 64;



void raindrop() {
  int rainDrops[raindropAmount][7];
  CRGB copiedLeds[256];
  bool readyToDraw = false;

  for (int i = 0; i < raindropAmount; i++) {
    rainDrops[i][0] = 0; //Y position 
    rainDrops[i][1] = random(0, WIDTH); //X position
    rainDrops[i][2] = 0; //Time
    rainDrops[i][3] = random(0, HEIGHT); //Execution Time
    rainDrops[i][4] = random(0, curColorLength + 1); //Color index

    rainDrops[i][5] = 0;
    rainDrops[i][6] = raindropSpeed;
  }
  
  while(raining) {
    // handleBluetooth();
    if (background != "~" ) {
      Serial.println("Drawing frame");
      displayFrame("/drawings/" + background + ".TXT");
      // Copy the 'leds' array to 'copiedLeds'
      memcpy(copiedLeds, leds, sizeof(CRGB) * 256);
      readyToDraw = true;
      background = "~";
    } else if (readyToDraw) {
      memcpy(leds, copiedLeds, sizeof(CRGB) * 256);
    } else {
      FastLED.clear();
    }

    for (int j = 0; j < raindropAmount; j++) {
      // handleBluetooth();
      if (!raining) {
        break;
      }
      int curPos = pgm_read_word(&LED_MAP[rainDrops[j][0]][rainDrops[j][1]]);

      COLORS color = paletteColors[rainDrops[j][4]];

      int timeSinceDrop = rainDrops[j][2] - rainDrops[j][3];

      if (timeSinceDrop >= 0) {
      //Draw pixel
      leds[curPos].r = color.r;
      leds[curPos].g = color.g;
      leds[curPos].b = color.b;


      //If rainDrop hit ground
      if (rainDrops[j][0] >= HEIGHT-1) {
        rainDrops[j][5]++;
        //If ground animation done playing
        if (rainDrops[j][5] >= HEIGHT-1) {
          rainDrops[j][2] = 0;
          rainDrops[j][0] = 0;
          rainDrops[j][1] = random(0, WIDTH);
          rainDrops[j][5] = 0;
          rainDrops[j][6] = raindropSpeed;
        } else {
          for (int i = 0; i < rainDrops[j][5]/random(4,6); i++) {
            int forwardPos = pgm_read_word(&LED_MAP[rainDrops[j][0]][rainDrops[j][1]] + i);
            int backwardPos = pgm_read_word(&LED_MAP[rainDrops[j][0]][rainDrops[j][1]] - i);

            // Check if the positions are within the valid LED range
            if (forwardPos >= 0 && forwardPos < 256) {
              leds[forwardPos].r = color.r;
              leds[forwardPos].g = color.g;
              leds[forwardPos].b = color.b;
            }

            if (backwardPos >= 0 && backwardPos < 256) {
              leds[backwardPos].r = color.r;
              leds[backwardPos].g = color.g;
              leds[backwardPos].b = color.b;
            }
        }
      }
      } else {
        //If rainDrop didn't hit ground
          if (timeSinceDrop % rainDrops[j][6] == 0 || rainDrops[j][6] == 0) {
            if (rainDrops[j][5] == 2 && rainDrops[j][6] > 0) {
              rainDrops[j][5] = 0;
              rainDrops[j][6] -= 1;
            } else {
              rainDrops[j][5]++;
            }
            rainDrops[j][0]++;
          }
        }
      }
        rainDrops[j][2]++;
    }
    // handleBluetooth();
    FastLED.show();
  }
}