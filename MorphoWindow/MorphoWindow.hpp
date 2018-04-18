
#ifndef MORPHO_WINDOW_H
#define MORPHO_WINDOW_H

#include <gtkmm.h>
#include "MorphoWorker.hpp"
#include "VirtualDisplay.hpp"
#include "GtkDisplay.hpp"


class MorphoWindow : public Gtk::Window {
    public:
        GtkDisplay gtk;
        MorphoWindow();

        // Called from the worker thread.
        void notify();
        MorphoWorker* m_Worker;

    private:
        // Signal handlers.
        void on_start_button_clicked();
        void on_stop_button_clicked();
        void on_quit_button_clicked();

        void update_start_stop_buttons();
        void update_widgets();

        // Dispatcher handler.
        void on_notification_from_worker_thread();

        // Member data.
        Gtk::Box m_VBox;
        Gtk::ButtonBox m_ButtonBox;
        Gtk::Button m_ButtonStart;
        Gtk::Button m_ButtonStop;
        Gtk::Button m_ButtonQuit;
        Gtk::ProgressBar m_ProgressBar;
        Gtk::ScrolledWindow m_ScrolledWindow;
        Gtk::TextView m_TextView;

        Glib::Dispatcher m_Dispatcher;
        std::thread* m_WorkerThread;
};

#endif // MORPHO_WINDOW_H
