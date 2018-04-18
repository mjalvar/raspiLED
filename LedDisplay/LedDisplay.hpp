/* -------------------------------------------------------------------------------

    L E D   D I S P L A Y
    LedDisplay.hpp
    Header del controlador del led display


    Syntax format: use 4 spaces instead of tab
    set expandtab
    set shiftwidth=4
    set softtabstop=4
    filetype plugin indent on

    Copyright MorphoColor 2016
    Prohibido cualquier uso de este codigo fuente propiedad de MorphoColor
    Todos los derechos reservados 2016

   -------------------------------------------------------------------------------
*/

#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#include "Constants.hpp"
#include "MorphoColor.hpp"
#include "VirtualDisplay.hpp"

//Set up Data Block for LEDs
const int LED_NUM_S = Constants::MORPHO_DISPLAY_H;
const int LED_P_S = Constants::MORPHO_DISPLAY_W;
const int LED_TOTAL = LED_NUM_S*LED_P_S;
const int TS_CH = 5;  // 5 strips per channel
const int LED_TOTAL_CH = TS_CH*LED_P_S;
const int END = 12;
const int BYTES = (1+LED_TOTAL+END)*4;
const int BYTES_CH = (1+LED_TOTAL_CH+END)*4;
const int TOTAL_CH = Constants::MORPHO_DISPLAY_H/TS_CH;
const int BUTTON_CTR = 25;


class LedDisplay: public VirtualDisplay{

    private:
        int default_brightness;
        char ledData[TOTAL_CH][BYTES_CH];
        void pixelRGB(int ch, int i,int16_t br,uint16_t R, uint16_t G, uint16_t B);
        int pixelXY2led(int x, int y);

        bool spi_init();
        void spi_end();
        void spi_show();

        bool gpio_init();
        void gpio_end();
        void gpio_show();

        void read_button(bool force);
        int button;
        int button_cnt;

    public:
        LedDisplay();
        ~LedDisplay();

        bool SPI_OK;

        void fps_test();
        void show();
        void clear();
        void sleep(int d);
        void pixelRGB_xy(int x, int y, MorphoColor c);
        void setBrightness(int b);
        void pixelBRGB_xy(int x, int y, MorphoColor c);

        void set_button();
        void clr_button();
        int get_button();
};


#endif
