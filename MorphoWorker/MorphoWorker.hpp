

#ifndef MORPHO_WORKER_H
#define MORPHO_WORKER_H

#include "LedDisplay.hpp"
#include "Constants.hpp"
#include "PowerEffect.hpp"
#include "FadeInOutEffect.hpp"
#include "LineEffect.hpp"
#include "ColorEffect.hpp"

#include <thread>
#include <mutex>

#include "opencv2/opencv.hpp"

#ifdef MORPHO_GTK
    #include <gtkmm.h>
    #include "GtkDisplay.hpp"
#endif

class MorphoWindow;

class MorphoWorker {
    public:
         MorphoWorker();

        // Thread function.
        #ifdef MORPHO_GTK
            void do_work(MorphoWindow* caller, GtkDisplay* gui);
            void get_data(double* fraction_done, Glib::ustring* message) const;
        #else
            void do_work();
        #endif

        void stop_work();
        bool has_stopped() const;

    private:
        LedDisplay led;

        // Synchronizes access to member data.
        mutable std::mutex m_Mutex;

        // Data used by both GUI thread and worker thread.
        bool m_shall_stop;
        bool m_has_stopped;
        double m_fraction_done;

        void load_img(cv::Mat& mat,string img_path);
        #ifdef MORPHO_GTK
            Glib::ustring m_message;
        #endif

};

#endif // MORPHO_WORKER_H
