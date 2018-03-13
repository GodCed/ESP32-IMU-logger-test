#include "display.h"

#include "fonts.h"

Display::Display()
{

}

Display::~Display()
{

}

void Display::begin()
{
    _epd.Init();
}

void Display::end()
{
    _epd.Sleep();
}

void Display::showSplashScreen()
{
    _blackPaint.Clear(0);
    _redPaint.Clear(0);

    _redPaint.DrawStringAt(10, 20, "Open IMU", &Font24, 1);
    _blackPaint.DrawStringAt(10, 50, "System is idle", &Font16, 1);

    _blackPaint.DrawStringAt(10, 245, "SW2 to start...", &Font16, 1);

    _epd.DisplayFrame(_blackImage, _redImage);
}

void Display::showMenu(Menu* menu)
{
    _blackPaint.Clear(0);
    _redPaint.Clear(0);

    menu->paint(_blackPaint, _redPaint, 5, 5);
    _epd.DisplayFrame(_blackImage, _redImage);
}

void Display::updateMenu(Menu* menu)
{
    _blackPaint.Clear(0);
    _redPaint.Clear(0);

    menu->paint(_blackPaint, _redPaint, 5, 5);
    _epd.DisplayFrameQuick(_blackImage, _redImage);
}
