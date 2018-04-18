#ifndef GTK_DISPLAY_H
#define GTK_DISPLAY_H

#include "Constants.hpp"
#include "MorphoColor.hpp"
#include "VirtualDisplay.hpp"

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <cmath>
#include <thread>
#include <mutex>


class GtkDisplay : public Gtk::DrawingArea {

    public:
        GtkDisplay();
        ~GtkDisplay();

        void show();
        void redraw();
        void clear();
        void sleep(int ms);
        void setBrightness(int b);
        void pixelRGB_xy(int x, int y, MorphoColor c);
        void pixelBRGB_xy(int x, int y, MorphoColor c);
        void set_button();
        void clr_button();
        int get_button();

    private:
        int ratio, leds_x, leds_y;
        int default_brightness;
        double getR_xy(int x, int y);
        double getG_xy(int x, int y);
        double getB_xy(int x, int y);
        double getBR_xy(int x, int y);
        int *data;
        int *data_draw;

        int button;

        // Synchronizes access to member data.
        mutable std::mutex m_Mutex;



    protected:
        //Override default signal handler:
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

#endif // GTK_DISPLAY_H
