void Game_menu_credits(){

  
  WiiDash_spritetext(rmode->viWidth/2,200+20*0,(char*)"v0.50 Programming and graphics by ThatOtherPerson",2);
  WiiDash_spritetext(rmode->viWidth/2,200+20*1,(char*)"thatotherdev.wordpress.com",2);
  
  WiiDash_spritetext(rmode->viWidth/2,200+20*3,(char*)"v0.55 enhancements by Carl Lefrançois",2);
  
  WiiDash_spritetext(rmode->viWidth/2,200+20*5,(char*)"Dance, NOW! music by RekcahDam",2);
  WiiDash_spritetext(rmode->viWidth/2,200+20*6,(char*)"rekcahdam.blogspot.com",2);
  
  if(WiiDash_button(rmode->viWidth-100-40,rmode->viHeight-10-40,100,10,0,1,(char*)"Back"))gamestate=0;
}
