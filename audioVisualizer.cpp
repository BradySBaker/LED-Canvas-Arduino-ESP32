#include "BlueFunctions.h"
#define micPin 32

#include <arduinoFFT.h>

#define SAMPLES 64
#define SAMPLING_FREQ   10000
unsigned long newTime;
#define AMPLITUDE 70 // Increased amplitude for lower frequencies
int bandValues[30];
double vReal[SAMPLES];
double vImag[SAMPLES];
#define NOISE        200 // Increased noise threshold
unsigned int sampling_period_us;
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);
int fallingPixels[30];

bool initializationDone = false;

int frequencyBins[] = {2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};


COLORS interpolateColor(const COLORS& startColor, const COLORS& endColor, float progress) {
  COLORS result;

  result.r = startColor.r + static_cast<byte>((endColor.r - startColor.r) * progress);
  result.g = startColor.g + static_cast<byte>((endColor.g - startColor.g) * progress);
  result.b = startColor.b + static_cast<byte>((endColor.b - startColor.b) * progress);
  return result;
}

void resetPixels() {
  for (int i = 0; i < WIDTH; i++) {
    fallingPixels[i] = -1; //y
  }
  fallingPixels[1] = HEIGHT-1;
}


void drawFallingPixel(int barHeight, int x) {
  if (barHeight > fallingPixels[x]) {
    fallingPixels[x] = barHeight;
  } else {
    fallingPixels[x]--;
  }
  if (fallingPixels[x] > 0) {
    const int fallPos =  pgm_read_word(&LED_MAP[(HEIGHT - 1) - fallingPixels[x]][x]); 
    leds[fallPos].r = paletteColors[curColorLength-1].r;
    leds[fallPos].g = paletteColors[curColorLength-1].g;
    leds[fallPos].b = paletteColors[curColorLength-1].b;
  }
}


void audioVisualizer(bool pixelFall) {
  runningFunction = true;
  if (!initializationDone) {
    resetPixels();
    sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
    initializationDone = true;
  }
  for (int i = 0; i < WIDTH; i++) {
    bandValues[i] = 0;
  }
  FastLED.show();
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(micPin);
    if (vReal[i] == 0) {
      return;
    }
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();
  
  for (int i = 0; i < WIDTH; i++) {
    bandValues[i] = 0;
  }

  for (byte i = 2; i < (SAMPLES / 2); i++) {
    if (vReal[i] > NOISE) {
      for (int band = 0; band < WIDTH; band++) {
        if (i <= frequencyBins[band]) {
          bandValues[band] += (int)vReal[i];
          break; // Exit the loop after adding the value to the corresponding band
        }
      }
    }
  }
  FastLED.clear();
  for (byte x = 0; x < WIDTH; x++) {
    // Scale the bars for the display
    int barHeight = max(0, min(bandValues[x] / AMPLITUDE, 21));
    for (byte y = 1; y < barHeight; y++) {
      const int curPos = pgm_read_word(&LED_MAP[HEIGHT - y][x]);
      float progress = static_cast<float>(y) / static_cast<float>(barHeight);
      progress *= progress;
      COLORS color = interpolateColor(paletteColors[0], paletteColors[1], progress);
      leds[curPos].r = color.r;
      leds[curPos].g = color.g;
      leds[curPos].b = color.b;
    }
    if (pixelFall) {
      drawFallingPixel(barHeight, x);
    }
  }
  FastLED.show();
  runningFunction = false;
}