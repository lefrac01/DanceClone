#include "WiiDash_setup_sprite.h"

bool WDonoff=0;
double WDonoffpercent=0;

struct tm *WDtime;
const char* WDwday[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
const char* WDmonth[]={"January","February","March","April","May","June","July","August","September","October","November","December"};
const char* WDday[]={"1st","2nd","3rd","4th","5th","6th","7th","8th","9th","10th","11th","12th","13th","14th","15th","16th","17th","18th","19th","20th","21st","22nd","23rd","24th","25th","26th","27th","28th","29th","30th","31st"};

int WDruntime=0;
int WDgametime=0;
int WDframetime=0;
int WDframerate=0;
double WDtimespeed=0;

char WDextracredits[100];

void WiiDash_init(){
	WiiDash_setup_sprite();
}
