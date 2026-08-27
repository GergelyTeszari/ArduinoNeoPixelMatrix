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

void setup() {
  strip.begin();
  strip.show();
}

void loop() {
  for (int color = 0; color < 3; color++) {
    for (int y = 0; y < 16; y++) {
      for (int x = 0; x < 16; x++) {
        int pixelIndex = getPixelIndex(x, y);
        if (color == 0) {
          strip.setPixelColor(pixelIndex, strip.Color(100, 0, 0));
        } else if (color == 1) {
          strip.setPixelColor(pixelIndex, strip.Color(0, 100, 0));
        } else {
          strip.setPixelColor(pixelIndex, strip.Color(0, 0, 100));
        }
        strip.show();
        delay(10);
      }
    }
  }
}
