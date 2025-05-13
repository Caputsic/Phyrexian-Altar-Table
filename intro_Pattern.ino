void modeIntroPattern(int whichPattern){
  if(currentMillis - pixelPrevious >= pixelInterval) { //  Check for expired time
    pixelPrevious = currentMillis; //  Run current frame
    if(whichPattern == 0){
      theaterChase(strip.Color(127, 127, 127), 200); // White
    }else if(whichPattern == 1){
      rainbow(100);
    }
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  static uint32_t loop_count = 0;
  static uint16_t current_pixel = 0;

  pixelInterval = wait;                   //  Update delay time

  strip.clear();

  for(int c=current_pixel; c < pixelNumber; c += 3) {
    strip.setPixelColor(c, color);
  }
  strip.show();

  current_pixel++;
  if (current_pixel >= 3) {
    current_pixel = 0;
    loop_count++;
  }

  if (loop_count >= 10) {
    current_pixel = 0;
    loop_count = 0;
    //patternComplete = true; //I don't think I need this variable since I'm not cycling patterns automatically
  }
}

void rainbow(uint8_t wait) {
  if(pixelInterval != wait)
    pixelInterval = wait;                   
  for(uint16_t i=0; i < pixelNumber; i++) {
    strip.setPixelColor(i, Wheel((i + pixelCycle) & 255)); //  Update delay time  
  }
  strip.show();                             //  Update strip to match
  pixelCycle++;                             //  Advance current cycle
  if(pixelCycle >= 256)
    pixelCycle = 0;                         //  Loop the cycle back to the begining
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setFullTableRed(){//Sets the entire table to red for photographs // This is no longer called since it was replaced by the 6 player mode
  for(int i=0;i<=LED_COUNT;i++){
    strip.setPixelColor(i, strip.Color(255,0,0));
  }
  strip.show();

  for(int i=0;i<=nonPlayerLeds[1];i++){
    strip2.setPixelColor(i, strip2.Color(255,0,0));
  }
  strip2.show();

}
