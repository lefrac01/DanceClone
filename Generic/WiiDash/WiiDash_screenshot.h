void WiiDash_savebmpscreenshot(){
	#ifdef WII
	ifstream test;char filename[64];int filenumber=1;
	sprintf(filename,"sd:/screenshot%d.bmp",filenumber);
	test.open(filename);test.close();
	if(!test.fail()){
		while(!test.fail()){
			filenumber=filenumber+1;
			sprintf(filename,"sd:/screenshot%d.bmp",filenumber);
			test.open(filename);test.close();
		}
	}
	SDL_SaveBMP(WDbackground,filename);
	#endif
	#ifdef WIN
	ifstream test;char filename[64];int filenumber=1;
	sprintf(filename,"screenshot%d.bmp",filenumber);
	test.open(filename);test.close();
	if(!test.fail()){
		while(!test.fail()){
			filenumber=filenumber+1;
			sprintf(filename,"screenshot%d.bmp",filenumber);
			test.open(filename);test.close();
		}
	}
	SDL_SaveBMP(WDbackground,filename);
	#endif
}

void WiiDash_savepngscreenshot(){
	#ifdef WII
	ifstream test;char filename[64];int filenumber=1;
	sprintf(filename,"sd:/screenshot%d.png",filenumber);
	test.open(filename);test.close();
	if(!test.fail()){
		while(!test.fail()){
			filenumber=filenumber+1;
			sprintf(filename,"sd:/screenshot%d.png",filenumber);
			test.open(filename);test.close();
		}
	}
	IMG_SavePNG((char*)filename,WDbackground,Z_BEST_COMPRESSION);
	#endif
}