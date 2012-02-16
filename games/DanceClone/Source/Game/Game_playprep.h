void Game_playprep(){
	char temptext[100];	
	sprintf(temptext,"%s%s","Music/",songfilename);	

	#ifdef WIN
		music = Mix_LoadMUS(temptext);
	#endif
	#ifdef WII
		ASND_Init();
		MP3Player_Init();
		FILE *BGM = 0;
		long lSize;
		char * buffer;
		//size_t result;
		BGM = fopen(temptext, "rb");
		fseek (BGM , 0 , SEEK_END);
		lSize = ftell (BGM);
		rewind (BGM);
		buffer = (char*) malloc (sizeof(char)*lSize);
		//result = fread (buffer,1,lSize,BGM);
		fread (buffer,1,lSize,BGM);
		fclose(BGM);
	#endif
	
	while(n_arrows)deletearrow(0);
	while(n_ratings)deleterating(0);
	
	sprintf(temptext,"%s%s%s","Music/",songfilename,".dc");
	ifstream indata;indata.open(temptext);
	bool start=0;
	for(int a=0; n_arrows<maxarrows-10 && indata.good(); a++){
		if(start==0){
			indata >> temptext;
			if(difficulty==0 && charmatchstart((char*)temptext,(char*)"<startbeginner>"))start=1;
			if(difficulty==1 && charmatchstart((char*)temptext,(char*)"<starteasy>"))start=1;
			if(difficulty==2 && charmatchstart((char*)temptext,(char*)"<startmedium>"))start=1;
			if(difficulty==3 && charmatchstart((char*)temptext,(char*)"<starthard>"))start=1;
			if(difficulty==4 && charmatchstart((char*)temptext,(char*)"<startchallenge>"))start=1;
		}else{
			arrows[n_arrows] = new arrow(0,0,0);
			indata >> arrows[n_arrows]->direction;
			indata >> arrows[n_arrows]->time;
			indata >> arrows[n_arrows]->length;
			indata >> temptext;
			n_arrows++;
		}
	}delete arrows[n_arrows];n_arrows=n_arrows-1;
	indata.close();
	
	#ifdef WIN
		Mix_PlayMusic(music,0);
	#endif
	#ifdef WII
		MP3Player_PlayBuffer(buffer, lSize, NULL);
		free(buffer);
	#endif
	
	longestcombo=0;
	combo=0;
	boo=0;
	good=0;
	perfect=0;

	songstarttime=WDgametime+(SDL_GetTicks()-WDruntime);
	songtime=0;
	upcontrol=0;
	downcontrol=0;
	leftcontrol=0;
	rightcontrol=0;
	gamestate=8;

}
