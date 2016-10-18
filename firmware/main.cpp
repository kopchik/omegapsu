#include "mbed.h"
#include "port_api.h"
#include "ILI9340_Driver.h"
#include "CRotaryEncoder.h"
#include "MCP4822A.h"
#include <stdint.h>


#define FONT_SIZE 2
#define CSIZE_X (6*FONT_SIZE)
#define CSIZE_Y (8*FONT_SIZE)
#define SIZE_X (320/CSIZE_X)
#define SIZE_Y (240/SIZE_Y)
#define CHBUFLEN (SIZE_X*SIZE_Y)
char chbuf[CHBUFLEN];


class Display: public ILI9340_Display {
  public:
    Display(void);
    void flush(void);
    void print(int x, int y, const char* str);
  private:
    const static int sizex = SIZE_X;
    const static int sizey = SIZE_Y;
    char buf[CHBUFLEN] = {0};
    char oldbuf[CHBUFLEN] = {0};
};


Display::Display(void):
              ILI9340_Display(PB_5  /* MOSI 28 */,
                PB_4  /* MISO 27 */,
                PB_3  /* SCK  26 */,
                PA_3  /* CS   19 */,
                PA_10 /* RST  20 */,
                PA_1  /* DC   18 */)
{
  DispInit();
  SetRotation(3);
  FillScreen(ILI9340_BLACK);
}


void Display::print(int x, int y, const char* str) {
  int offset = x + y*sizex;
  while(*str && offset <sizex*sizey) {
    char ch = *str++;
    if (ch == '\n') {
      //TODO
    } else {
      buf[offset++] = ch;
    }
  }
  flush();
}


void Display::flush() {
  int offset = 0;
  for (int y=0; y<sizey; y++) {
    for (int x=0; x<sizex; x++) {
      offset = x + y*sizex;
      if (buf[offset] != oldbuf[offset]) {
        DrawAscii(oldbuf[offset], x*CSIZE_X, y*CSIZE_Y, FONT_SIZE, ILI9340_BLACK);
        DrawAscii(buf[offset], x*CSIZE_X, y*CSIZE_Y, FONT_SIZE, ILI9340_WHITE);
        oldbuf[offset] = buf[offset];
      }
    }
  }
}

Display display;

void assert_failed(uint8_t* file, uint32_t line) {
  snprintf(chbuf, CHBUFLEN, "assert failed on %s:%lu", file, line);
  display.print(0,0, chbuf);
  while(1);
}

int x = 0;
void on_rise() {
  x = 1;
};
void on_fall() {
  x = -1;
};

int main() {
    volatile int vref = 0, current = 0;

    // encoder
    CRotaryEncoder enc1(PF_0 /* 2 */, PF_1 /* 3 */);
    CRotaryEncoder enc2(PA_7 /* 13 */, PB_0 /* 14 */);

    DigitalIn enter(PA_6, PullUp);
    DigitalIn esc(PB_7, PullUp);
    DigitalIn overload(PA_8, PullDown);
    AnalogIn  vout(PA_0);
    AnalogIn  cout(PA_2);

    PinName cses[] = {PB_6}; /* CS 2 */
    MCP4822A dac(1,
                PB_5   /* MOSI 28 */,
                PB_3   /* SCK  26 */,
                cses   /* CS    2 */,
                NC);


    dac.writeA1(0, 250*2);
    while(1) {
        vref = enc1.Get() * 10 / 4;
        current = enc2.Get() * 10 / 4;
        snprintf(chbuf, CHBUFLEN, "SET: %5umV %5umA", vref, current);
        display.print(0, 0, chbuf);
        snprintf(chbuf, CHBUFLEN, "OUT: %5umV %5umA", vout.read_u16(), cout.read_u16());
        display.print(0, 1, chbuf);
        snprintf(chbuf, CHBUFLEN, "enter %d esc %d", enter.read(), esc.read());
        display.print(0, 2, chbuf);
        snprintf(chbuf, CHBUFLEN, "ovr: %d", overload.read());
        display.print(0, 3, chbuf);
        snprintf(chbuf, CHBUFLEN, "! PF_0: %d PF_1: %d state: %d", enc1.m_pinA, enc1.m_pinA, enc1.state);
        display.print(0, 4, chbuf);
        //snprintf(chbuf, CHBUFLEN, "X: %d ", x);
        //display.print(0, 5, chbuf);

        dac.writeA1(0, vref*2);
        dac.writeB1(0, current*2);
        wait(0.1);
    }
}
