int highscore0;
int highscore1;
int highscore2;
int highscore3;
int highscore4;
int highscore;

extern play_data current_play_data;

void Game_menu_score(){
// +1 for kiss ;)  
/*
  string temp_text;
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
    
    ofstream outdata;
    outdata.open(current_song.score_path().c_str());
   
    outdata << highscore0 << endl;
    outdata << highscore1 << endl;
    outdata << highscore2 << endl;
    outdata << highscore3 << endl;
    outdata << highscore4 << endl;
    outdata.close();
  }

  temp_text = current_song.name();
  temp_text = temp_text.substr(0, lownumber(temp_text.size(), 50));
  WiiDash_spritetext(25,y,(char*)temp_text.c_str(),1);

  y+=20;
  if(difficulty==0)WiiDash_spritetext(25,y,(char*)"beginner",1);
  if(difficulty==1)WiiDash_spritetext(25,y,(char*)"easy",1);
  if(difficulty==2)WiiDash_spritetext(25,y,(char*)"medium",1);
  if(difficulty==3)WiiDash_spritetext(25,y,(char*)"hard",1);
  if(difficulty==4)WiiDash_spritetext(25,y,(char*)"challenge",1);

  y+=30;
  char buf[100];
  WiiDash_spritetext(215,y,(char*)"BOO",1);
  sprintf(buf,"%d",boo);
  WiiDash_spritetext(425,y,buf,3);
  y+=20;
  WiiDash_spritetext(215,y,(char*)"GOOD",1);
  sprintf(buf,"%d",good);
  WiiDash_spritetext(425,y,buf,3);
  y+=20;
  WiiDash_spritetext(215,y,(char*)"PERFECT",1);
  sprintf(buf,"%d",perfect);
  WiiDash_spritetext(425,y,buf,3);
  y+=20;
  WiiDash_spritetext(215,y,(char*)"LONGEST COMBO",1);
  sprintf(buf,"%d",longestcombo);
  WiiDash_spritetext(425,y,buf,3);

  y+=30;
  WiiDash_spritetext(215,y,(char*)"YOUR POINTS",1);
  sprintf(buf,"%d",points);
  WiiDash_spritetext(425,y,buf,3);
  y+=20;
  WiiDash_spritetext(215,y,(char*)"MAX POINTS",1);
  sprintf(buf,"%d",maxpoints);
  WiiDash_spritetext(425,y,buf,3);

  y+=30;
  WiiDash_spritetext(215,y,(char*)"SCORE",1);
  sprintf(buf,"%d%s",score,"%");
  WiiDash_spritetext(425,y,buf,3);
  y+=20;
  WiiDash_spritetext(215,y,(char*)"HIGH SCORE",1);
  sprintf(buf,"%d%s",highscore,"%");
  WiiDash_spritetext(425,y,buf,3);

  if(WiiDash_button(rmode->viWidth-100-40,rmode->viHeight-10-40,100,10,0,1,(char*)"temp no score - Back"))gamestate=4;
*/
  //TODO: multiplayer
  player_data& pd = current_play_data.current_player_data[0];  

  char temptext[100];
  sprintf(temptext,"%d%s",pd.marvellous," MARVELLOUS");
  WiiDash_spritetext(rmode->viWidth/2,440-18*8,temptext,2);
  sprintf(temptext,"%d%s",pd.perfect," PERFECT");
  WiiDash_spritetext(rmode->viWidth/2,440-18*7,temptext,2);
  sprintf(temptext,"%d%s",pd.great," GREAT");
  WiiDash_spritetext(rmode->viWidth/2,440-18*6,temptext,2);
  sprintf(temptext,"%d%s",pd.good," GOOD");
  WiiDash_spritetext(rmode->viWidth/2,440-18*5,temptext,2);
  sprintf(temptext,"%d%s",pd.boo," BOO");
  WiiDash_spritetext(rmode->viWidth/2,440-18*4,temptext,2);
  sprintf(temptext,"%d%s",pd.longest_combo," MAX COMBO");
  WiiDash_spritetext(rmode->viWidth/2,440-18*1,temptext,2);


  if(WiiDash_button(rmode->viWidth-300-40,rmode->viHeight-10-40,100,10,0,1,(char*)"temp no score - Back"))gamestate=4;
}
