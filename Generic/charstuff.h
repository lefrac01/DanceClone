int getnumberfromchar(char text){
int textnumber=0;
if(text==' ')textnumber=0;
if(text=='!')textnumber=1;
if(text=='"')textnumber=2;
if(text=='#')textnumber=3;
if(text=='$')textnumber=4;
if(text=='%')textnumber=5;
if(text=='&')textnumber=6;
if(text=='\'')textnumber=7;
if(text=='(')textnumber=8;
if(text==')')textnumber=9;
if(text=='*')textnumber=10;
if(text=='+')textnumber=11;
if(text==',')textnumber=12;
if(text=='-')textnumber=13;
if(text=='.')textnumber=14;
if(text=='/')textnumber=15;
if(text=='0')textnumber=16;
if(text=='1')textnumber=17;
if(text=='2')textnumber=18;
if(text=='3')textnumber=19;
if(text=='4')textnumber=20;
if(text=='5')textnumber=21;
if(text=='6')textnumber=22;
if(text=='7')textnumber=23;
if(text=='8')textnumber=24;
if(text=='9')textnumber=25;
if(text==':')textnumber=26;
if(text==';')textnumber=27;
if(text=='<')textnumber=28;
if(text=='=')textnumber=29;
if(text=='>')textnumber=30;
if(text=='?')textnumber=31;
if(text=='@')textnumber=32;
if(text=='A')textnumber=33;
if(text=='B')textnumber=34;
if(text=='C')textnumber=35;
if(text=='D')textnumber=36;
if(text=='E')textnumber=37;
if(text=='F')textnumber=38;
if(text=='G')textnumber=39;
if(text=='H')textnumber=40;
if(text=='I')textnumber=41;
if(text=='J')textnumber=42;
if(text=='K')textnumber=43;
if(text=='L')textnumber=44;
if(text=='M')textnumber=45;
if(text=='N')textnumber=46;
if(text=='O')textnumber=47;
if(text=='P')textnumber=48;
if(text=='Q')textnumber=49;
if(text=='R')textnumber=50;
if(text=='S')textnumber=51;
if(text=='T')textnumber=52;
if(text=='U')textnumber=53;
if(text=='V')textnumber=54;
if(text=='W')textnumber=55;
if(text=='X')textnumber=56;
if(text=='Y')textnumber=57;
if(text=='Z')textnumber=58;
if(text=='[')textnumber=59;
if(text=='\\')textnumber=60;
if(text==']')textnumber=61;
if(text=='^')textnumber=62;
if(text=='_')textnumber=63;
if(text=='`')textnumber=64;
if(text=='a')textnumber=65;
if(text=='b')textnumber=66;
if(text=='c')textnumber=67;
if(text=='d')textnumber=68;
if(text=='e')textnumber=69;
if(text=='f')textnumber=70;
if(text=='g')textnumber=71;
if(text=='h')textnumber=72;
if(text=='i')textnumber=73;
if(text=='j')textnumber=74;
if(text=='k')textnumber=75;
if(text=='l')textnumber=76;
if(text=='m')textnumber=77;
if(text=='n')textnumber=78;
if(text=='o')textnumber=79;
if(text=='p')textnumber=80;
if(text=='q')textnumber=81;
if(text=='r')textnumber=82;
if(text=='s')textnumber=83;
if(text=='t')textnumber=84;
if(text=='u')textnumber=85;
if(text=='v')textnumber=86;
if(text=='w')textnumber=87;
if(text=='x')textnumber=88;
if(text=='y')textnumber=89;
if(text=='z')textnumber=90;
if(text=='{')textnumber=91;
if(text=='|')textnumber=92;
if(text=='}')textnumber=93;
if(text=='~')textnumber=94;
return textnumber;
}

char getcharfromnumber(int textnumber){
char text='?';
if(textnumber==0)text=' ';
if(textnumber==1)text='!';
if(textnumber==2)text='"';
if(textnumber==3)text='#';
if(textnumber==4)text='$';
if(textnumber==5)text='%';
if(textnumber==6)text='&';
if(textnumber==7)text='\'';
if(textnumber==8)text='(';
if(textnumber==9)text=')';
if(textnumber==10)text='*';
if(textnumber==11)text='+';
if(textnumber==12)text=',';
if(textnumber==13)text='-';
if(textnumber==14)text='.';
if(textnumber==15)text='/';
if(textnumber==16)text='0';
if(textnumber==17)text='1';
if(textnumber==18)text='2';
if(textnumber==19)text='3';
if(textnumber==20)text='4';
if(textnumber==21)text='5';
if(textnumber==22)text='6';
if(textnumber==23)text='7';
if(textnumber==24)text='8';
if(textnumber==25)text='9';
if(textnumber==26)text=':';
if(textnumber==27)text=';';
if(textnumber==28)text='<';
if(textnumber==29)text='=';
if(textnumber==30)text='>';
if(textnumber==31)text='?';
if(textnumber==32)text='@';
if(textnumber==33)text='A';
if(textnumber==34)text='B';
if(textnumber==35)text='C';
if(textnumber==36)text='D';
if(textnumber==37)text='E';
if(textnumber==38)text='F';
if(textnumber==39)text='G';
if(textnumber==40)text='H';
if(textnumber==41)text='I';
if(textnumber==42)text='J';
if(textnumber==43)text='K';
if(textnumber==44)text='L';
if(textnumber==45)text='M';
if(textnumber==46)text='N';
if(textnumber==47)text='O';
if(textnumber==48)text='P';
if(textnumber==49)text='Q';
if(textnumber==50)text='R';
if(textnumber==51)text='S';
if(textnumber==52)text='T';
if(textnumber==53)text='U';
if(textnumber==54)text='V';
if(textnumber==55)text='W';
if(textnumber==56)text='X';
if(textnumber==57)text='Y';
if(textnumber==58)text='Z';
if(textnumber==59)text='[';
if(textnumber==60)text='\\';
if(textnumber==61)text=']';
if(textnumber==62)text='^';
if(textnumber==63)text='_';
if(textnumber==64)text='`';
if(textnumber==65)text='a';
if(textnumber==66)text='b';
if(textnumber==67)text='c';
if(textnumber==68)text='d';
if(textnumber==69)text='e';
if(textnumber==70)text='f';
if(textnumber==71)text='g';
if(textnumber==72)text='h';
if(textnumber==73)text='i';
if(textnumber==74)text='j';
if(textnumber==75)text='k';
if(textnumber==76)text='l';
if(textnumber==77)text='m';
if(textnumber==78)text='n';
if(textnumber==79)text='o';
if(textnumber==80)text='p';
if(textnumber==81)text='q';
if(textnumber==82)text='r';
if(textnumber==83)text='s';
if(textnumber==84)text='t';
if(textnumber==85)text='u';
if(textnumber==86)text='v';
if(textnumber==87)text='w';
if(textnumber==88)text='x';
if(textnumber==89)text='y';
if(textnumber==90)text='z';
if(textnumber==91)text='{';
if(textnumber==92)text='|';
if(textnumber==93)text='}';
if(textnumber==94)text='~';
return text;
}

bool charmatch(char* text1,char* text2){
	bool dotheymatch=1;
	if(strlen(text1)!=strlen(text2)){
		dotheymatch=0;
	}else{
		for(unsigned int a=0;a<strlen(text1);a++)
			if(text1[a]!=text2[a])
				dotheymatch=0;
	}
	return dotheymatch;
}

bool charmatchstart(char* fulltext,char* partial){
bool dotheymatch=1;
for(unsigned int a=0; a<strlen(partial); a++)
	if(fulltext[a] != partial[a])
		dotheymatch=0;
return dotheymatch;
}

bool charmatchend(char* fulltext,char* partial){
bool dotheymatch=1;
for(unsigned int a=0; a<strlen(partial); a++)
	if(fulltext[strlen(fulltext)-strlen(partial)+a] != partial[a])
		dotheymatch=0;
return dotheymatch;
}

char* charuppercase(char* text){
int charnumber;
for(unsigned int a=0; a<strlen(text); a++){
	charnumber=getnumberfromchar(text[a]);
	if(charnumber>64 && charnumber<91)
		text[a]=getcharfromnumber(charnumber-32);
}
return text;
}
/*char* charlowercase(char* text){
int charnumber;
for(unsigned int a=0; a<strlen(text); a++){
	charnumber=getnumberfromchar(text[a]);
	if(charnumber>64-32 && charnumber<91-32)
		text[a]=getcharfromnumber(charnumber+32);
}
return text;
}*/
char* charlowercase(char* originaltext){
char* text = new char[strlen(originaltext)+1];
text[0]='\0';
if(strlen(originaltext)){
	int charnumber;
	for(unsigned int a=0; a<strlen(originaltext); a++){
		charnumber=getnumberfromchar(originaltext[a]);
		if(charnumber>64-32 && charnumber<91-32){
			text[a]=getcharfromnumber(charnumber+32);
		}else{
			text[a]=originaltext[a];
		}
		text[a+1]='\0';
	}
}
return text;
delete text;
}
/*
char* charpartial(char* text,int start,int end){
for(int a=0; a<end-start; a++)text[a]=text[a+start];
text[end-start]='\0';
return text;
}*/
char* charpartial(char* originaltext,int start,int end){
char* text = new char[end-start+2];
text[0]='\0';
if(start<end && end<=(signed int)strlen(originaltext))for(int a=0; a<end-start; a++){
	text[a]=originaltext[a+start];
	text[a+1]='\0';
}
return text;
delete text;
}