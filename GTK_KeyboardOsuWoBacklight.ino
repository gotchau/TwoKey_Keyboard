// Osu edition! без подсветки 08/06/2021
// Скечт для двухкнопочной клавиатуры.
// Библиотеки Adafruit занимают много места, потом поменяю.
//
// Ссылки на библиотеки:
// TrinketHidCombo https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketHidCombo
// YouTube https://youtu.be/BGkTiVP5ryY
// Gotchau 06/02/2021

#include <Arduino.h>
#include <TrinketHidCombo.h>

#define PIN_BTN_A 0
#define PIN_BTN_B 2

bool leftBtnState = 0;
bool leftBtnLastState = 0;
uint32_t leftBtnLastTimePressed = 0;

bool rightBtnState = 0;
bool rightBtnLastState = 0;
uint32_t rightBtnLastTimePressed = 0;

void setup()
{
  pinMode(PIN_BTN_A, INPUT_PULLUP);
  pinMode(PIN_BTN_B, INPUT_PULLUP);

  TrinketHidCombo.begin();
}

void loop()
{
  uint32_t now = micros();

  //считываем текущее значение
  leftBtnState = !digitalRead(PIN_BTN_A);
  if (now - leftBtnLastTimePressed > 1000)
  {
    leftBtnLastTimePressed = now;

    // при нажатии левой кнопки
    if (leftBtnState && !leftBtnLastState)
    {
      leftBtnLastState = leftBtnState;
      TrinketHidCombo.pressKey(0, KEYCODE_Z);
    }
    // при отпускании левой кнопки
    if (!leftBtnState && leftBtnLastState)
    {
      leftBtnLastState = leftBtnState;
      TrinketHidCombo.pressKey(0, 0);
    }
  }

  //считываем текущее значение
  rightBtnState = !digitalRead(PIN_BTN_B);
  if (now - rightBtnLastTimePressed > 1000)
  {
    rightBtnLastTimePressed = now;

    // при нажатии правой кнопки
    if (rightBtnState && !rightBtnLastState)
    {
      rightBtnLastState = rightBtnState;
      TrinketHidCombo.pressKey(0, KEYCODE_X);
    }
    // при отпускании правой кнопки
    if (!rightBtnState && rightBtnLastState)
    {
      rightBtnLastState = rightBtnState;
      TrinketHidCombo.pressKey(0, 0);
    }
  }

  TrinketHidCombo.poll();
}