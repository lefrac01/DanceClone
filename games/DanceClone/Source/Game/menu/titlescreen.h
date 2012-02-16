void Game_menu_titlescreen(){

	apply_surface(640/2-300/2, 30, titleimage, screen, NULL);
	
	if(WiiDash_button(320,55+4*40,600,10,1,1,(char*)"Play")){gamestate=3;}
	if(WiiDash_button(320,55+5*40,600,10,1,1,(char*)"Creditzzz")){gamestate=1;}

}
