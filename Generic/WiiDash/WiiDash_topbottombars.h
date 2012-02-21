void WiiDash_topbottombars(){
char temptext[100]; 
int trans=WiiDash_onoffslide(0,61);

apply_surface(0,-70-trans,WDtopbottomborderimage,screen,NULL);

sprintf(temptext,"%s%s%s%s%s%s%d",
    WDwday[WDtime->tm_wday],
    " ",
    WDmonth[WDtime->tm_mon],
    " ",
    WDday[WDtime->tm_mday-1],
    " ",
    1900+WDtime->tm_year
    );
WiiDash_spritetext(rmode->viWidth-20,-70-trans+131-43,temptext,3);

strcpy(temptext,"");
if(WDtime->tm_hour>12){
sprintf(temptext,"%s%d",temptext,WDtime->tm_hour-12);}else{
if(WDtime->tm_hour==0){
sprintf(temptext,"%s%d",temptext,12);}else{
sprintf(temptext,"%s%d",temptext,WDtime->tm_hour);}}
strcat(temptext,":");
if(WDtime->tm_min<10)strcat(temptext,"0");
sprintf(temptext,"%s%d",temptext,WDtime->tm_min);
strcat(temptext,":");
if(WDtime->tm_sec<10)strcat(temptext,"0");
sprintf(temptext,"%s%d",temptext,WDtime->tm_sec);
if(WDtime->tm_hour>11){
strcat(temptext," PM");}else{
strcat(temptext," AM");}
WiiDash_spritetext(rmode->viWidth-20,-70-trans+131-23,temptext,3);

apply_surface(0,rmode->viHeight-61+trans,WDtopbottomborderimage,screen,NULL);
WiiDash_spritetext(20,rmode->viHeight-61+trans+3,(char*)WDextracredits,1);
WiiDash_spritetext(20,rmode->viHeight-61+trans+23,(char*)"thatotherdev.wordpress.com",1);

}
