/* -------------------------------------------------------------------------------

    M A I N
    main.cpp


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

#include "Constants.hpp"

#include "MorphoWorker.hpp"
#include "CmdParser.hpp"

#ifdef MORPHO_GTK
    #include "MorphoWindow.hpp"
    #include <gtkmm/application.h>
    #include <gtkmm/window.h>
#endif




int main (int argc, char **argv) {
    CmdParser cmd(argc,argv);
    MorphoWorker W;


    /* ---------------------------------
        Initialization
       ---------------------------------
    */
    #ifdef MORPHO_GTK
        argc = 1;
        auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
        MorphoWindow win;
        win.m_Worker = &W;
        //D = &win;
    #else
        W.do_work();
    #endif


    /* ---------------------------------
        Protocol release
       ---------------------------------
    */
    #ifdef MORPHO_GTK
        //win.set_default_size(win_w,win_h);
        //win.add(gtk);
        //gtk.show();
    return app->run(win);
    #else

    #endif

    return 0;
}
