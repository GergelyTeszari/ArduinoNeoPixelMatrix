#include <Adafruit_NeoPixel.h>

// Hardware configuration for the 16 x 16 matrix.
constexpr uint8_t DATA_PIN = 6;
constexpr uint8_t MATRIX_WIDTH = 16;
constexpr uint8_t MATRIX_HEIGHT = 16;
constexpr uint16_t NUM_PIXELS = MATRIX_WIDTH * MATRIX_HEIGHT;
constexpr unsigned long SERIAL_BAUD_RATE = 115200;

// The legacy protocol receives one six-character RRGGBB value per line.
// Historically, any ten-character line has acted as a frame reset command.
constexpr uint8_t PIXEL_HEX_LENGTH = 6;
constexpr uint8_t RESET_MESSAGE_LENGTH = 10;

Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// Index of the next logical pixel in row-major image order.
uint16_t nextPixel = 0;

// Convert an image coordinate to the physical LED number.
//
// The matrix is installed bottom-to-top and wired in a serpentine (snake)
// pattern. Image data can therefore stay in conventional top-left origin,
// left-to-right row order while this function handles the wiring layout.
uint16_t getPixelIndex(uint8_t x, uint8_t y) {
  uint8_t invertedY = MATRIX_HEIGHT - 1 - y;

  if (invertedY % 2 == 0) {
    return invertedY * MATRIX_WIDTH + x;
  }

  return invertedY * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - x);
}

void clearMatrix() {
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void displayNextPixel(const String &hexColor) {
  // Interpret the six input characters as one 24-bit RRGGBB value.
  unsigned long color = strtoul(hexColor.c_str(), nullptr, 16);
  uint8_t red = (color >> 16) & 0xFF;
  uint8_t green = (color >> 8) & 0xFF;
  uint8_t blue = color & 0xFF;

  uint8_t x = nextPixel % MATRIX_WIDTH;
  uint8_t y = nextPixel / MATRIX_WIDTH;
  uint16_t physicalIndex = getPixelIndex(x, y);

  strip.setPixelColor(physicalIndex, strip.Color(red, green, blue));

  // This is intentionally immediate in the legacy implementation. It makes
  // the incoming image visible pixel by pixel, but is slower than the row-based
  // optimized receiver.
  strip.show();
  nextPixel++;
}

void processSerialLine(const String &line) {
  if (line.length() == RESET_MESSAGE_LENGTH) {
    // Legacy compatibility: the original sketch treated any ten-character
    // line as reset. It clears the LEDs and restarts at logical pixel (0, 0).
    nextPixel = 0;
    clearMatrix();
    return;
  }

  if (line.length() == PIXEL_HEX_LENGTH && nextPixel < NUM_PIXELS) {
    displayNextPixel(line);
  }

  // Empty, malformed and excess pixel lines are ignored.
}

void setup() {
  strip.begin();
  strip.show();  // Start with all LEDs off.
  Serial.begin(SERIAL_BAUD_RATE);
}

void loop() {
  static String input;

  while (Serial.available()) {
    char character = Serial.read();

    // Both LF and CR are accepted as line endings. With CRLF, the second
    // character produces an empty line, which processSerialLine ignores.
    if (character == '\n' || character == '\r') {
      processSerialLine(input);
      input = "";
      continue;
    }

    input += character;

    // Recover automatically if a sender transmits an unexpectedly long line.
    if (input.length() > 16) {
      input = "";
    }
  }
}
