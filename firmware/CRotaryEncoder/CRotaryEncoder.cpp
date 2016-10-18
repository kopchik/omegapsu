#include "mbed.h"
#include "CRotaryEncoder.h"

CRotaryEncoder::CRotaryEncoder(PinName pinA, PinName pinB)
{
    m_pinA = new InterruptIn(pinA);
    m_pinB = new InterruptIn(pinB);

    m_pinA->rise(this, &CRotaryEncoder::arise);
    m_pinA->fall(this, &CRotaryEncoder::afall);

    m_pinB->rise(this, &CRotaryEncoder::brise);
    m_pinB->fall(this, &CRotaryEncoder::bfall);

    m_pinA->mode(PullUp);
    m_pinB->mode(PullUp);

    m_position = 0;
    last_read = 0;
    state = 0;
}

CRotaryEncoder::~CRotaryEncoder()
{
    delete m_pinA;
    delete m_pinB;
}

int CRotaryEncoder::Get(void)
{
    return m_position;
}

int CRotaryEncoder::diff(void) {
  int diff = m_position - last_read;
  if (diff) {
    last_read = m_position;
  }
  return diff;
}

void CRotaryEncoder::Set(int value)
{
    m_position = value;
}

void CRotaryEncoder::afall(void) {
  if(*m_pinB == 1 && state == 0) {
      m_position++;
      state = 1;
  }
  else if (*m_pinB == 0 && state == 3) {
      m_position--;
      state = 2;
  }
}

void CRotaryEncoder::arise(void) {
  if (*m_pinB == 0 && state == 2) {
      m_position++;
      state = 3;
  }
  else if(*m_pinB == 1 && state == 1) {
      m_position--;
      state = 0;
  }
}

void CRotaryEncoder::bfall(void) {
  if (*m_pinA == 0 && state == 1) {
      m_position++;
      state = 2;
  }
  else if(*m_pinA == 1 && state == 0) {
      m_position--;
      state = 3;
  }
}

void CRotaryEncoder::brise(void) {
  if(*m_pinA == 1 && state == 3) {
      m_position++;
      state = 0;
  }
  else if (*m_pinA == 0 && state == 2) {
      m_position--;
      state = 1;
  }
}

