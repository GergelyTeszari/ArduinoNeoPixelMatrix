#include <Adafruit_NeoPixel.h>

// Hardware configuration for the 16 x 16 matrix.
constexpr uint8_t DATA_PIN = 6;
constexpr uint8_t MATRIX_WIDTH = 16;
constexpr uint8_t MATRIX_HEIGHT = 16;
constexpr uint16_t NUM_PIXELS = MATRIX_WIDTH * MATRIX_HEIGHT;
constexpr unsigned long SERIAL_BAUD_RATE = 115200;

// One protocol line represents one complete matrix row:
// 16 pixels x 6 hexadecimal RRGGBB characters = 96 characters.
constexpr size_t HEX_CHARACTERS_PER_PIXEL = 6;
constexpr size_t ROW_HEX_LENGTH = MATRIX_WIDTH * HEX_CHARACTERS_PER_PIXEL;
constexpr char RESET_COMMAND[] = "0123456789";

Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// Convert a conventional image coordinate to the physical LED number.
// The matrix is mounted bottom-to-top and wired in alternating directions.
uint16_t getPixelIndex(uint8_t x, uint8_t y) {
  uint8_t invertedY = MATRIX_HEIGHT - 1 - y;

  if (invertedY % 2 == 0) {
    return invertedY * MATRIX_WIDTH + x;
  }

  return invertedY * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - x);
}

bool isHexCharacter(char character) {
  return (character >= '0' && character <= '9') ||
         (character >= 'a' && character <= 'f') ||
         (character >= 'A' && character <= 'F');
}

bool isValidPixelRow(const char *row, size_t length) {
  if (length != ROW_HEX_LENGTH) {
    return false;
  }

  for (size_t i = 0; i < length; i++) {
    if (!isHexCharacter(row[i])) {
      return false;
    }
  }

  return true;
}

uint8_t parseHexByte(char highNibble, char lowNibble) {
  char byteText[3] = {highNibble, lowNibble, '\0'};
  return static_cast<uint8_t>(strtoul(byteText, nullptr, 16));
}

void displayRow(const char *rowData, uint8_t logicalY) {
  for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
    size_t offset = x * HEX_CHARACTERS_PER_PIXEL;
    uint8_t red = parseHexByte(rowData[offset], rowData[offset + 1]);
    uint8_t green = parseHexByte(rowData[offset + 2], rowData[offset + 3]);
    uint8_t blue = parseHexByte(rowData[offset + 4], rowData[offset + 5]);

    uint16_t physicalIndex = getPixelIndex(x, logicalY);
    strip.setPixelColor(physicalIndex, strip.Color(red, green, blue));
  }

  // Updating once per row is the main speed improvement over the legacy
  // receiver, which calls show() after every individual pixel.
  strip.show();
}

void setup() {
  strip.begin();
  strip.show();  // Start with all LEDs off.
  Serial.begin(SERIAL_BAUD_RATE);
}

void loop() {
  // Extra space allows an optional carriage return before the terminating LF.
  static char inputBuffer[ROW_HEX_LENGTH + 2];
  static uint8_t currentRow = 0;

  if (!Serial.available()) {
    return;
  }

  size_t bytesRead = Serial.readBytesUntil(
      '\n', inputBuffer, sizeof(inputBuffer) - 1);

  // Accept both LF and CRLF senders by removing a trailing carriage return.
  if (bytesRead > 0 && inputBuffer[bytesRead - 1] == '\r') {
    bytesRead--;
  }
  inputBuffer[bytesRead] = '\0';

  if (bytesRead == sizeof(RESET_COMMAND) - 1 &&
      strcmp(inputBuffer, RESET_COMMAND) == 0) {
    // The optimized reset only moves the write cursor to row zero. It does not
    // clear the already displayed pixels; the following rows overwrite them.
    currentRow = 0;
    return;
  }

  // A frame contains exactly 16 rows. Further rows are ignored until RESET.
  if (currentRow >= MATRIX_HEIGHT) {
    return;
  }

  if (!isValidPixelRow(inputBuffer, bytesRead)) {
    return;
  }

  displayRow(inputBuffer, currentRow);
  currentRow++;
}
