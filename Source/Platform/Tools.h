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


int getnumberfromchar(char text);
char getcharfromnumber(int textnumber);
bool charmatch(char* text1,char* text2);
bool charmatchstart(char* fulltext,char* partial);
bool charmatchend(char* fulltext,char* partial);
char* charuppercase(char* text);
char* charlowercase(char* originaltext);
char* charpartial(char* originaltext,int start,int end);
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
#endif
