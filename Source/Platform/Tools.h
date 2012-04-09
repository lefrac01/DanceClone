//      Tools.h
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
#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <SDL.h>

#include "OS.h"

#define PI 3.141592654

double dist(double x1,double y1,double x2,double y2);
double angle(double x1,double y1,double x2,double y2);
int lownumber(int a,int b);
int highnumber(int a,int b);
bool boxoverlap(double x1,double y1,double x2,double y2,double size);
bool distlessthan(double x1,double y1,double x2,double y2,double distance);
bool ininclusiverange(int num,int low,int high);
bool float_same(float a, float b);
bool double_same(double a, double b);
bool SegmentIntersectRectangle(double a_rectangleMinX,
                 double a_rectangleMinY,
                 double a_rectangleMaxX,
                 double a_rectangleMaxY,
                 double a_p1x,
                 double a_p1y,
                 double a_p2x,
                 double a_p2y);
bool PointInSDLRect(int x, int y, SDL_Rect& r);
void SwapByteOrder(unsigned short& us);
void SwapByteOrder(unsigned int& ui);
void SwapByteOrder(unsigned long long& ull);

class PFunc
{
public:

  enum Function
  {
    Linear,
    Inverse,
    Square,
    Cube,
    Sine,
    Cosine,
    Log,
    NiceLog,
    NUM_FUNCTIONS
  };

  // return min to max as p varies from [0..1]
  static float Parametric(Function f, float p, float start, float end);
  // return [0..1] as x varies from min to maxs
  //NOTE: Log and NiceLog do not work with this yet
  static float ParamByVal(Function f, float x, float start, float end);
};

#endif
