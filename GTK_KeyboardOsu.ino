// Osu edition! 31/05/2021
// Скечт для двухкнопочной клавиатуры с RGB-подсветкой на ws2812 2020.
// Библиотеки Adafruit занимают много места, потом поменяю.
// Примечание. Оба режима прекрасно работают с версией библиотеки Adafruit NeoPixel@1.3.4
//
// Ссылки на библиотеки:
// TrinketHidCombo https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketHidCombo
// Adafruit_NeoPixel https://github.com/adafruit/Adafruit_NeoPixel
// YouTube https://youtu.be/BGkTiVP5ryY
// Gotchau 06/02/2021

#include <Arduino.h>
#include <TrinketHidCombo.h>
#include <Adafruit_NeoPixel.h>

#define USB_CFG_VENDOR_NAME 'G', 'o', 't', 'c', 'h', 'a', 'u'
#define USB_CFG_VENDOR_NAME_LEN 7
#define USB_CFG_DEVICE_NAME 'G', 'o', 't', 'c', 'h', 'a', 'u' ' ', 'H', 'I', 'D',
#define USB_CFG_DEVICE_NAME_LEN 11

#define PIN_BTN_A 0
#define PIN_BTN_B 2

#define PIN_LEDS 1
#define NUMPIXELS 2
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

uint32_t lastTime = 0;
uint8_t periodMs = 10; // отвечает за скорость анимации. Меньше - быстрее.

uint8_t btnBrightness[] = {100, 100};

bool leftBtnState = 0;
bool leftBtnLastState = 0;

bool rightBtnState = 0;
bool rightBtnLastState = 0;

void setup()
{
  pinMode(PIN_BTN_A, INPUT_PULLUP);
  pinMode(PIN_BTN_B, INPUT_PULLUP);

  TrinketHidCombo.begin();
  pixels.begin();
  pixels.setBrightness(255);
}

void fading()
{
  for (int i = 0; i < 2; i++)
    if (btnBrightness[i] > 0)
    {
      uint32_t color = pixels.getPixelColor(i);
      uint8_t r = ((color & 0x00FF0000) >> 16) * btnBrightness[i] / 100;
      uint8_t g = ((color & 0x0000FF00) >> 8) * btnBrightness[i] / 100;
      uint8_t b = (color & 0x000000FF) * btnBrightness[i] / 100;

      pixels.setPixelColor(i, pixels.Color(r, g, b));
      btnBrightness[i]--;
      pixels.show();
    }
}

void rainbow2(uint16_t step)
{
  static long firstPixelHue = 0;
  firstPixelHue += step;

  if (firstPixelHue > 5 * 65536)
    firstPixelHue = 0;

  for (uint16_t i = 0; i < pixels.numPixels(); i++)
  {
    int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
    pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
  }
  pixels.show();
}

void loop()
{
  uint32_t now = millis();
  if (now - lastTime > periodMs)
  {
    lastTime = now;

    //Плавное затухание
    //fading();
    //Режим тру-геймера. Аргументом передается плавность радуги. Меньше-плавнее.
    rainbow2(127);
  }

  //считываем текущее значение
  leftBtnState = !digitalRead(PIN_BTN_A);
  // при нажатии левой кнопки
  if (leftBtnState && !leftBtnLastState)
  {
    leftBtnLastState = leftBtnState;
    TrinketHidCombo.pressKey(0, KEYCODE_Z);
    pixels.setPixelColor(0, pixels.Color(150, 0, 210)); // любой цвет в формате RGB 0-255
    btnBrightness[0] = 100;
  }
  // при отпускании левой кнопки
  if (!leftBtnState && leftBtnLastState)
  {
    leftBtnLastState = leftBtnState;
    TrinketHidCombo.pressKey(0, 0);
  }

  //считываем текущее значение
  rightBtnState = !digitalRead(PIN_BTN_B);
  // при нажатии правой кнопки
  if (rightBtnState && !rightBtnLastState)
  {
    rightBtnLastState = rightBtnState;
    TrinketHidCombo.pressKey(0, KEYCODE_X);
    pixels.setPixelColor(1, pixels.Color(150, 0, 210)); // любой цвет в формате RGB 0-255
    btnBrightness[1] = 100;
  }
  // при отпускании правой кнопки
  if (!rightBtnState && rightBtnLastState)
  {
    rightBtnLastState = rightBtnState;
    TrinketHidCombo.pressKey(0, 0);
  }

  TrinketHidCombo.poll();
}