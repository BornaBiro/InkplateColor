/*************************************************** 
This library is used for controling AC057TC1 5.65 inch epaper panel on e-radionica's InkplateColor dev board (we are still working on it!).
If you don't know what Inkplate is, check it out here: https://inkplate.io/

Author: Borna Biro ( https://github.com/BornaBiro/ )
Organization: e-radionica.com / TAVU

This library uses Adafruit GFX library (https://github.com/adafruit/Adafruit-GFX-Library) made by Adafruit Industries.

NOTE: This library is still heavily in progress, so there is still some bugs. Use it on your own risk!
 ****************************************************/
 
#ifndef __INKPLATECOLOR_H__
#define __INKPLATECOLOR_H__

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef ARDUINO_ESP32_DEV
#error "Wrong board selected! Select ESP32 Wrover from board menu!"
#endif

// Connections between ESP32 and color Epaper
#define EPAPER_RST_PIN          19
#define EPAPER_DC_PIN           33
#define EPAPER_CS_PIN           15
#define EPAPER_BUSY_PIN         32
#define EPAPER_CLK              18
#define EPAPER_DIN              23

// Timeout for init of epaper (1.5 sec in this case)        
#define INIT_TIMEOUT            1500

// Epaper registers
#define PANEL_SET_REGISTER          0x00
#define POWER_SET_REGISTER          0x01
#define POWER_OFF_REGISTER          0x02
#define POWER_OFF_SEQ_SET_REGISTER  0x03
#define POWER_OFF_REGISTER          0x04
#define BOOSTER_SOFTSTART_REGISTER  0x06
#define DEEP_SLEEP_REGISTER         0x07
#define DATA_START_TRANS_REGISTER   0x10
#define DATA_STOP_REGISTER          0x11
#define DISPLAY_REF_REGISTER        0x12
#define IMAGE_PROCESS_REGISTER      0x13
#define PLL_CONTROL_REGISTER        0x30
#define TEMP_SENSOR_REGISTER        0x40
#define TEMP_SENSOR_EN_REGISTER     0x41
#define TEMP_SENSOR_WR_REGISTER     0x42
#define TEMP_SENSOR_RD_REGISTER     0x43
#define VCOM_DATA_INTERVAL_REGISTER 0x50
#define LOW_POWER_DETECT_REGISTER   0x51
#define RESOLUTION_SET_REGISTER     0x61
#define STATUS_REGISTER             0x71
#define VCOM_VALUE_REGISTER         0x81
#define VCM_DC_SET_REGISTER         0x02

// Epaper resolution and colors
#define INKPLATE_WIDTH  600
#define INKPLATE_HEIGHT 448
#define INKPLATE_BLACK  0b00000000
#define INKPLATE_WHITE  0b00000001
#define INKPLATE_GREEN  0b00000010
#define INKPLATE_BLUE   0b00000011
#define INKPLATE_RED    0b00000100
#define INKPLATE_YELLOW 0b00000101
#define INKPLATE_ORANGE 0b00000110


#include "Adafruit_GFX.h"
#include "Wire.h"
#include "SPI.h"

class Inkplate : public Adafruit_GFX {
  public:
        Inkplate();
        bool begin();
        void clearDisplay();
        void display();
        void drawPixel(int16_t _x0, int16_t _y0, uint16_t _color);
        void setPanelState(bool _state);
        bool getPanelState();
        void clean();
        void fillPanel(uint8_t _color);
        
  private:
        bool _panelState = false;
        bool _begin = false;
        uint8_t *imageBuffer;
        const uint8_t pixelMaskGLUT[2] = {B00001111, B11110000};
        void resetPanel();
        void sendCommand(uint8_t _command);
        void sendData(uint8_t *_data, int _n);
        void sendData(uint8_t _data);
};

#endif