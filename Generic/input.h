int cursorx[4];
int cursory[4];

const int keycount=500;
int keystate[keycount];
const int mousecount=10;
int mousestate[mousecount];

#ifdef WII
	bool wiimoteactive[4];
	u32 WiiButtonsHeld[4];
	u32 WiiButtonsDown[4];
	u32 WiiButtonsUp[4];
	u16 GCButtonsHeld[4];
	u16 GCButtonsDown[4];
	u16 GCButtonsUp[4];
	ir_t ir[4];
	expansion_t expans[4];
	#ifdef USEACCELEROMETER
	gforce_t gforce[4];
	#endif
	s8 HWButton = -1;
	void WiiResetPressed(){HWButton = SYS_RETURNTOMENU;}
	void WiiPowerPressed(){HWButton = SYS_POWEROFF;}
	void WiimotePowerPressed(s32 chan){HWButton = SYS_POWEROFF;}
#endif

void updatecontrolinput(){
	SDL_Event event;

	for(int a=0;a<keycount;a++)if(keystate[a]){keystate[a]=1;}else{keystate[a]=0;}
	while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_KEYDOWN)))
		for(int a=0;a<keycount;a++)if(event.key.keysym.sym==a)keystate[a]=2;
	while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_KEYUP)))
		for(int a=0;a<keycount;a++)if(event.key.keysym.sym==a)keystate[a]=0;

	for(int a=0;a<mousecount;a++)if(mousestate[a]){mousestate[a]=1;}else{mousestate[a]=0;}
	while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN)))
		for(int a=0;a<mousecount;a++)if(event.button.button==a)mousestate[a]=2;
	while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEBUTTONUP)))
		for(int a=0;a<mousecount;a++)if(event.button.button==a)mousestate[a]=0;

	#ifdef WIN
		Uint8 mousestate=SDL_GetMouseState(&cursorx[0],&cursory[0]);
	#endif
	#ifdef WII
		WPAD_ScanPads();
		u32 ext;
		wiimoteactive[0]=(WPAD_Probe(WPAD_CHAN_0,&ext)!=WPAD_ERR_NO_CONTROLLER);
		wiimoteactive[1]=(WPAD_Probe(WPAD_CHAN_1,&ext)!=WPAD_ERR_NO_CONTROLLER);
		wiimoteactive[2]=(WPAD_Probe(WPAD_CHAN_2,&ext)!=WPAD_ERR_NO_CONTROLLER);
		wiimoteactive[3]=(WPAD_Probe(WPAD_CHAN_3,&ext)!=WPAD_ERR_NO_CONTROLLER);
		WiiButtonsHeld[0]=WPAD_ButtonsHeld(WPAD_CHAN_0);
		WiiButtonsDown[0]=WPAD_ButtonsDown(WPAD_CHAN_0);
		WiiButtonsUp[0]=WPAD_ButtonsUp(WPAD_CHAN_0);
		WiiButtonsHeld[1]=WPAD_ButtonsHeld(WPAD_CHAN_1);
		WiiButtonsDown[1]=WPAD_ButtonsDown(WPAD_CHAN_1);
		WiiButtonsUp[1]=WPAD_ButtonsUp(WPAD_CHAN_1);
		WiiButtonsHeld[2]=WPAD_ButtonsHeld(WPAD_CHAN_2);
		WiiButtonsDown[2]=WPAD_ButtonsDown(WPAD_CHAN_2);
		WiiButtonsUp[2]=WPAD_ButtonsUp(WPAD_CHAN_2);
		WiiButtonsHeld[3]=WPAD_ButtonsHeld(WPAD_CHAN_3);
		WiiButtonsDown[3]=WPAD_ButtonsDown(WPAD_CHAN_3);
		WiiButtonsUp[3]=WPAD_ButtonsUp(WPAD_CHAN_3);
		WPAD_IR(WPAD_CHAN_0,&ir[0]);
		WPAD_IR(WPAD_CHAN_1,&ir[1]);
		WPAD_IR(WPAD_CHAN_2,&ir[2]);
		WPAD_IR(WPAD_CHAN_3,&ir[3]);
		WPAD_Expansion(WPAD_CHAN_0,&expans[0]);
		WPAD_Expansion(WPAD_CHAN_1,&expans[1]);
		WPAD_Expansion(WPAD_CHAN_2,&expans[2]);
		WPAD_Expansion(WPAD_CHAN_3,&expans[3]);
		#ifdef USEACCELEROMETER
		WPAD_GForce(0,&gforce[0]);
		WPAD_GForce(1,&gforce[1]);
		WPAD_GForce(2,&gforce[2]);
		WPAD_GForce(3,&gforce[3]);
		#endif
		PAD_ScanPads();
		GCButtonsHeld[0]=PAD_ButtonsHeld(0);
		GCButtonsDown[0]=PAD_ButtonsDown(0);
		GCButtonsUp[0]=PAD_ButtonsUp(0);
		SYS_SetResetCallback(WiiResetPressed);
		SYS_SetPowerCallback(WiiPowerPressed);
		WPAD_SetPowerButtonCallback(WiimotePowerPressed);
		if(HWButton != -1){SYS_ResetSystem(HWButton, 0, 0);}
		if(ir[0].valid){cursorx[0]=ir[0].x*1.25-80;cursory[0]=ir[0].y*1.25-60;}
		if(ir[1].valid){cursorx[1]=ir[1].x*1.25-80;cursory[1]=ir[1].y*1.25-60;}
		if(ir[2].valid){cursorx[2]=ir[2].x*1.25-80;cursory[2]=ir[2].y*1.25-60;}
		if(ir[3].valid){cursorx[3]=ir[3].x*1.25-80;cursory[3]=ir[3].y*1.25-60;}
		for(int a=0;a<4;a++){
			if(cursorx[a]<0)cursorx[a]=0;
			if(cursorx[a]>640)cursorx[a]=640;
			if(cursory[a]<0)cursory[a]=0;
			if(cursory[a]>480)cursory[a]=480;
		}
	#endif
	
}
