/* -------------------------------------------------------------------------------

    C O N S T A N T S
    Constants.hpp
    Archivo global con la definicion de constantes


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

#ifndef CONSTANTS_H
#define CONSTANTS_H


// Constants.hpp
namespace Constants {

    const int MORPHO_DISPLAY_W = 144;       // number of leds X
#ifdef RPI  
    const int MORPHO_DISPLAY_H = 15;        // number of leds Y
#else
    const int MORPHO_DISPLAY_H = 40;        // number of leds Y
#endif  

    /*
                              DX
                <-------------------------->

                +--------------------------+    ^
                |                          |    |
                |                          |    |
                |                          |    |
                |      +------------+      |    |
                |      |            |      |    |
                |      |            |      |    |
                |      |     2R     |      |    |  DY
                |      | <--------> |      |    |
                |      |            |      |    |
                |      |            |      |    |
                |      +------------+      |    |
                |                          |    |
                |                          |    |
                |                          |    |
                +--------------------------+    v
    */

    // Additional constants for the GtkDisplay 
    const int MORPHO_DISPLAY_DX = 7;            // mm
    //const int MORPHO_DISPLAY_DY = 12;         // mm
    const int MORPHO_DISPLAY_DY = 9;            // mm
    const int MORPHO_DISPLAY_2R = 5;            // diameter mm
    const float MORPHO_DISPLAY_SCALE = 1.2;     // magnifying factor
    const int MORPHO_DISPLAY_MIN_SLEEP = 10;    // minimal sleep to avoid flickering in the GTK display


}

#endif
