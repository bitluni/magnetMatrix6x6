#include <tinyNeoPixel_Static.h>

const int ledPin = PIN_PA1;
const unsigned char xres = 6;
const unsigned char yres = 6;
const int ledBufferSize = xres * yres * 3;
unsigned char ledBuffer[ledBufferSize];

const int pins[4][2] = {{PIN_PA2, PIN_PA3}, {PIN_PA4, PIN_PA5}, {PIN_PA6, PIN_PA7}, {PIN_PB2, PIN_PB3}};


tinyNeoPixel leds = tinyNeoPixel(xres * yres, ledPin, NEO_GRB, ledBuffer);

int dimm = 4;

void fill(int i)
{
  for(int y = 0; y < yres; y++)
    for(int x = 0; x < xres; x++)
    {
      int cx = y & 1 ? xres - 1 - x : x;
      ledBuffer[(x + y * xres) * 3 + 0] = (((cx + i)%xres) * 256 / xres) >> dimm;  
      ledBuffer[(x + y * xres) * 3 + 1] = (((y + i)%yres) * 256 / yres) >> dimm;  
      ledBuffer[(x + y * xres) * 3 + 2] = ((xres - max(cx,y) - 1) * 256 / xres) >> dimm;
    }
}
void clear()
{
  for(int i = 0; i < xres * yres * 3; i++)
      ledBuffer[i] = 0;
}

int addr(int x, int y)
{
  if(y & 1)
    return (y * xres + xres - 1 - x) * 3;
  else
    return (y * xres + x) * 3;
}

void showSides(int i, int sides)
{
  clear();
  for(int x = 0; x < xres; x++)
  {
    if((sides & 1) == 0)
    if(((3-x) & 3) == (i & 3))
    {
      ledBuffer[(x + 0) * 3 + 0] = 255 >> dimm;
      ledBuffer[(x + 0) * 3 + 1] = 255 >> dimm;
      ledBuffer[(x + 0) * 3 + 2] = 255 >> dimm;
    }
    if((sides & 4) == 0)
    if(((3-x) & 3) == (i & 3))
    {
      ledBuffer[(x + (yres - 1) * xres) * 3 + 0] = 255 >> dimm;
      ledBuffer[(x + (yres - 1) * xres) * 3 + 1] = 255 >> dimm;
      ledBuffer[(x + (yres - 1) * xres) * 3 + 2] = 255 >> dimm;
    }
  }
  for(int y = 0; y < yres; y++)
  {
    if((sides & 2) == 0)
    if((y & 3) == (i & 3))
    {
      ledBuffer[addr(0, y) + 0] = 255 >> dimm;
      ledBuffer[addr(0, y) + 1] = 255 >> dimm;
      ledBuffer[addr(0, y) + 2] = 255 >> dimm;
    }
    if((sides & 8) == 0)
    if(((3 - y) & 3) == (i & 3))
    {
      ledBuffer[addr(xres - 1, y) + 0] = 255 >> dimm;
      ledBuffer[addr(xres - 1, y) + 1] = 255 >> dimm;
      ledBuffer[addr(xres - 1, y) + 2] = 255 >> dimm;
    }
  }
}

void setup() 
{
  //led
  pinMode(ledPin, OUTPUT);
  //magnet connectors
  pinMode(pins[0][0], INPUT_PULLUP);
  pinMode(pins[0][1], OUTPUT); digitalWrite(pins[0][1], 0);
  pinMode(pins[1][0], INPUT_PULLUP);
  pinMode(pins[1][1], OUTPUT); digitalWrite(pins[1][1], 0);
  pinMode(pins[2][0], INPUT_PULLUP);
  pinMode(pins[2][1], OUTPUT); digitalWrite(pins[2][1], 0);
  pinMode(pins[3][1], INPUT_PULLUP);
  pinMode(pins[3][0], OUTPUT); digitalWrite(pins[3][0], 0);
  //buttons
  pinMode(PIN_PB0, INPUT_PULLUP);
  pinMode(PIN_PB1, INPUT_PULLUP);

  fill(0);
}

void loop() 
{
  static int i = 0;
  int sides = 
    (digitalRead(pins[0][0]) ? 0 : 1) + 
    (digitalRead(pins[1][0]) ? 0 : 2) + 
    (digitalRead(pins[2][0]) ? 0 : 4) + 
    (digitalRead(pins[3][1]) ? 0 : 8);
  int buttons = 
    (digitalRead(PIN_PB0) ? 0 : 1) + 
    (digitalRead(PIN_PB1) ? 0 : 2);
  static int oldButtons = 0;
  if(oldButtons != 0 && buttons == 0)
    dimm = (dimm + 1) % 6;
  oldButtons = buttons;
  if(sides)
    showSides(i, sides);
  else
    fill(i);
  i++;
  leds.show();
  delay(50);
}
