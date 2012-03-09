//TODO: implement 2nd player
//
//TODO: implement separate rating for succesfully held arrows 
// currently this adds one perfect

void Game_play_controls(){
/*
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "Game_play_controls() begins" << endl;
    //#log.close();
  }

  // update player controls
  
  u32 WiiButtons1Held = WiiButtonsHeld[0];
  u32 WiiButtons1Down = WiiButtonsDown[0];
  u16 GCButtons1Held = GCButtonsHeld[0];
  u16 GCButtons1Down = GCButtonsDown[0];
  expansion_t WPAD_Expansion1 = expans[0];

  //TODO: multiplayer
  //TODO: leaving in reference to frame_time but 
  // this is to move to a function called from frame() where
  // it will receive frame time there
  player_data& pd = current_play_data.current_player_data[0];
  pd.up_control = pd.up_control - current_play_data.frame_time;
  pd.down_control = pd.down_control - current_play_data.frame_time;
  pd.left_control = pd.left_control - current_play_data.frame_time;
  pd.right_control = pd.right_control - current_play_data.frame_time;


  if(pd.up_control<0)pd.up_control=0;
  if(pd.down_control<0)pd.down_control=0;
  if(pd.left_control<0)pd.left_control=0;
  if(pd.right_control<0)pd.right_control=0;

  #ifdef WIN
  if(keystate[SDLK_UP] && upcontrol==0){upcontrol=1;}
  if(keystate[SDLK_DOWN] && downcontrol==0){downcontrol=1;}
  if(keystate[SDLK_LEFT] && leftcontrol==0){leftcontrol=1;}
  if(keystate[SDLK_RIGHT] && rightcontrol==0){rightcontrol=1;}
  if(keystate[SDLK_UP]==2)upcontrol=125;
  if(keystate[SDLK_DOWN]==2)downcontrol=125;
  if(keystate[SDLK_LEFT]==2)leftcontrol=125;
  if(keystate[SDLK_RIGHT]==2)rightcontrol=125;
  #endif
  
  #ifdef WII

  if(pd.up_control==0 && WiiButtons1Held & WPAD_BUTTON_UP)pd.up_control=1;
  if(pd.down_control==0 && WiiButtons1Held & WPAD_BUTTON_DOWN)pd.down_control=1;
  if(pd.left_control==0 && WiiButtons1Held & WPAD_BUTTON_LEFT)pd.left_control=1;
  if(pd.right_control==0 && WiiButtons1Held & WPAD_BUTTON_RIGHT)pd.right_control=1;
  if(WiiButtons1Down & WPAD_BUTTON_UP)pd.up_control=125;
  if(WiiButtons1Down & WPAD_BUTTON_DOWN)pd.down_control=125;
  if(WiiButtons1Down & WPAD_BUTTON_LEFT)pd.left_control=125;
  if(WiiButtons1Down & WPAD_BUTTON_RIGHT)pd.right_control=125;



  if(pd.up_control==0 && GCButtons1Held & PAD_BUTTON_UP)pd.up_control=1;
  if(pd.down_control==0 && GCButtons1Held & PAD_BUTTON_DOWN)pd.down_control=1;
  if(pd.left_control==0 && GCButtons1Held & PAD_BUTTON_LEFT)pd.left_control=1;
  if(pd.right_control==0 && GCButtons1Held & PAD_BUTTON_RIGHT)pd.right_control=1;
  if(GCButtons1Down & PAD_BUTTON_UP)pd.up_control=125;
  if(GCButtons1Down & PAD_BUTTON_DOWN)pd.down_control=125;
  if(GCButtons1Down & PAD_BUTTON_LEFT)pd.left_control=125;
  if(GCButtons1Down & PAD_BUTTON_RIGHT)pd.right_control=125;



  if(WPAD_Expansion1.type == WPAD_EXP_CLASSIC){
    if(pd.up_control==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_UP)pd.up_control=1;
    if(pd.down_control==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_DOWN)pd.down_control=1;
    if(pd.left_control==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_LEFT)pd.left_control=1;
    if(pd.right_control==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_RIGHT)pd.right_control=1;
    if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_UP)pd.up_control=125;
    if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_DOWN)pd.down_control=125;
    if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_LEFT)pd.left_control=125;
    if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_RIGHT)pd.right_control=125;    
  }

  #endif


  // rate arrows
  // this should be in a separate function or in Game_play()

  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "checking for boo" << endl;
    //#log.close();
  }
  //TODO: recode using current bpm to calculate a max distance.
  //TODO: if arrow has flowed past, consider it still the next ratable
  // arrow until either the max distance(boo rating) or until the next
  // arrow is closer.  this is too allow excluding old arrows before the
  // normal timeout if a stream of closely-placed arrows arrives.
  detect_missed_arrows(0);
  //#for(unsigned int a=player_base_arrow;a<player_arrows.size();a++)if(player_arrows[a].time-songtime<-1000/8)
  //#{
    //#ratearrow(a,0);combo=0;boo=boo+1;
  //#}
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "checking for perfect / good" << endl;
    //#log.close();
  }
  
  //TODO: record rating.  each direction has a "current ratable arrow" 
  // whose y position is analysed relative to a scale calculated based on 
  // current BPM.  separate each control's hit detection statement to 
  // use the current ratable arrow for each column.
  
  
  
  //TODO: fix this.
//  detect_perfect_arrows(0);

  for(int b=0;b<4;b++)
  if((b==0 && leftcontrol==125)||(b==1 && downcontrol==125)||(b==2 && upcontrol==125)||(b==3 && rightcontrol==125))
  {
    for(unsigned int a=player_base_arrow;a<player_arrows.size();a++)if(player_arrows[a].length==0)if(player_arrows[a].direction==b)
    {
      if(player_arrows[a].time-songtime>-1000/15 && player_arrows[a].time-songtime<1000/15)
      {
        ratearrow(a,2);combo=combo+1;perfect=perfect+1;break;
      }
      else if(player_arrows[a].time-songtime>-1000/5 && player_arrows[a].time-songtime<1000/5)
      {
        ratearrow(a,1);combo=combo+1;good=good+1;break;
      }
    }
  }
  
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "checking for perfect on held arrow" << endl;
    //#log.close();
  }
  for(int b=0;b<4;b++)
  if((b==0 && leftcontrol)||(b==1 && downcontrol)||(b==2 && upcontrol)||(b==3 && rightcontrol))
  {
    for(unsigned int a=player_base_arrow;a<player_arrows.size();a++)if(player_arrows[a].length!=0)if(player_arrows[a].direction==b)
    {
      if(player_arrows[a].time-songtime>-1000/5 && player_arrows[a].time-songtime<1000/5)
      {
        player_arrows[a].ypos+=timehaspast;
        player_arrows[a].length-=timehaspast;
        if(player_arrows[a].length<=0)
        {
          ratearrow(a,2);combo=combo+1;perfect=perfect+1;
        }
      }
    }
  }

  */
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "leaving Game_play_controls()" << endl;
    //#log.close();
  }
}
