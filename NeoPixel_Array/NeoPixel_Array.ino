#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_PIXELS 256

Adafruit_NeoPixel strip(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int getPixelIndex(int x, int y) {
  int invertedY = 15 - y; /* Reverse lines */
  if (invertedY % 2 == 0) {
    return invertedY * 16 + x; /* ==> */
  } else {
    return invertedY * 16 + (15 - x); /* <== */
  }
}

uint32_t colorMatrix[16][16] = {
{strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), },
{strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(0, 61, 61), strip.Color(0, 61, 61), },
{strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(0, 61, 61), },
{strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 30, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(0, 61, 61), },
{strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), },
{strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(61, 61, 61), strip.Color(61, 61, 61), strip.Color(61, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(61, 30, 0), strip.Color(61, 30, 0), strip.Color(61, 30, 0), strip.Color(61, 30, 0), strip.Color(61, 30, 0), strip.Color(0, 61, 61), },
{strip.Color(0, 61, 61), strip.Color(61, 61, 61), strip.Color(61, 61, 61), strip.Color(61, 61, 61), strip.Color(61, 61, 61), strip.Color(61, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(61, 30, 0), strip.Color(61, 61, 0), strip.Color(61, 30, 0), strip.Color(61, 61, 0), strip.Color(61, 30, 0), strip.Color(0, 61, 61), },
{strip.Color(61, 61, 61), strip.Color(0, 0, 61), strip.Color(61, 61, 61), strip.Color(61, 61, 61), strip.Color(61, 61, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(0, 61, 61), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(0, 61, 61), strip.Color(0, 61, 61), },
{strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 61, 61), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), },
{strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 61, 61), strip.Color(61, 30, 0), strip.Color(61, 61, 0), strip.Color(61, 30, 0), strip.Color(61, 61, 0), strip.Color(61, 30, 0), strip.Color(0, 61, 61), },
{strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 0, 61), strip.Color(0, 61, 61), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(0, 61, 61), strip.Color(0, 61, 61), },
{strip.Color(0, 30, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), },
{strip.Color(0, 61, 0), strip.Color(0, 30, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 30, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 30, 0), strip.Color(61, 30, 0), strip.Color(61, 61, 0), strip.Color(61, 30, 0), strip.Color(61, 61, 0), strip.Color(61, 30, 0), strip.Color(0, 30, 0), },
{strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(30, 15, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), },
{strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 30, 0), strip.Color(0, 61, 0), strip.Color(0, 30, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), strip.Color(61, 0, 0), },
{strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 30, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 61, 0), strip.Color(0, 30, 0), strip.Color(61, 30, 0), strip.Color(61, 61, 0), strip.Color(61, 30, 0), strip.Color(61, 61, 0), strip.Color(61, 30, 0), strip.Color(0, 30, 0), },
};

void setup() {
  strip.begin();
  strip.show(); // Turn all LEDs off
}

void loop() {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      int pixelIndex = getPixelIndex(x, y);
      strip.setPixelColor(pixelIndex, colorMatrix[y][x]);
    }
  }
  strip.show();
  while(1) {} /* endless loop */
}
