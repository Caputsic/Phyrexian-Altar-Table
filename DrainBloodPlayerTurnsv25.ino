#include <Adafruit_NeoPixel.h>
#include <ButtonEvents.h> // we have to include the library in order to use it

#define freeForAll 0 //
#define TwoHeadedGiant 1 // These two defines are for clarity when checking the gameMode
#define SixPlayerGame 2 //Used when checking gameMode
#define LED_PIN    6  // Which pin on the Arduino is connected to the NeoPixels?
#define LED_COUNT 159  // How many NeoPixels are attached to the Arduino?
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // Declare our NeoPixel strip object:
Adafruit_NeoPixel strip2(LED_COUNT, 13, NEO_GRB + NEO_KHZ800); // Declare our NeoPixel strip object:

//   **** Game LED Variables ****
int whoseTurn = 0; // start with player 1's turn
int activePlayer=0;
int maxPlayers = 4; //default this to 4 ie allowing for free for all players 0-3.  Will be set to 6 for 6 player game

int freeForAllArrayBegin[]{0,32,79,111,999,999};//999 is a place holder for the 6 player game mode
int freeForAllArrayEnd[]{21,53,100,132,999,999}; 
int twoHeadedGiantArrayBegin[]{0,79,999,999,999,999}; //this is a two player game so the 2,3,4, and 5 slots wont be used
int twoHeadedGiantArrayEnd[]{53,132,999,999,999,999}; 
int sixPlayerGameArrayBegin[]{0,32,55,79,111,135};
int sixPlayerGameArrayEnd[]{21,53,76,100,132,156};

int playerBegin[]{0,0,0,0,0,0};  //This array drives all of the flowin and flow out. But will be updated based on the gameMode using above arrays
int playerEnd[]{0,0,0,0,0,0};

int endOfPlayerLeds = 159;  //This is the same as LED count for strip because all the center leds are strip2
int randomList[159]; //used for Death Animation  // it is initialized to a sequential list in Setup

// **** Center LED Variables ****
int nonPlayerLeds[]{0,119}; // Defines the range for the breathing LEDs// this is the full count since it is the full strip 2 (But may be shorter once I cut the strip)
int gameModeBegin[]{19,41,50,93};//defines the 4 quadrants of the center circle for game mode indication
int gameModeEnd[]{28,50,58,101};
int gameMode = 0; //default the game mode to free for all 

//   **** Turn Order Variables ****
//define the pins for each button
const byte buttonPinA = 12; 
const byte buttonPinB = 8; 
const byte buttonPinC = 9;
const byte buttonPinD = 11;
const byte buttonPinE = 3;
const byte buttonPinF = 10;  //Player Pass Turn Button1
const byte buttonPinG = 7;  //Player Pass Turn Button2

// define the potitions for the active player array
const int p1 = 0;
const int p2 = 1;
const int p3 = 2;
const int p4 = 3;
const int p5 = 4;
const int p6 = 5;

// create an instance of the ButtonEvents class to attach to our button
ButtonEvents player1;
ButtonEvents player2; 
ButtonEvents player3; 
ButtonEvents player4;
ButtonEvents modeButton; //Used for switching game modes
ButtonEvents passTurnButton1; //Player Pass Turn Button1
ButtonEvents passTurnButton2; //Player Pass Turn Button2

//keep track of player active or eliminated
boolean player1Status =false; //All players default to dead
boolean player2Status =false; //All players default to dead 
boolean player3Status =false; //All players default to dead
boolean player4Status =false; //All players default to dead
boolean player5Status =false; //All players default to dead
boolean player6Status =false; //All players default to dead

//keep track of turn
int activePlayers[] = {0, 0, 0, 0, 0, 0}; //an array to track who is active (used for turn order)  0 is dead, 1 is alive
unsigned long durationSincePlayerJoinLED = 0;
boolean ledsWereUpdated = true; //used to only trigger an led update if a change occured

//   **** Intro Animation LED Variables ****
int pixelInterval = 50;       // Pixel Interval (ms)
uint16_t      pixelNumber = LED_COUNT;  // Total Number of Pixels
int mode = 1; //Default mode to 1, which is Intro mode
int oldMode = 1;
unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long currentMillis = 0;
int pixelCycle = 0;           // Pattern Pixel Cycle
bool breathCycle = true; // Defines if the LEDs are breathing in or out

//  **** Misc Variables ****
bool debug = false; // set this to true to do debugging

void setup() {
  Serial.begin(115200);
  
     // ***Setup LEDs*** 
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(150); // Set BRIGHTNESS (max = 255)

  //  Code for a second LED strip //
  strip2.begin();
  strip2.show();
  strip2.setBrightness(150);
  for(int i=0;i<=nonPlayerLeds[1];i++){
    strip2.setPixelColor(i, strip2.Color(255,0,0));
  }
  for(int i=gameModeBegin[gameMode];i<gameModeEnd[gameMode];i++){//Then update just the game Mode leds in the circle to white
      strip2.setPixelColor(i, strip2.Color(255,255,255));
  }
  strip2.show();

     // ***Setup Players***
  // Configure each button pin as in input, with a pullup resistor
  pinMode(buttonPinA, INPUT_PULLUP);   
  pinMode(buttonPinB, INPUT_PULLUP); 
  pinMode(buttonPinC, INPUT_PULLUP);
  pinMode(buttonPinD, INPUT_PULLUP); 
  pinMode(buttonPinE, INPUT_PULLUP);
  pinMode(buttonPinF, INPUT_PULLUP);
  pinMode(buttonPinG, INPUT_PULLUP);

  // attach our ButtonEvents instance to the button pins
  player1.attach(buttonPinA);
  player2.attach(buttonPinB);
  player3.attach(buttonPinC);
  player4.attach(buttonPinD);
  modeButton.attach(buttonPinE);
  passTurnButton1.attach(buttonPinF);
  passTurnButton2.attach(buttonPinG);

  //Set the button hold timing
  int playerButtonHoldTimes = 800; //This is only used right below for setting the duration for holding player entry buttons
  player1.holdTime(playerButtonHoldTimes);
  player2.holdTime(playerButtonHoldTimes);
  player3.holdTime(playerButtonHoldTimes);
  player4.holdTime(playerButtonHoldTimes);
  passTurnButton1.holdTime(playerButtonHoldTimes);
  passTurnButton2.holdTime(playerButtonHoldTimes);

  //initialize randomized list of leds 
  for(int i=0;i<strip.numPixels();i++){
    randomList[i]=i; // Set each spot to itself
  }
  shuffleArray(randomList,strip.numPixels());  //Call the shuffle array twice to help make the array be all mixed up!
  shuffleArray(randomList,strip.numPixels());  //Calling it twice helps prevent the first few deaths from being kinda sequential

  //initialize gameMode variables
  playerBegin[0]=freeForAllArrayBegin[0];//initial game Mode defaults to free for all
  playerBegin[1]=freeForAllArrayBegin[1];
  playerBegin[2]=freeForAllArrayBegin[2];
  playerBegin[3]=freeForAllArrayBegin[3];// These arrays go up to 6 values, but since game mode 0 freefor all is only 4 players I can leave this to initialize at 4
  playerEnd[0]=freeForAllArrayEnd[0];
  playerEnd[1]=freeForAllArrayEnd[1];
  playerEnd[2]=freeForAllArrayEnd[2];
  playerEnd[3]=freeForAllArrayEnd[3];
}


void loop() {
  currentMillis = millis(); //  Update current time  
  updatePlayersButtons(); //refresh the button timings

  //When the mode button is tapped, or either passTurnButton is held change the mode
  if(passTurnButton1.held()||passTurnButton2.held()){  
    mode++; 
    if(mode>=3){//Set it to never get to mode 3. I just want 2 modes, but am keeping the code in the switch statment below
      mode=1; 
    }
  }

    switch(mode){
      case 1:
        if(mode!=oldMode){
          drawGameModeCircle();//update the center circle leds to indicate which game mode we are in
        }
        modeStartScreen();
        break;
      case 2:
        if(mode!=oldMode){//if the mode was changed, then update the leds to players again.
          ledsWereUpdated=true;
        }
        if(gameMode==2){
          //setFullTableRed();//Sets the whole table red. This is a placeholder for the 6 player gameModes and is for photos!
          modeGamePlayLoop(); //gameMode 2 is a 6 player game
        }else if(gameMode==3){
          modeIntroPattern(1); //This is the multicolor animation
        }else{
          modeGamePlayLoop();
        }
        break;
      default:
       //modeDefault(); //This mode should never happen, I may wish to put a single indicator LED to act as an alert
        break;
    }
    oldMode=mode;
}//end Loop

void modeStartScreen(){
  modeIntroPattern(0);
  if(passTurnButton1.tapped()||passTurnButton2.tapped()){  //if either passTurnButton is tapped while on the Intro Screen then change game 4way/2hd/5player/6player
    gameMode++;

    if(gameMode>=4){// gameMode is defined by the sectionh of the cent circle and equates to free for all, 2HG, and 6 player game. 
      gameMode=0; // when the mode rolls over set it back to 1
    }
    //Serial.println(gameMode);
    drawGameModeCircle();//update the center circle leds to indicate which game mode we are in  
    switch(gameMode){
      case freeForAll:
        initializeFreeForAll();
        break;
      case TwoHeadedGiant:
        initialzeTwoHeadedgiant();
        break;
      case SixPlayerGame:
        initializeSixPlayerGame();
        break;  
      case 3:
        //modeIntroPattern(1); //This is the multicolor animation
        //Case 3 is referenced in the modeGamePlayLoop to set the full table to the multicolor animation
       break;
      default:
        //hopefully this should never happen
        break;
    }
  }
}

void drawGameModeCircle(){
  for(int i=0;i<nonPlayerLeds[1];i++){//Reset all center leds to red
      strip2.setPixelColor(i, strip2.Color(255,0,0));
    }
  for(int i=gameModeBegin[gameMode];i<gameModeEnd[gameMode];i++){//Then update just the game Mode leds in the circle to white
      strip2.setPixelColor(i, strip2.Color(255,255,255));
    }
  strip2.show();
}

void initializeFreeForAll(){
  maxPlayers = 4;
  activePlayers[0] = 0; //reset everybody to dead
  activePlayers[1] = 0; //reset everybody to dead
  activePlayers[2] = 0; //reset everybody to dead
  activePlayers[3] = 0; //reset everybody to dead
  activePlayers[4] = 0; //reset everybody to dead
  activePlayers[5] = 0; //reset everybody to dead
  player1Status = false; //All players default to dead
  player2Status = false; //All players default to dead 
  player3Status = false; //All players default to dead
  player4Status = false; //All players default to dead
  player5Status = false; //All players default to dead
  player6Status = false; //All players default to dead
  whoseTurn = 0; // start with player 1's turn
  activePlayer=0; //Player 1 is the active player
  playerBegin[0]=freeForAllArrayBegin[0];//initial game Mode defaults to free for all
  playerBegin[1]=freeForAllArrayBegin[1];
  playerBegin[2]=freeForAllArrayBegin[2];
  playerBegin[3]=freeForAllArrayBegin[3];
  playerBegin[4]=freeForAllArrayBegin[4];
  playerBegin[5]=freeForAllArrayBegin[5];
  playerEnd[0]=freeForAllArrayEnd[0];
  playerEnd[1]=freeForAllArrayEnd[1];
  playerEnd[2]=freeForAllArrayEnd[2];
  playerEnd[3]=freeForAllArrayEnd[3];
  playerEnd[4]=freeForAllArrayEnd[4];
  playerEnd[5]=freeForAllArrayEnd[5];
  //resetLEDsToActivePlayerView(playerBegin[0],playerEnd[0]); Don't need to do this, cause the marqueue is still running
}

void initialzeTwoHeadedgiant(){
  maxPlayers = 2;
  activePlayers[0] = 0; //reset everybody to dead
  activePlayers[1] = 0; //reset everybody to dead
  activePlayers[2] = 0; //reset everybody to dead
  activePlayers[3] = 0; //reset everybody to dead
  activePlayers[4] = 0; //reset everybody to dead
  activePlayers[5] = 0; //reset everybody to dead
  player1Status = false; //All players default to dead
  player2Status = false; //All players default to dead 
  player3Status = false; //All players default to dead
  player4Status = false; //All players default to dead
  player5Status = false; //All players default to dead
  player6Status = false; //All players default to dead
  whoseTurn = 0; // start with player 1's turn
  activePlayer=0; //Player 1 is the active player
  playerBegin[0]=twoHeadedGiantArrayBegin[0];//initial game Mode defaults to free for all
  playerBegin[1]=twoHeadedGiantArrayBegin[1];
  playerBegin[2]=twoHeadedGiantArrayBegin[2];
  playerBegin[3]=twoHeadedGiantArrayBegin[3];
  playerBegin[4]=twoHeadedGiantArrayBegin[4];
  playerBegin[5]=twoHeadedGiantArrayBegin[5];
  playerEnd[0]=twoHeadedGiantArrayEnd[0];
  playerEnd[1]=twoHeadedGiantArrayEnd[1];
  playerEnd[2]=twoHeadedGiantArrayEnd[2];
  playerEnd[3]=twoHeadedGiantArrayEnd[3];
  playerEnd[4]=twoHeadedGiantArrayEnd[4];
  playerEnd[5]=twoHeadedGiantArrayEnd[5];
  //resetLEDsToActivePlayerView(playerBegin[0],playerEnd[0]); Don't need to do this cause the marqueue is still running
}

void initializeSixPlayerGame(){
  maxPlayers = 6;
  activePlayers[0] = 0; //reset everybody to dead
  activePlayers[1] = 0; //reset everybody to dead
  activePlayers[2] = 0; //reset everybody to dead
  activePlayers[3] = 0; //reset everybody to dead
  activePlayers[4] = 0; //reset everybody to dead
  activePlayers[5] = 0; //reset everybody to dead
  player1Status = false; //All players default to dead
  player2Status = false; //All players default to dead 
  player3Status = false; //All players default to dead
  player4Status = false; //All players default to dead
  player5Status = false; //All players default to dead
  player6Status = false; //All players default to dead
  whoseTurn = 0; // start with player 1's turn
  activePlayer=0; //Player 1 is the active player
  playerBegin[0]=sixPlayerGameArrayBegin[0];//initial game Mode defaults to free for all
  playerBegin[1]=sixPlayerGameArrayBegin[1];
  playerBegin[2]=sixPlayerGameArrayBegin[2];
  playerBegin[3]=sixPlayerGameArrayBegin[3];
  playerBegin[4]=sixPlayerGameArrayBegin[4];
  playerBegin[5]=sixPlayerGameArrayBegin[5];
  playerEnd[0]=sixPlayerGameArrayEnd[0];
  playerEnd[1]=sixPlayerGameArrayEnd[1];
  playerEnd[2]=sixPlayerGameArrayEnd[2];
  playerEnd[3]=sixPlayerGameArrayEnd[3];
  playerEnd[4]=sixPlayerGameArrayEnd[4];
  playerEnd[5]=sixPlayerGameArrayEnd[5];
  //resetLEDsToActivePlayerView(playerBegin[0],playerEnd[0]); Don't need to do this cause the marqueue is still running
}

void modeGamePlayLoop(){
  updatePlayersStatus(); //toggle player active/eliminated

  //see if any player has tapped the button (if so pass turn)
  if(player1.tapped()||player2.tapped()||player3.tapped()||player4.tapped()||passTurnButton1.tapped()||passTurnButton2.tapped()){ 
     passTurn(25);  //18 is good
  }

  //After 1 second has passed update the LEDs to show the active player
  if(millis()-1000>=durationSincePlayerJoinLED&&ledsWereUpdated){ 
    resetLEDsToActivePlayerView(playerBegin[whoseTurn],playerEnd[whoseTurn]);
    ledsWereUpdated=false;
  }
  breathing(nonPlayerLeds[0],nonPlayerLeds[1],18); //Run the breathing animation for the centerLEDs
}

void setDeadPlayer(int start,int end){
  for(int i=start;i<=end;i++){
    strip.setPixelColor(i, strip.Color(50,0,50));
  }
  strip.show();
}

void setActivePlayer(int beginning,int end){
  ///strip.clear();  //Not sure if I need this. It was initially added to clean up any mess from testing?
  for(int i=beginning;i<=end;i++){
    strip.setPixelColor(i, strip.Color(255,0,0));
  }
  strip.show();
}

void passTurn(int wait){
  activePlayer=whoseTurn; //set the active player to the player whose turn it is. (used for LED animations)

  findWhoIsNext(); //Increments the whoseTurn variable to the next alive player
  if(activePlayer!=whoseTurn){ //If activePlayer ==whoseTurn after calling findWhoIsNext then there is only 1 active player
      flowInAndOut(playerBegin[activePlayer],playerEnd[activePlayer],playerBegin[whoseTurn],playerEnd[whoseTurn],wait);
  }
 durationSincePlayerJoinLED=millis();//update the millis to prevent this function from being called repeatedly
}

void findWhoIsNext(){ // This nexts through all remaining alive players and sets whoseTurn to that
  int count=0; //used to prevent getting stuck in an infinite loop if all players are eliminated.
  do{
    whoseTurn=whoseTurn+1;
    if(whoseTurn==maxPlayers){//don't allow the turn to become greater than the total players
      whoseTurn=0;
    }
    count=count+1;
  }while(activePlayers[whoseTurn]==0&&count<maxPlayers);
}

void flowOut(int start, int end, int wait){
  for(int i = start;i<=end;i++){
    strip.setPixelColor(i, strip.Color(100,0,0));
    strip.show();
    delay(wait);
  }
  for(int i = start;i<=end;i++){
    strip.setPixelColor(i, strip.Color(0,0,0));
    strip.show();
    delay(wait);
  }
}

void flowIn(int start, int end, int wait){
  for(int i = start;i<=end;i++){
    strip.setPixelColor(i, strip.Color(100,0,0));
    strip.show();
    delay(wait);
  }
  for(int i = start;i<=end;i++){
    strip.setPixelColor(i, strip.Color(255,0,0));
    strip.show();
    delay(wait);

  }
}

void flowInAndOut(int startA, int endA, int startB, int endB, int wait){
  int gap=0; //initialize

  if(startB<endA){
    gap=(endOfPlayerLeds-endA); // This will allow the leds to flow through the last players if they are dead.
  }else{
    gap=startB-endA; // set the number of leds between players so the leds can light up between the assigned player spots
  }

  int j = gap; // initialize j to assist with the flow out dimming.

  if(debug){
    Serial.print("gap = ");
    Serial.print(gap);
    Serial.print(", ");
    Serial.print("startA = ");
    Serial.print(startA);
    Serial.print(", ");
    Serial.print("endA = ");
    Serial.print(endA);
    Serial.print(", ");
    Serial.print("startB = ");
    Serial.print(startB);
    Serial.print(", ");
    Serial.print("endB = ");
    Serial.print(endB);
    Serial.println(", ");
  }


  for (int i=0;i<=(endA-startA+gap);i++){
    if(i<((endA-startA+gap)/2)){
      strip.setPixelColor(startA+i, strip.Color(100,0,0));//set active player dimmer
      if(startB-gap+i<=0){  //checking for negative values which occur when wrapping around the end of the strip. Thus the need for subtracking from the end of the strip instaed of subtracking from the starting player which is led 0
        strip.setPixelColor(endOfPlayerLeds-gap+i, strip.Color(255,0,0)); //set new player on
      }else{
        strip.setPixelColor(startB-gap+i, strip.Color(255,0,0)); //set new player on
      }
      
      if(debug){
        Serial.print("startA+i (dim)= ");
        Serial.print(startA+i);
        if(startB-gap+i<=0){
          Serial.print(",  endOfPlayerLeds-gap+i (On)=  ");
          Serial.println(endOfPlayerLeds-gap+i);
        }else{
          Serial.print(",  startB-gap+i (On)=  ");
          Serial.println(startB-gap+i);
        }
      }//end debug if

    } else {
      strip.setPixelColor(startA+i, strip.Color(100,0,0));//set active player dimmer
      strip.setPixelColor(startB-gap+i, strip.Color(255,0,0)); //set new player on
      strip.setPixelColor(startA-gap+j, strip.Color(0,0,0));//set active player off
      
      if(debug){
        Serial.print("startA+i (dim)= ");
        Serial.print(startA+i);
        Serial.print(",  startB-gap+i (On)=  ");
        Serial.print(startB-gap+i);
        Serial.print(", startA-gap+j (off)= ");
        Serial.println(startA-gap+j);
      }
      j++; // increment j to keep turning off active player
    }
    strip.show();
    delay(wait); 
  }

  if(debug){
    Serial.println("Start of Second for loop");
    Serial.print("j = ");
    Serial.print(j);
    Serial.print("  endA = ");
    Serial.print(endA);
    Serial.print("  startA = ");
    Serial.print(startA);
    Serial.print("  gap = ");
    Serial.print(gap);
    Serial.print("  endA-startA+gap = ");
    Serial.println(endA-startA+gap);
    delay(2000); //adding in extra delay for debugging. 
  } 
  
  for (int i=j;i<=(endA-startA+gap);i++){//the endA-startA gives me the number Leds total
    strip.setPixelColor(startA-gap+i, strip.Color(0,0,0));//set active player off
    strip.setPixelColor(startB-gap+i, strip.Color(255,0,0)); //set new player on
    strip.show();
    //delay(wait+(i*20));
    delay(wait);
    
    if(debug){
      Serial.print("startA-gap+i (off) = ");
      Serial.print(startA-gap+i);
      Serial.print(", startB-gap+i (On)= ");
      Serial.println(startB-gap+i);
    }
  }
  if(debug){
    Serial.println("Start of Third for loop");
    delay(2000); //adding in extra delay for debugging. 
  }
  
  for(int i=0;i<gap;i++){
    strip.setPixelColor(endA+i,strip.Color(0,0,0));
    //strip.setPixelColor(((endA-startA+gap)/2)+i,strip.Color(0,255,255));
    strip.show();
    delay(wait);

    if(debug){
      Serial.print("endA+i (off)= ");
      Serial.println(endA+i);
    }
  }
  if(debug){
    delay(2000); //adding in extra delay for debugging. 
  }
  
  resetLEDsToActivePlayerView(playerBegin[whoseTurn],playerEnd[whoseTurn]);
}

void resetLEDsToActivePlayerView(int start, int end){
  strip.clear(); // clear the whole strip so we have a fresh start
  for(int i=start; i<=end; i++) {    
    strip.setPixelColor(i, strip.Color(255,   0,   0));         //  Set pixel's color (in RAM)
  }
  strip.show();                          //  Update strip 
  resetDeadPlayersLeds(true);//set to true so I skip the strip.clear
}

void resetDeadPlayersLeds(int skip){
  if(skip==false){//I want to skip clearing the whole strip when calling after I've already called resetLEDsToActivePlayerView
    strip.clear(); // clear the whole strip so we have a fresh start
  }
  for(int i=0;i<maxPlayers;i++){ 
    if(activePlayers[i]==0){
      setDeadPlayer(playerBegin[i],playerEnd[i]);
    }
  }
}

void deathAnimation(int player){
  for(int i=0; i<=255; i++){//These two for loops set all LEDs to white
    for(int j=0; j<strip.numPixels(); j++){
      strip.setPixelColor(j, strip.Color(i,i,i));   //  Set pixel's color to white in a fade up (in RAM)
    }
    strip.show();
    delay(3);
  }

  shuffleArray(randomList,strip.numPixels());
  for(int i=0;i<strip.numPixels();i++){ // Randomly set all of the strip to red
    strip.setPixelColor(randomList[i],strip.Color(255,0,0));
    strip.show();
    delay(11);
  }
  delay(200);

  int wasUpdated = false;
  for(int i=0,j=strip.numPixels() ;i<strip.numPixels();i++,j--){ //Make all of the red flow to the dead player
    if(i<playerBegin[player]){
      strip.setPixelColor(i,strip.Color(255,255,255));
      wasUpdated=true;
    }
    if(j>playerEnd[player]){
      strip.setPixelColor(j,strip.Color(255,255,255));
      wasUpdated=true;
    }
    if(wasUpdated){ //if either of the two above if statements triggered then update the led strip and wait a moment
      strip.show();
      delay(30);
    }
  }

  for(int i=0;i<255;i=i+10){//Cause the now red dead player to fade to purple
    for(int j=playerBegin[player];j<=playerEnd[player];j++){
      strip.setPixelColor(j,strip.Color(255,0,i));
    }
    strip.show();
    delay(50);
  }
  delay(500); //Wait a moment for the death to sink in :D 

  pixelCycle=0; //reset pixelCycle so the breathing can start over from 0
}

void shuffleArray(int * array, int size)
{
  int last = 0;
  int temp = array[last];
  for (int i=0; i<size; i++)
  {
    int index = random(size);
    array[last] = array[index];
    last = index;
  }
  array[last] = temp;
}


