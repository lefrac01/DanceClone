//      Tools.c
//      
//      Copyright 2012 Carl Lefrançois <carl.lefrancois@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
#include "Tools.h"

bool PointInSDLRect(int x, int y, SDL_Rect& r)
{
  return x >= r.x && x <= r.x+r.w && y >= r.y && y < r.y+r.h;
}

double dist(double x1,double y1,double x2,double y2){
x1-=x2;y1-=y2;return sqrt(x1*x1+y1*y1);}

double angle(double x1,double y1,double x2,double y2){
return atan2(y1-y2,x1-x2)*57.2957795;}

int lownumber(int a,int b){
if(a<b){return a;}else{return b;}}

int highnumber(int a,int b){
if(a>b){return a;}else{return b;}}


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

bool float_same(float a, float b)
{
  return fabs(a - b) < 0.0001;
}

bool double_same(double a, double b)
{
  return fabs(a - b) < 0.00000001;
}

//posted by metamal at
//http://stackoverflow.com/questions/99353/how-to-test-if-a-line-segment-intersects-an-axis-aligned-rectange-in-2d

bool SegmentIntersectRectangle(double a_rectangleMinX,
                 double a_rectangleMinY,
                 double a_rectangleMaxX,
                 double a_rectangleMaxY,
                 double a_p1x,
                 double a_p1y,
                 double a_p2x,
                 double a_p2y){

  // Find min and max X for the segment
  double minX = a_p1x;
  double maxX = a_p2x;  
  if(a_p1x > a_p2x){
    minX = a_p2x;
    maxX = a_p1x;
  }
  
  // Find the intersection of the segment's and rectangle's x-projections
  if(maxX > a_rectangleMaxX){
    maxX = a_rectangleMaxX;
  } 
  if(minX < a_rectangleMinX){
    minX = a_rectangleMinX;
  } 
  if(minX > maxX){// If their projections do not intersect return false
    return false;
  }
  
  // Find corresponding min and max Y for min and max X we found before
  double minY = a_p1y;
  double maxY = a_p2y;
  double dx = a_p2x - a_p1x;
  if(fabs(dx) > 0.0000001){
    double a = (a_p2y - a_p1y) / dx;
    double b = a_p1y - a * a_p1x;
    minY = a * minX + b;
    maxY = a * maxX + b;
  }
  if(minY > maxY){
    double tmp = maxY;
    maxY = minY;
    minY = tmp;
  }
  
  // Find the intersection of the segment's and rectangle's y-projections
  if(maxY > a_rectangleMaxY){
    maxY = a_rectangleMaxY;
  }
  if(minY < a_rectangleMinY){
    minY = a_rectangleMinY;
  }
  if(minY > maxY){// If Y-projections do not intersect return false
    return false;
  }
  
  return true;
}

void SwapByteOrder(unsigned short& us)
{
  us = (us >> 8) |
       (us << 8);
}

void SwapByteOrder(unsigned int& ui)
{
  ui = (ui >> 24) |
       ((ui<<8) & 0x00FF0000) |
       ((ui>>8) & 0x0000FF00) |
       (ui << 24);
}
void SwapByteOrder(unsigned long long& ull)
{
  ull = (ull >> 56) |
        ((ull<<40) & 0x00FF000000000000) |
        ((ull<<24) & 0x0000FF0000000000) |
        ((ull<<8)  & 0x000000FF00000000) |
        ((ull>>8)  & 0x00000000FF000000) |
        ((ull>>24) & 0x0000000000FF0000) |
        ((ull>>40) & 0x000000000000FF00) |
        (ull << 56);
}

float PFunc::Parametric(Function f, float p, float start, float end)
{
  //#if (min > max)
  //#{
    //#min = max;
  //#}
  if (p < 0)
  {
    p = 0;
  }
  else if (p > 1)
  {
    p = 1;
  }
  
  switch(f)
  {
    case Linear:
      return start + (end-start) * p;
    case Inverse:
      return start + (end-start) * (1-p);
    case Square:
      return start + (end-start) * (p*p);
    case Cube:
      return start + (end-start) * (p*p*p);
    case Sine:
      return sin(Parametric(Linear, Parametric(Linear, p, start, end), 0, 2*PI));
//      return sin(Parametric(Linear, Parametric(Linear, p, start, end), asin(-1), asin(1)));
    case Cosine:
      return cos(Parametric(Linear, Parametric(Linear, p, start, end), 0, 2*PI));
//      return cos(Parametric(Linear, Parametric(Linear, p, start, end), acos(1), acos(-1)));
    case Log:
      return 0; // need header on Wii... too tired :(
      //return start + (end-start) * log(p);
    case NiceLog:
      return 0; // need header on Wii... too tired :(
      //return 1 to 2.7182 of Log which gives approx output (0..1)
      //return start + (end-start) * log(Parametric(Linear, p, 1.0, 2.718281828));
    default:
      return start;
  }
}

float PFunc::ParamByVal(Function f, float x, float start, float end)
{
  float min = 0;
  float max = 0;
  if (start > end)
  {
    min = end;
    max = start;
  }
  else
  {
    max = end;
    min = start;
  }
  if (x <= min)
  {
    return 0;
  }
  else if (x >= max)
  {
    return 1;
  }
  
  switch(f)
  {
    case Linear:
      return (x - min) / (max - min);   // end != start or already returned 0 or 1
    case Square:
      return pow(ParamByVal(Linear, x, min, max), 0.5);
    case Cube:
      return pow(ParamByVal(Linear, x, min, max), 1.0/3.0);
    case Log:
      return 0; // fail... been too long since uni
      //return exp(ParamByVal(Linear, x, start, end));
    case NiceLog:
      return 0; // fail...
      //return exp (Parametric(Linear, ParamByVal(Linear, x, start, end), 0.0, 1.0));
    default:
      return start;
  }
}

    
    // testing of parametric function class
    //#LOG(DEBUG_BASIC, "Parametric testing ===============+" << endl)
    //#LOG(DEBUG_BASIC, "Sine 0 to 1:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << ParametricFunction::Parametric(ParametricFunction::Sine, p, 0.0, 1.0) << endl)
    //#}
    //#LOG(DEBUG_BASIC, "Cosine 0 to 1:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << ParametricFunction::Parametric(ParametricFunction::Cosine, p, 0.0, 1.0) << endl)
    //#}
    //#LOG(DEBUG_BASIC, "Linear 0 to 1:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << ParametricFunction::Parametric(ParametricFunction::Linear, p, 0.0, 1.0) << and back: " << ParametricFunction::ParamByVal(ParametricFunction::Linear, ParametricFunction::Parametric(ParametricFunction::Linear, p, 0.0, 1.0), 0.0, 1.0) << endl)
    //#}
    //#LOG(DEBUG_BASIC, "Linear 0 to 100:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << ParametricFunction::Parametric(ParametricFunction::Linear, p, 0.0, 100.0) << and back: " << ParametricFunction::ParamByVal(ParametricFunction::Linear, ParametricFunction::Parametric(ParametricFunction::Linear, p, 0.0, 100.0), 0.0, 100.0) << endl)
//#
    //#}
    //#LOG(DEBUG_BASIC, "Linear 200 to 500:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << ParametricFunction::Parametric(ParametricFunction::Linear, p, 200.0, 500.0) << and back: " << ParametricFunction::ParamByVal(ParametricFunction::Linear, ParametricFunction::Parametric(ParametricFunction::Linear, p, 200.0, 500.0), 200.0, 500.0) << endl)
//#
    //#}
    //#LOG(DEBUG_BASIC, "Square 0 to 1:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << ParametricFunction::Parametric(ParametricFunction::Square, p, 0.0, 1.0) << and back: " << ParametricFunction::ParamByVal(ParametricFunction::Square, ParametricFunction::Parametric(ParametricFunction::Square, p, 0.0, 1.0), 0.0, 1.0) << endl)
//#
    //#}
    //#LOG(DEBUG_BASIC, "Cube 0 to 1:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << ParametricFunction::Parametric(ParametricFunction::Cube, p, 0.0, 1.0) << and back: " << ParametricFunction::ParamByVal(ParametricFunction::Cube, ParametricFunction::Parametric(ParametricFunction::Cube, p, 0.0, 1.0), 0.0, 1.0) << endl)
//#
    //#}
    //#LOG(DEBUG_BASIC, "Log 0 to 1:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << ParametricFunction::Parametric(ParametricFunction::Log, p, 0.0, 1.0) <<  and back: " << ParametricFunction::ParamByVal(ParametricFunction::Log, ParametricFunction::Parametric(ParametricFunction::Log, p, 0.0, 1.0), -4.6, 0.0) << endl)
//#
    //#}
    //#LOG(DEBUG_BASIC, "NiceLog 0 to 1:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << ParametricFunction::Parametric(ParametricFunction::NiceLog, p, 0.0, 1.0) << and back: " << ParametricFunction::ParamByVal(ParametricFunction::NiceLog, ParametricFunction::Parametric(ParametricFunction::NiceLog, p, 0.0, 1.0), 0.0, 1.0) << endl)
//#
    //#}
    //#LOG(DEBUG_BASIC, "Parametric testing ===============-" << endl)
    //#
