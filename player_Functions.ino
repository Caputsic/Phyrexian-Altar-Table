//refresh the timing on all the buttons
void updatePlayersButtons(){ 
  player1.update(); //Master control board player 1 functions (Log into game, pass turn, and Kill player)
  player2.update(); //Master control board player 1 functions
  player3.update(); //Master control board player 1 functions
  player4.update(); //Master control board player 1 functions
  modeButton.update();  // The button for switching between display modes
  passTurnButton1.update(); // The player's button for passing the turn
  passTurnButton2.update(); // The player's button for passing the turn
}

// if a button is held flip player status from active to inactive and vice versa
void updatePlayersStatus(){ 
  if(player1.held()){
    if(gameMode==freeForAll){
      updatePlayer1();  
    }else if(gameMode==TwoHeadedGiant){
      updatePlayer1();
    }else if(gameMode==SixPlayerGame){
      updatePlayer1();
    }
  }
  
  if(player2.held()){
    if(gameMode==freeForAll){
      updatePlayer2();  
    }else if(gameMode==TwoHeadedGiant){
      updatePlayer1(); //buttons for player 1 and player 2 are both now team player 1
    }else if(gameMode==SixPlayerGame){
      updatePlayer2(); 
    }
  }

 if(player3.held()){
    if(gameMode==freeForAll){
      updatePlayer3();  
    }else if(gameMode==TwoHeadedGiant){
      updatePlayer2(); //buttons for player 3 and player 4 are both now team player 2
    }else if(gameMode==SixPlayerGame){
      updatePlayer4(); //In a six player game holding the player 3 button sets the fourth player
    }
  }

  if(player4.held()){
    if(gameMode==freeForAll){
      updatePlayer4();  
    }else if(gameMode==TwoHeadedGiant){
      updatePlayer2(); //buttons for player 3 and player 4 are both now team player 2
    }else if(gameMode==SixPlayerGame){
      updatePlayer5();//In a six player game holding the player 4 button sets the fifth player
    }
  }

  if(player1.doubleTapped()||player4.doubleTapped()){ //when a button on the player 1 end of the table is hit, activate player 6
    if(gameMode==SixPlayerGame){ //if not in a 6 player game don't do anything
      updatePlayer6(); 
    }
  } 

  if(player2.doubleTapped()||player3.doubleTapped()){ //when a button on the player 2 end of the table is hit, activate player 3
    if(gameMode==SixPlayerGame){ //if not in a 6 player game don't do anything
      updatePlayer3(); 
    }
  } 
}

void updatePlayer1(){
  if(player1Status){ //if active then set to eliminated
    player1Status = false;
    activePlayers[p1]=0; //update array to deactivate player
    //Serial.println("Player 1 deactivated");
    deathAnimation(p1);//call the death animation :D
    setDeadPlayer(playerBegin[p1],playerEnd[p1]); //Update LEDs to Deactivated
    if(whoseTurn==p1){
      findWhoIsNext();//Sets the active player to the next alive person
    } 
    ledsWereUpdated=true;
  }else {//else set player to active
    player1Status = true;
    activePlayers[p1]=1; //update array to activate player
    //Serial.println("Player 1 ACTIVATED");
    setActivePlayer(playerBegin[p1],playerEnd[p1]); //Update LEDs to Activated
    durationSincePlayerJoinLED=millis();
    ledsWereUpdated=true;
  }
}

void updatePlayer2(){
  if(player2Status){  //if active then set to eliminated
    player2Status = false;
    activePlayers[p2]=0; //update array to deactivate player
    // Serial.println("Player 2 deactivated");
    deathAnimation(p2);//call the death animation :D
    setDeadPlayer(playerBegin[p2],playerEnd[p2]); //Update LEDs to Deactivated
    if(whoseTurn==p2){
      findWhoIsNext();//Sets the active player to the next alive person
    } 
    ledsWereUpdated=true;
  }else {//else set player to active
    player2Status = true;
    activePlayers[p2]=1; //update array to activate player
    //Serial.println("Player 2 ACTIVATED");
    setActivePlayer(playerBegin[p2],playerEnd[p2]); //Update LEDs to Activated
    durationSincePlayerJoinLED=millis();
    ledsWereUpdated=true;
  }
}

void updatePlayer3(){
  if(player3Status){ //if active then set to eliminated
    player3Status = false;
    activePlayers[p3]=0; //update array to deactivate player
    //Serial.println("Player 3 deactivated");
    deathAnimation(p3);//call the death animation :D
    setDeadPlayer(playerBegin[p3],playerEnd[p3]); //Update LEDs to Deactivated
    if(whoseTurn==p3){
      findWhoIsNext();//Sets the active player to the next alive person
    } 
    ledsWereUpdated=true;
  }else {//else set player to active
    player3Status = true;
    activePlayers[p3]=1; //update array to activate player
    //Serial.println("Player 3 ACTIVATED");
    setActivePlayer(playerBegin[p3],playerEnd[p3]); //Update LEDs to Activated
    durationSincePlayerJoinLED=millis();
    ledsWereUpdated=true;
  }
}

void updatePlayer4(){
  if(player4Status){  //if active then set to eliminated
    player4Status = false;
    activePlayers[p4]=0; //update array to deactivate player
    //Serial.println("Player 4 deactivated");
    deathAnimation(p4);//call the death animation :D
    setDeadPlayer(playerBegin[p4],playerEnd[p4]); //Update LEDs to Deactivated
    if(whoseTurn==p4){
      findWhoIsNext();//Sets the active player to the next alive person
    } 
    ledsWereUpdated=true;
  }else {//else set player to active
    player4Status = true;
    activePlayers[p4]=1; //update array to activate player
    //Serial.println("Player 4 ACTIVATED");
    setActivePlayer(playerBegin[p4],playerEnd[p4]); //Update LEDs to Activated
    durationSincePlayerJoinLED=millis();
    ledsWereUpdated=true;
  }
}

void updatePlayer5(){
  if(player5Status){  //if active then set to eliminated
    player5Status = false;
    activePlayers[p5]=0; //update array to deactivate player
    //Serial.println("Player 5 deactivated");
    deathAnimation(p5);//call the death animation :D
    setDeadPlayer(playerBegin[p5],playerEnd[p5]); //Update LEDs to Deactivated
    if(whoseTurn==p5){
      findWhoIsNext();//Sets the active player to the next alive person
    } 
    ledsWereUpdated=true;
  }else {//else set player to active
    player5Status = true;
    activePlayers[p5]=1; //update array to activate player
    //Serial.println("Player 5 ACTIVATED");
    setActivePlayer(playerBegin[p5],playerEnd[p5]); //Update LEDs to Activated
    durationSincePlayerJoinLED=millis();
    ledsWereUpdated=true;
  }
}

void updatePlayer6(){
  if(player6Status){  //if active then set to eliminated
    player6Status = false;
    activePlayers[p6]=0; //update array to deactivate player
    //Serial.println("Player 6 deactivated");
    deathAnimation(p6);//call the death animation :D
    setDeadPlayer(playerBegin[p6],playerEnd[p6]); //Update LEDs to Deactivated
    if(whoseTurn==p6){
      findWhoIsNext();//Sets the active player to the next alive person
    } 
    ledsWereUpdated=true;
  }else {//else set player to active
    player6Status = true;
    activePlayers[p6]=1; //update array to activate player
    //Serial.println("Player 6 ACTIVATED");
    setActivePlayer(playerBegin[p6],playerEnd[p6]); //Update LEDs to Activated
    durationSincePlayerJoinLED=millis();
    ledsWereUpdated=true;
  }
}

