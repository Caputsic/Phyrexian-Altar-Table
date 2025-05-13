void breathing(int start, int end, uint8_t wait) {
  
  if(currentMillis - pixelPrevious >= pixelInterval) { //  Check for expired time
    pixelPrevious = currentMillis; //  Run current frame
    
    if(pixelInterval != wait){
      pixelInterval = wait;
    }                
  
    for(int i=start; i <= end; i++) {
      //strip.setPixelColor(i, strip.Color(pixelCycle,25,25)); 
      strip2.setPixelColor(i, strip2.Color(pixelCycle,0,0)); //  Update breathing LEDs   
    }
    strip2.show();                             //  Update strip to match
   
    if(breathCycle){
      pixelCycle++;                             //  Advance current cycle      
    }else{
      pixelCycle--;
    }
    if(pixelCycle >= 255){
      breathCycle = false;     // set to breath out
      }else if(pixelCycle <=30){
        breathCycle = true;    // set to breath in
    }
  }
}


