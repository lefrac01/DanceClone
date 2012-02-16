//TODO implement 2nd player

void Game_play_controls(){

  u32 WiiButtons1Held = WiiButtonsHeld[0];
  u32 WiiButtons1Down = WiiButtonsDown[0];
  u16 GCButtons1Held = GCButtonsHeld[0];
  u16 GCButtons1Down = GCButtonsDown[0];
  expansion_t WPAD_Expansion1 = expans[0];

	upcontrol=upcontrol-timehaspast;
	downcontrol=downcontrol-timehaspast;
	leftcontrol=leftcontrol-timehaspast;
	rightcontrol=rightcontrol-timehaspast;
	
	if(upcontrol<0)upcontrol=0;
	if(downcontrol<0)downcontrol=0;
	if(leftcontrol<0)leftcontrol=0;
	if(rightcontrol<0)rightcontrol=0;
	
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
	if(upcontrol==0 && WiiButtons1Held & WPAD_BUTTON_UP)upcontrol=1;
	if(downcontrol==0 && WiiButtons1Held & WPAD_BUTTON_DOWN)downcontrol=1;
	if(leftcontrol==0 && WiiButtons1Held & WPAD_BUTTON_LEFT)leftcontrol=1;
	if(rightcontrol==0 && WiiButtons1Held & WPAD_BUTTON_RIGHT)rightcontrol=1;
	if(WiiButtons1Down & WPAD_BUTTON_UP)upcontrol=125;
	if(WiiButtons1Down & WPAD_BUTTON_DOWN)downcontrol=125;
	if(WiiButtons1Down & WPAD_BUTTON_LEFT)leftcontrol=125;
	if(WiiButtons1Down & WPAD_BUTTON_RIGHT)rightcontrol=125;
	
	if(upcontrol==0 && GCButtons1Held & PAD_BUTTON_UP)upcontrol=1;
	if(downcontrol==0 && GCButtons1Held & PAD_BUTTON_DOWN)downcontrol=1;
	if(leftcontrol==0 && GCButtons1Held & PAD_BUTTON_LEFT)leftcontrol=1;
	if(rightcontrol==0 && GCButtons1Held & PAD_BUTTON_RIGHT)rightcontrol=1;
	if(GCButtons1Down & PAD_BUTTON_UP)upcontrol=125;
	if(GCButtons1Down & PAD_BUTTON_DOWN)downcontrol=125;
	if(GCButtons1Down & PAD_BUTTON_LEFT)leftcontrol=125;
	if(GCButtons1Down & PAD_BUTTON_RIGHT)rightcontrol=125;
		
	if(WPAD_Expansion1.type == WPAD_EXP_CLASSIC){
		if(upcontrol==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_UP)upcontrol=1;
		if(downcontrol==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_DOWN)downcontrol=1;
		if(leftcontrol==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_LEFT)leftcontrol=1;
		if(rightcontrol==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_RIGHT)rightcontrol=1;
		if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_UP)upcontrol=125;
		if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_DOWN)downcontrol=125;
		if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_LEFT)leftcontrol=125;
		if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_RIGHT)rightcontrol=125;
	}
	#endif

	for(int a=0;a<n_arrows;a++)if(arrows[a]->time-songtime<-1000/8){
		ratearrow(a,0);combo=0;boo=boo+1;a-=1;}

	for(int b=0;b<4;b++)
	if((b==0 && leftcontrol==125)||(b==1 && downcontrol==125)||(b==2 && upcontrol==125)||(b==3 && rightcontrol==125))
	for(int a=0;a<n_arrows;a++)if(arrows[a]->length==0)if(arrows[a]->direction==b){
		if(arrows[a]->time-songtime>-1000/15 && arrows[a]->time-songtime<1000/15){
			ratearrow(a,2);a=a-1;combo=combo+1;perfect=perfect+1;a=n_arrows;
		}else if(arrows[a]->time-songtime>-1000/5 && arrows[a]->time-songtime<1000/5){
			ratearrow(a,1);a=a-1;combo=combo+1;good=good+1;a=n_arrows;
		}
	}

	for(int b=0;b<4;b++)
	if((b==0 && leftcontrol)||(b==1 && downcontrol)||(b==2 && upcontrol)||(b==3 && rightcontrol))
	for(int a=0;a<n_arrows;a++)if(arrows[a]->length!=0)if(arrows[a]->direction==b){
		if(arrows[a]->time-songtime>-1000/5 && arrows[a]->time-songtime<1000/5){
			arrows[a]->time+=timehaspast;
			arrows[a]->length-=timehaspast;
			if(arrows[a]->length<=0){
				ratearrow(a,2);a=a-1;combo=combo+1;perfect=perfect+1;
			}
		}
	}

}