/*************************************************** 
This library is used for controling AC057TC1 5.65 inch epaper panel on e-radionica's InkplateColor dev board (we are still working on it!).
If you don't know what Inkplate is, check it out here: https://inkplate.io/

Author: Borna Biro ( https://github.com/BornaBiro/ )
Organization: e-radionica.com / TAVU

This library uses Adafruit GFX library (https://github.com/adafruit/Adafruit-GFX-Library) made by Adafruit Industries.

NOTE: This library is still heavily in progress, so there is still some bugs. Use it on your own risk!
 ****************************************************/

#include <stdlib.h>
#include "Adafruit_GFX.h"
#include "InkplateColor.h"

Inkplate::Inkplate() : Adafruit_GFX(INKPLATE_WIDTH, INKPLATE_HEIGHT)
{

}

void Inkplate::begin()
{
    SPI.begin();
    pinMode(EPAPER_BUSY_PIN, INPUT);
    pinMode(EPAPER_RST_PIN, OUTPUT);
    pinMode(EPAPER_DC_PIN, OUTPUT);
    pinMode(EPAPER_CS_PIN, OUTPUT);

    // Allocate memory for internal frame buffer
    imageBuffer = (uint8_t*)ps_malloc(INKPLATE_WIDTH * INKPLATE_HEIGHT / 2);
    if (imageBuffer == NULL)
    {
        Serial.println("Memory allocation failed, program stops!");
        while (true);
    }

    // Color whole frame buffer in wgite color
    memset(imageBuffer, INKPLATE_WHITE | (INKPLATE_WHITE << 4), INKPLATE_WIDTH * INKPLATE_HEIGHT / 2);

    // Reset epaper
    resetPanel();

    // Wait for epapper to be ready by reading busy high signal
    while (!(digitalRead(EPAPER_BUSY_PIN)));

    // Send whole bunch of commands and data

    uint8_t panel_set_data[] = {0xef, 0x08};
    sendCommand(PANEL_SET_REGISTER);
    sendData(panel_set_data, 2);

    uint8_t power_set_data[] = {0x37, 0x00, 0x23, 0x23};
    sendCommand(POWER_SET_REGISTER);
    sendData(power_set_data, 4);

    sendCommand(POWER_OFF_SEQ_SET_REGISTER);
    sendData(0x00);

    uint8_t booster_softstart_data[] = {0xc7, 0xc7, 0x1d};
    sendCommand(BOOSTER_SOFTSTART_REGISTER);
    sendData(booster_softstart_data, 3);

    sendCommand(PLL_CONTROL_REGISTER);
    sendData(0x3c);

    sendCommand(TEMP_SENSOR_REGISTER);
    sendData(0x00);

    sendCommand(VCOM_DATA_INTERVAL_REGISTER);
    sendData(0x37);

    sendCommand(0x60);
    sendData(0x20);

    uint8_t res_set_data[] = {0x02, 0x58, 0x01, 0xc0};
    sendCommand(RESOLUTION_SET_REGISTER);
    sendData(res_set_data, 4);

    sendCommand(0xE3);
    sendData(0xaa);

    delay(100);
    sendCommand(0x50);
    sendData(0x37);
}

void Inkplate::clearDisplay()
{
    memset(imageBuffer, INKPLATE_WHITE | (INKPLATE_WHITE << 4), INKPLATE_WIDTH * INKPLATE_HEIGHT / 2);
}

void Inkplate::display()
{
    // Set resolution setting
    uint8_t res_set_data[] = {0x02, 0x58, 0x01, 0xc0};
    sendCommand(0x61);
    sendData(res_set_data, 4);

    // Push pixel data to the epaper RAM
    sendCommand(0x10);
    digitalWrite(EPAPER_DC_PIN, HIGH);
    digitalWrite(EPAPER_CS_PIN, LOW);
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(imageBuffer, INKPLATE_WIDTH * INKPLATE_HEIGHT / 2);
    SPI.endTransaction();
    digitalWrite(EPAPER_CS_PIN, HIGH);

    sendCommand(0x04);
    while (!(digitalRead(EPAPER_BUSY_PIN)));   // Wait for busy high signal
    sendCommand(0x12);
    while (!(digitalRead(EPAPER_BUSY_PIN)));   // Wait for busy high signal
    sendCommand(0x02);
    while (digitalRead(EPAPER_BUSY_PIN));      // Wait for busy low signal
    delay(200);
}

void Inkplate::drawPixel(int16_t _x0, int16_t _y0, uint16_t _color)
{
    if (_color > 6 || _color < 0) return;
    int _x = _x0 / 2;
    int _x_sub = _x0 % 2;
    uint8_t temp;
    temp = *(imageBuffer + INKPLATE_WIDTH / 2 * _y0 + _x);
    *(imageBuffer + INKPLATE_WIDTH / 2 * _y0 + _x) = pixelMaskGLUT[_x_sub] & temp | (_x_sub ? _color : _color << 4);
}
void Inkplate::resetPanel()
{
    digitalWrite(EPAPER_RST_PIN, LOW);
    delay(1);
    digitalWrite(EPAPER_RST_PIN, HIGH);
    delay(200);
}

void Inkplate::sendCommand(uint8_t _command)
{
    digitalWrite(EPAPER_DC_PIN, LOW);
    digitalWrite(EPAPER_CS_PIN, LOW);
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(_command);
    SPI.endTransaction();
    digitalWrite(EPAPER_CS_PIN, HIGH);
}

void Inkplate::sendData(uint8_t *_data, int _n)
{
    digitalWrite(EPAPER_DC_PIN, HIGH);
    digitalWrite(EPAPER_CS_PIN, LOW);
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(_data, _n);
    SPI.endTransaction();
    digitalWrite(EPAPER_CS_PIN, HIGH);
}

void Inkplate::sendData(uint8_t _data)
{
    digitalWrite(EPAPER_DC_PIN, HIGH);
    digitalWrite(EPAPER_CS_PIN, LOW);
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(_data);
    SPI.endTransaction();
    digitalWrite(EPAPER_CS_PIN, HIGH);
}

void Inkplate::fillPanel(uint8_t _color)
{
    memset(imageBuffer, _color | (_color << 4), INKPLATE_WIDTH * INKPLATE_HEIGHT / 2);
}