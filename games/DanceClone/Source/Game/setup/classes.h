class arrow
{
public:
  int direction;
  long time;
  long ypos;
  int length;
  int type; // for drawing based on note value
  arrow(int d, long t, long y, int l, int ty) {direction = d; time = t; ypos = y; length = l; type = ty;};
};


class bpm_change
{
public:
  int   beat;
  float timestamp;
  float bpm;
  bpm_change(int b, float t, float bp) {beat = b; timestamp = t; bpm = bp;};
};
