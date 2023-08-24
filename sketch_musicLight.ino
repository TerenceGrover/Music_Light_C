#include <Adafruit_NeoPixel.h>

String data;

#define MAX_VOLUME 8  // Maximum volume level expected
#define NUM_LEDS_PER_STRIP 13 // Number of LEDs in your strip

// Define pins for each strip
const int ledPins[5] = {11, 10, 9, 6, 5};

// Define RGB values for the gradient
const int colorGradient[5][3] = {
  {225, 230, 230},
  {255, 175, 205},
  {225, 130, 195},
  {225, 135, 215},
  {255, 210, 255}
};

// Create an instance of the NeoPixel strip for each channel
Adafruit_NeoPixel strips[5] = {
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, ledPins[0], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, ledPins[1], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, ledPins[2], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, ledPins[3], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, ledPins[4], NEO_GRB + NEO_KHZ800)
};

void setup() {
  Serial.begin(115200);
  
  // Initialize all strips
  for(int i = 0; i < 5; i++) {
    strips[i].begin();
    strips[i].show();  // Initialize all pixels to 'off'
  }
}

void loop() {
    while (Serial.available()) {
      data = Serial.readStringUntil('\n');

      int volumes[5];
      int startIndex = 0;
      int endIndex;
      // Split the incoming data into separate volumes
      for(int i = 0; i < 5; i++) {
        endIndex = data.indexOf(',', startIndex);
        if(endIndex == -1) endIndex = data.length(); // If no more commas, take till end of string
        volumes[i] = map(data.substring(startIndex, endIndex).toInt(), 0, MAX_VOLUME, 0, NUM_LEDS_PER_STRIP);
        startIndex = endIndex + 1; // Move past the comma for next iteration
      }

      // Set the brightness of each strip based on the parsed volume
      for(int i = 0; i < 5; i++) {
        for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
          int ledIndex = (i % 2 == 0) ? NUM_LEDS_PER_STRIP - j - 1 : j; // Invert index for every other strip
          if (j < volumes[i]) {
            // If this LED should be on, set its color to the gradient color
            strips[i].setPixelColor(ledIndex, strips[i].Color(colorGradient[i][0], colorGradient[i][1] - j * 10, colorGradient[i][2] - j * 15));
          } else {
            // Otherwise, turn it off
            strips[i].setPixelColor(ledIndex, strips[i].Color(0, 0, 0));
          }
        }
        strips[i].show();
      }
    }
}
