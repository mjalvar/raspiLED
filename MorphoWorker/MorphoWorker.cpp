

#include <sstream>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "MorphoWorker.hpp"

#ifdef MORPHO_GTK
    #include "MorphoWindow.hpp"
#endif


#ifdef MORPHO_GTK
MorphoWorker::MorphoWorker() :
    m_Mutex(),
    m_shall_stop(false),
    m_has_stopped(false),
    m_fraction_done(0.0),
    m_message() {
}
#else
MorphoWorker::MorphoWorker() :
    m_Mutex(),
    m_shall_stop(false),
    m_has_stopped(false),
    m_fraction_done(0.0) {
}
#endif


void MorphoWorker::load_img(cv::Mat& mat,string img_path){
    if( !access( img_path.c_str(), 0 ) ){
        mat = cv::imread(img_path);
        printf("loading %s\n",img_path.c_str());
    }
    else
        printf("Error: no access to %s\n",img_path.c_str());
}


#ifdef MORPHO_GTK
// Accesses to these data are synchronized by a mutex.
// Some microseconds can be saved by getting all data at once, instead of having
// separate get_fraction_done() and get_message() methods.
void MorphoWorker::get_data(double* fraction_done, Glib::ustring* message) const {
    std::lock_guard<std::mutex> lock(m_Mutex);

    if (fraction_done)
        *fraction_done = m_fraction_done;

    if (message)
        *message = m_message;
}
#endif

void MorphoWorker::stop_work() {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_shall_stop = true;
}

bool MorphoWorker::has_stopped() const {
    std::lock_guard<std::mutex> lock(m_Mutex);
    return m_has_stopped;
}


/* ---------------------------------
    do_work
   ---------------------------------
*/
#ifdef MORPHO_GTK
    void MorphoWorker::do_work(MorphoWindow* caller,GtkDisplay* gui) {
    GtkDisplay* D;
    D = gui;
#else
    void MorphoWorker::do_work() {
    LedDisplay* D;
    D = &led;
#endif

    VirtualDisplay* VD;
    VD = (VirtualDisplay*) D;
    MorphoColor c;

    #ifdef MORPHO_GTK
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_has_stopped = false;
        m_fraction_done = 0.0;
        m_message = "";
    } // The mutex is unlocked here by lock's destructor.
    #endif

    printf("D:%p VD:%p\n",D,VD);


    // Prueba de FPS
    #ifndef MORPHO_GTK
    //D->fps_test();
    #endif

    // Prueba Borders
    c.blue();
    for(int x=0;x<Constants::MORPHO_DISPLAY_W;++x) {
        D->pixelRGB_xy(x,0,c);
        D->pixelRGB_xy(x,Constants::MORPHO_DISPLAY_H-1,c);
    }
    for(int y=0;y<Constants::MORPHO_DISPLAY_H;++y) {
        c.green();
        D->pixelRGB_xy(0,y,c);
        D->pixelRGB_xy(10,y,c);
        c.red();
        D->pixelRGB_xy(Constants::MORPHO_DISPLAY_W-10,y,c);
        D->pixelRGB_xy(Constants::MORPHO_DISPLAY_W-1,y,c);
        D->pixelRGB_xy(Constants::MORPHO_DISPLAY_W-10,y,c);
    }
    D->show();

    printf("Prueba de bordes, presione botton para continuar\n");
    while( D->get_button()==0 ){
        D->sleep(1000);
        #ifdef MORPHO_GTK
        D->set_button();
        #endif
    }
    D->clear();
    D->clr_button();


    // Prueba de power
    /*
    printf("Prueba de power\n");
    PowerEffect power(VD);
    power.delay=100;
    power.run();
    */


    // Loading Effects
    cv::Mat felizIMG;
    cv::Mat campanasIMG;
    cv::Mat ranaIMG;
    cv::Mat ranaIMG_mask;
    cv::Mat colochosIMG;
    cv::Mat logoIMG;
    cv::Mat logo2IMG;
    cv::Mat logoIMG_mask;
    cv::Mat carretaIMG;
    load_img(felizIMG,"images/feliznav_144x40.png");
    load_img(campanasIMG,"images/campanas_144x40.png");
    load_img(ranaIMG,"images/rana_144x40.png");
    load_img(ranaIMG_mask,"images/rana_mask_144x40.png");
    load_img(colochosIMG,"images/colochos_144x40.png");
    load_img(logoIMG,"images/logo_144x40.png");
    load_img(logo2IMG,"images/logo_144x40.jpg");
    load_img(logoIMG_mask,"images/logo_mask_144x40.png");
    load_img(carretaIMG,"images/carreta_144x40.png");

    LineEffect line(VD);
    line.up_down = true;

    ColorEffect color(VD);

    FadeInOutEffect fade(VD);
    fade.use_br = false;

    // Video
    cv::VideoCapture testVID("videos/morpho.mp4");
    //testVID.set(CV_CAP_PROP_FOURCC, CV_FOURCC('H', '2', '6', '4'));
    if( !testVID.isOpened()){
         cout << "Cannot open the video file" << endl;
    }
    else {
         int Fcnt=0;
         cv::Mat edges;
         cv::Mat frame;
         D->clear();
         while( testVID.read(frame) && D->get_button()==0 ) {
             //testVID >> frame; // get a new frame from camera
             //cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
            //printf("frame %0d\n",Fcnt);
            for(int x=0;x<Constants::MORPHO_DISPLAY_W;++x) {
                for(int y=0;y<Constants::MORPHO_DISPLAY_H;++y) {
                    cv::Vec3b img_color = frame.at<cv::Vec3b>(y,x);
                    c.B = img_color.val[0];
                    c.G = img_color.val[1];
                    c.R = img_color.val[2];
                    //printf("pixel %0d,%0d: %0d.%0d.%0d\n",x,y,img_color.val[2],img_color.val[1],img_color.val[0]);
                    if( !c.is_black(50))
                        D->pixelRGB_xy(x,y,c);
                }
            }
            D->show();
            #ifdef MORPHO_GTK
            D->sleep(100);
            D->set_button();
            #else
            D->sleep(1);
            #endif
            D->clear();
            ++Fcnt;
         }
    }

    // // Simulate a long calculation.
    for (int i = 0; ; ++i) {// do until break

        #ifdef MORPHO_GTK
        //std::this_thread::sleep_for(std::chrono::milliseconds(250));
        {
            std::lock_guard<std::mutex> lock(m_Mutex);

            m_fraction_done += 0.001;

            if (i % 4 == 3) {
                std::ostringstream ostr;
                ostr << (m_fraction_done * 100.0) << "% done\n";
                m_message += ostr.str();
            }

            if (m_fraction_done >= 1.0) {
                m_message += "Finished";
                break;
            }
            if (m_shall_stop) {
                m_message += "Stopped";
                break;
            }
        }
        caller->notify();
        #endif


        // Main Loop
        // Running effects
        // Use ONLY PNG to avoid noise in the RPi version
        printf("MAIN loop starting...\n");
        if( D->get_button()!=0) {
            printf("Finalizando\n");
            #ifndef MORPHO_GTK
            D->clr_button();
            D->clear();
            D->show();
            D->sleep(1000);
            system("sudo poweroff");
            #endif
            break;
        }
        D->clear();
        fade.clear = false;
        fade.set_img(felizIMG);
        fade.fade_in();
        D->sleep(2000);
        fade.set_img(campanasIMG);
        fade.fade_in();
        D->sleep(2000);

        fade.set_img(felizIMG);
        fade.fade_out();
        fade.clear = true;
        D->sleep(1000);

        line.set_img(ranaIMG);
        line.run();
        D->sleep(1000);

        color.set_img(ranaIMG);
        color.set_mask(ranaIMG_mask);
        color.swap(true);
        D->sleep(1000);
        color.swap(false);
        D->sleep(5000);

        fade.set_img(ranaIMG);
        fade.fade_out();

        fade.set_img(logoIMG);
        fade.fade_in();
        D->sleep(5000);
        fade.fade_out();

        fade.random_in();
        D->sleep(3000);
        fade.random_out();
        D->sleep(1000);

        color.set_img(colochosIMG);
        color.random_in(150);
        D->sleep(3000);
        color.random_out(150);
        D->sleep(1000);

        color.set_img(logoIMG);
        color.set_mask(logoIMG_mask);
        color.swap(true);
        D->sleep(5000);
        color.swap(false);
        D->sleep(2000);

        color.set_img(carretaIMG);
        color.random_in(200);
        D->sleep(3000);
        color.random_out(200);
        D->sleep(1000);

    }

    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_shall_stop = false;
        m_has_stopped = true;
    }
    #ifdef MORPHO_GTK
    caller->notify();
    #endif
}
