int highscore0;
int highscore1;
int highscore2;
int highscore3;
int highscore4;
int highscore;

void Game_menu_score(){

	char temptext[100];
	int y=45;
	int points=boo*0+good*7+perfect*10+longestcombo*1;
	int maxpoints=(boo+good+perfect)*11;
	int score=(int)((double)points/(double)maxpoints*100);
	
	if(gamestatechange){
		highscore=0;
		if(difficulty==0){highscore=highscore0;if(score>highscore0)highscore0=score;}
		if(difficulty==1){highscore=highscore1;if(score>highscore1)highscore1=score;}
		if(difficulty==2){highscore=highscore2;if(score>highscore2)highscore2=score;}
		if(difficulty==3){highscore=highscore3;if(score>highscore3)highscore3=score;}
		if(difficulty==4){highscore=highscore4;if(score>highscore4)highscore4=score;}
		sprintf(temptext,"%s%s%s","Music/",songfilename,".score");
		ofstream outdata;
		outdata.open(temptext);
		outdata << highscore0 << endl;
		outdata << highscore1 << endl;
		outdata << highscore2 << endl;
		outdata << highscore3 << endl;
		outdata << highscore4 << endl;
		outdata.close();
	}

	sprintf(temptext,"%s",songfilename);
	sprintf(temptext,"%s",charpartial(temptext,0,lownumber(strlen(temptext)-4,50)));
	WiiDash_spritetext(25,y,(char*)temptext,1);

	y+=20;
	if(difficulty==0)WiiDash_spritetext(25,y,(char*)"beginner",1);
	if(difficulty==1)WiiDash_spritetext(25,y,(char*)"easy",1);
	if(difficulty==2)WiiDash_spritetext(25,y,(char*)"medium",1);
	if(difficulty==3)WiiDash_spritetext(25,y,(char*)"hard",1);
	if(difficulty==4)WiiDash_spritetext(25,y,(char*)"challenge",1);

	y+=30;
	WiiDash_spritetext(215,y,(char*)"BOO",1);
	sprintf(temptext,"%d",boo);
	WiiDash_spritetext(425,y,temptext,3);
	y+=20;
	WiiDash_spritetext(215,y,(char*)"GOOD",1);
	sprintf(temptext,"%d",good);
	WiiDash_spritetext(425,y,temptext,3);
	y+=20;
	WiiDash_spritetext(215,y,(char*)"PERFECT",1);
	sprintf(temptext,"%d",perfect);
	WiiDash_spritetext(425,y,temptext,3);
	y+=20;
	WiiDash_spritetext(215,y,(char*)"LONGEST COMBO",1);
	sprintf(temptext,"%d",longestcombo);
	WiiDash_spritetext(425,y,temptext,3);

	y+=30;
	WiiDash_spritetext(215,y,(char*)"YOUR POINTS",1);
	sprintf(temptext,"%d",points);
	WiiDash_spritetext(425,y,temptext,3);
	y+=20;
	WiiDash_spritetext(215,y,(char*)"MAX POINTS",1);
	sprintf(temptext,"%d",maxpoints);
	WiiDash_spritetext(425,y,temptext,3);

	y+=30;
	WiiDash_spritetext(215,y,(char*)"SCORE",1);
	sprintf(temptext,"%d%s",score,"%");
	WiiDash_spritetext(425,y,temptext,3);
	y+=20;
	WiiDash_spritetext(215,y,(char*)"HIGH SCORE",1);
	sprintf(temptext,"%d%s",highscore,"%");
	WiiDash_spritetext(425,y,temptext,3);

	if(WiiDash_button(640-100-40,480-10-40,100,10,0,1,(char*)"Back"))gamestate=4;
}