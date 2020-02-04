#include "FastLED.h"
#define NUM_LEDS 8
CRGB leds[NUM_LEDS];
#define PIN 5

// pin map, because I never remember these things...
// If you skip pin 2, it'll just start immediately. PIN 2 lets you wire it to a switch
// PIN 2 -> GROUND - this is the "open the box" switch
// PIN 5 -> LED strand DATA
// Led strip white -> Ground
// LED strip red -> +5
// The board will hang until Pin 2 is disconnected, so if there's an LED lit when you recompile this, it'll stay lit, don't sweat it.

// most of this code shamelessly stolen from:
//  https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/

void setup()
{
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  const byte buttonPin = 2;
  pinMode(buttonPin, INPUT_PULLUP);
  while (digitalRead(buttonPin) == LOW) {}
}

void loop() {
  // https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#HelpfultoolColorPicker
  // about 10 seconds
  for ( int counter = 1 ; counter <= 300 ; counter++ )
  { Sparkle(0xff, 0xff, 0xff, 50); }
  
  // if we add more LEDs, this looks cool
  //meteorRain(0x33,0xff,0x0a,1, 640, true, 200);

  // do we want the roses illuminated all the time or just dimly lit and sometimes "sparkling"?
  // Halloween eyes - good for bright rooms 
  // SnowSparkle - good for dim rooms
  
  // loop forever, pick two LEDs at random, light them up and fade them out
  //while(1) { HalloweenEyes(0xf7, 0xff, 0x00, 1,1, true, 100, 80, 3000); }

  // loop forever, light all LEDs with a color, sometimes one will flash
  while(1) { SnowSparkle(0xf7, 0xff, 0x00, 20, random(300,1000)); }
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

void HalloweenEyes(byte red, byte green, byte blue,
                   int EyeWidth, int EyeSpace,
                   boolean Fade, int Steps, int FadeDelay,
                   int EndPause){
  randomSeed(analogRead(0));
 
  int i;
  int StartPoint  = random( 0, NUM_LEDS - (2*EyeWidth) - EyeSpace );
  int Start2ndEye = StartPoint + EyeWidth + EyeSpace;
 
  for(i = 0; i < EyeWidth; i++) {
    setPixel(StartPoint + i, red, green, blue);
    setPixel(Start2ndEye + i, red, green, blue);
  }
 
  showStrip();
 
  if(Fade==true) {
    float r, g, b;
 
    for(int j = Steps; j >= 0; j--) {
      r = j*(red/Steps);
      g = j*(green/Steps);
      b = j*(blue/Steps);
     
      for(i = 0; i < EyeWidth; i++) {
        setPixel(StartPoint + i, r, g, b);
        setPixel(Start2ndEye + i, r, g, b);
      }
     
      showStrip();
      delay(FadeDelay);
    }
  }
 
  setAll(00,00,00); // Set all black
 
  delay(EndPause);
}

void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setAll(red,green,blue);
 
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,0xff,0xff,0xff);
  showStrip();
  delay(SparkleDelay);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
}
