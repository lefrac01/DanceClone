double dist(double x1,double y1,double x2,double y2){
x1-=x2;y1-=y2;return sqrt(x1*x1+y1*y1);}

double angle(double x1,double y1,double x2,double y2){
return atan2(y1-y2,x1-x2)*57.2957795;}

int lownumber(int a,int b){
if(a<b){return a;}else{return b;}}

int highnumber(int a,int b){
if(a>b){return a;}else{return b;}}

Uint8 in8bitrange(int num){
	if(num<=0) return 0;
	else if(num>=255) return 255;
	else return num;
}

bool boxoverlap(double x1,double y1,double x2,double y2,double size){
if(x1>x2-size && x1<x2+size && y1>y2-size && y1<y2+size){return 1;}else{return 0;}
}

bool distlessthan(double x1,double y1,double x2,double y2,double distance){
bool isitlessthan=0;
if(dist(x1,y1,x2,y2)<distance)isitlessthan=1;
return isitlessthan;
}

bool ininclusiverange(int num,int low,int high){
	if(num>=low && num<=high){
		return 1;
	}else{
		return 0;
	}
}