#ifndef CROTENC_H_INCLUDED
#define CROTENC_H_INCLUDED

#include "mbed.h"


/* This Class handles a rotary encoder like the one from Pollin electronic (Panasonic EVEP...).
 * It uses two pins, one creating an interrupt on change.
 * Rotation direction is determined by checking the state of the other pin.
 *
 * Operating the encoder changes an internal integer value that can be read
 * by Get() or the operator int() functions.
 * A new value can be set by Set(value) or opperator=.
 *
 * Autor: Thomas Raab (Raabinator)
 *
 * Dent steady point     !     !     !
 *                    +-----+     +-----+
 * pinA (interrupt)   |     |     |     |
 *                  --+     +-----+     +---
 *                      +-----+     +-----+
 * pinB                 |     |     |     |
 *                  ----+     +-----+     +-
 *                           --> C.W
 * CW:  increases position value
 * CCW: decreases position value
 *
 * changelog:
 *
 * 09. Nov. 2010
 *     First version published.
 *
 */



class CRotaryEncoder
{
  public:
    CRotaryEncoder(PinName pinA, PinName pinB);
    ~CRotaryEncoder();

    int Get(void);
    int diff(void);
    volatile int    state;
    inline operator int() { return Get(); }

    void Set(int value);
    inline CRotaryEncoder& operator= ( int  value ) { Set(value); return *this; }

  private:
    InterruptIn     *m_pinA;
    InterruptIn     *m_pinB;
    volatile int    m_position;
    volatile int    last_read;

    void arise(void);
    void afall(void);

    void brise(void);
    void bfall(void);
};


#endif
