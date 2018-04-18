/* -------------------------------------------------------------------------------

    L E D   D I S P L A Y
    LedDisplay.cpp
    Controlador del led display


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

#include "LedDisplay.hpp"

#define CLK  RPI_GPIO_P1_07
#define CH_0 RPI_GPIO_P1_11
#define CH_1 RPI_GPIO_P1_08
#define CH_2 RPI_GPIO_P1_10

#define BTN RPI_GPIO_P1_26


LedDisplay::LedDisplay() {
    int i;
    button = 0;
    button_cnt = 0;

    // start frame
    for(int ch=0;ch<TOTAL_CH;ch++){
        ledData[ch][0] = 0;
        ledData[ch][1] = 0;
        ledData[ch][2] = 0;
        ledData[ch][3] = 0;
    }

    // clean frame
    setBrightness(1);
    for(int ch=0;ch<TOTAL_CH;++ch){
        for(i=1; i<=LED_TOTAL_CH; ++i){
            pixelRGB(ch,i,default_brightness,0,0,0);
        }

        // end frame
        for(i=1; i<=END; ++i){
            pixelRGB(ch,LED_TOTAL_CH+i,255,255,255,255);
        }
    }

    #ifdef RPI
        printf("SPI RPI mode\n");
    #else
        printf("SPI non RPI mode\n");
    #endif

    //SPI_OK = spi_init();
    SPI_OK = gpio_init();

    printf("SPI NumLeds: %0d\n",LED_TOTAL);
    printf("SPI Size: %0d\n",BYTES);
    printf("SPI OK: %0d\n",SPI_OK);

    int debug;
    debug = pixelXY2led(0,0);
    printf("debug: %0d\n",debug);
    debug = pixelXY2led(0,5);
    printf("debug: %0d\n",debug);
    debug = pixelXY2led(0,2);
    printf("debug: %0d\n",debug);

}

LedDisplay::~LedDisplay(){
    //spi_end();
    gpio_end();
}


int LedDisplay::pixelXY2led(int x, int y){
    int led_num;
    y = y%TS_CH;
    if(y%2==0)
        led_num = (TS_CH-1-y)*LED_P_S+(LED_P_S-x);
    else
        led_num = (TS_CH-1-y)*LED_P_S+x+1;

    return led_num;
}


void LedDisplay::setBrightness(int br){
    br = br & 0x1F;
    br = 0xE0 | br;
    default_brightness = 0xE0 | br;
    printf("default_brightness = %0x\n",default_brightness);
}


void LedDisplay::pixelBRGB_xy(int x, int y, MorphoColor c){
    int br;
    int ch = TOTAL_CH-y/TS_CH-1;
    br = c.br & 0x1F;
    br = 0xE0 | br;
    pixelRGB( ch,pixelXY2led(x,y),br,c.R,c.G,c.B);
}


void LedDisplay::pixelRGB(int ch, int i, int16_t br,uint16_t R, uint16_t G, uint16_t B) {
    ledData[ch][i*4] = br;
    ledData[ch][i*4+1] = B;
    ledData[ch][i*4+2] = G;
    ledData[ch][i*4+3] = R;
}

void LedDisplay::pixelRGB_xy(int x, int y, MorphoColor c) {
    int ch = TOTAL_CH-y/TS_CH-1;
    pixelRGB( ch, pixelXY2led(x,y) ,default_brightness,c.R,c.G,c.B);
}


void LedDisplay::clear() {
    for(int i=1; i<=LED_TOTAL_CH; ++i){
        for(int ch=0;ch<TOTAL_CH;++ch)
            pixelRGB(ch,i,default_brightness,0,0,0);
    }
}


bool LedDisplay::gpio_init() {
    #ifdef RPI
    if (!bcm2835_init()) {
      printf("bcm2835_init failed. Are you running as root??\n");
      return false;
    }
    // Set the pins to be output
    bcm2835_gpio_fsel(BTN, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    bcm2835_gpio_set_pud(BTN, BCM2835_GPIO_PUD_UP);

    bcm2835_gpio_fsel(CLK, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(CH_0, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(CH_1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(CH_2, BCM2835_GPIO_FSEL_OUTP);
    #endif
    return true;
}


bool LedDisplay::spi_init() {

    #ifdef RPI
    //Initiate the SPI Data Frame
    if (!bcm2835_init()) {
      printf("bcm2835_init failed. Are you running as root??\n");
      return false;
    }
    if (!bcm2835_spi_begin()) {
      printf("bcm2835_spi_begin failedg. Are you running as root??\n");
      return false;
    }
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default

    /*
        BCM2835_SPI_MODE0       CPOL = 0, CPHA = 0
        BCM2835_SPI_MODE1       CPOL = 0, CPHA = 1
        BCM2835_SPI_MODE2       CPOL = 1, CPHA = 0
        BCM2835_SPI_MODE3       CPOL = 1, CPHA = 1
    */
    bcm2835_spi_setClockDivider(28); // The default
    /*
        BCM2835_SPI_CLOCK_DIVIDER_65536     65536 = 262.144us = 3.814697260kHz
        BCM2835_SPI_CLOCK_DIVIDER_32768     32768 = 131.072us = 7.629394531kHz
        BCM2835_SPI_CLOCK_DIVIDER_16384     16384 = 65.536us = 15.25878906kHz
        BCM2835_SPI_CLOCK_DIVIDER_8192      8192 = 32.768us = 30/51757813kHz
        BCM2835_SPI_CLOCK_DIVIDER_4096      4096 = 16.384us = 61.03515625kHz
        BCM2835_SPI_CLOCK_DIVIDER_2048      2048 = 8.192us = 122.0703125kHz
        BCM2835_SPI_CLOCK_DIVIDER_1024      1024 = 4.096us = 244.140625kHz
        BCM2835_SPI_CLOCK_DIVIDER_512       512 = 2.048us = 488.28125kHz
        BCM2835_SPI_CLOCK_DIVIDER_256       256 = 1.024us = 976.5625kHz
        BCM2835_SPI_CLOCK_DIVIDER_128       128 = 512ns = = 1.953125MHz
        BCM2835_SPI_CLOCK_DIVIDER_64        64 = 256ns = 3.90625MHz
        BCM2835_SPI_CLOCK_DIVIDER_32        32 = 128ns = 7.8125MHz
        BCM2835_SPI_CLOCK_DIVIDER_16        16 = 64ns = 15.625MHz
        BCM2835_SPI_CLOCK_DIVIDER_8         8 = 32ns = 31.25MHz
        BCM2835_SPI_CLOCK_DIVIDER_4         4 = 16ns = 62.5MHz
        BCM2835_SPI_CLOCK_DIVIDER_2         2 = 8ns = 125MHz, fastest you can get
    */
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    #endif
    return true;
}


void LedDisplay::read_button(bool force){
    #ifdef RPI
    uint8_t value;
    ++button_cnt;
    //printf("read button_ctr: %d\n", button_cnt);
    if(button_cnt>BUTTON_CTR||force) {
        value = bcm2835_gpio_lev(BTN);
        if(value==0) {
            button = button+1;
            printf("button status: %d\n", button);
        }
        button_cnt = 0;
    }
    #endif
}

int LedDisplay::get_button(){
    return button;
}

void LedDisplay::set_button(){
    button = button+1;
}


void LedDisplay::clr_button(){
    button = 0;
}


void LedDisplay::show(){
    read_button(false);
    if(button==0) {
        gpio_show();
    };
}


void LedDisplay::gpio_show(){
    char data_c;
    int bin_c;

    for(int i=0;i<BYTES_CH;++i){
        for(int b=7;b>=0;b--){
            for(int ch=0;ch<TOTAL_CH;ch++){
                data_c = ledData[ch][i];
                bin_c = (data_c>>b)&1;

                //if(ch==0 && i>=710*4 && i<=710*4+4 ){
                //    printf("BYTES: %0d %0d %0d\n",i,data_c,bin_c);
                //}

                #ifdef RPI
                switch(ch){
                    case 0:
                            if(bin_c==1)
                                bcm2835_gpio_write(CH_0, HIGH);
                            else
                                bcm2835_gpio_write(CH_0, LOW);
                            break;
                    case 1:
                            if(bin_c==1)
                                bcm2835_gpio_write(CH_1, HIGH);
                            else
                                bcm2835_gpio_write(CH_1, LOW);
                            break;
                    case 2:
                            if(bin_c==1)
                                bcm2835_gpio_write(CH_2, HIGH);
                            else
                                bcm2835_gpio_write(CH_2, LOW);
                            break;
                }
                #endif

            }
            #ifdef RPI
            bcm2835_gpio_write(CLK, HIGH);
            bcm2835_delayMicroseconds(0.00001);
            bcm2835_gpio_write(CLK, LOW);
            bcm2835_delayMicroseconds(0.00001);
            #endif
        }
    }

}


void LedDisplay::spi_show(){
    #ifdef RPI
    bcm2835_spi_writenb(ledData[0], BYTES);
    #endif
}


void LedDisplay::spi_end(){
    #ifdef RPI
    //close the spi bus
    bcm2835_spi_end();
    bcm2835_close();
    #endif
}


void LedDisplay::gpio_end(){
    #ifdef RPI
    bcm2835_close();
    #endif
}


void LedDisplay::sleep(int d){
    #ifdef RPI
    if(d>100)
        read_button(true);
    else
        read_button(false);
    if(button==0) bcm2835_delay(d);
    #else
    if(button==0) usleep(d);
    #endif
}


void LedDisplay::fps_test(){
    time_t start, stop;
    double seconds;

    if( SPI_OK ) {
        // send to LEDs
        time(&start);
        for(int j=1;j<=LED_TOTAL_CH;j++){
            clear();
            for(int ch=0;ch<TOTAL_CH;ch++)
                pixelRGB(ch,j,default_brightness,255,0,0);
            show();
        }
        time(&stop);
        seconds = difftime(stop,start);
        printf("SPI Time: %.20f seconds to run\n",seconds);
        printf("SPI FPS: %.20f\n",LED_TOTAL/seconds);
    }
}


