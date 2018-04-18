

#include "GtkDisplay.hpp"


GtkDisplay::GtkDisplay() {
    int win_w = Constants::MORPHO_DISPLAY_DX*Constants::MORPHO_DISPLAY_W*Constants::MORPHO_DISPLAY_SCALE;
    int win_h = Constants::MORPHO_DISPLAY_DY*Constants::MORPHO_DISPLAY_H*Constants::MORPHO_DISPLAY_SCALE;
    ratio = Constants::MORPHO_DISPLAY_2R*Constants::MORPHO_DISPLAY_SCALE;
    leds_x = Constants::MORPHO_DISPLAY_W;
    leds_y = Constants::MORPHO_DISPLAY_H;
    data = new int[leds_x*leds_y*4];
    data_draw = new int[leds_x*leds_y*4];

    printf("win size %dx%d\n",leds_x,leds_y);
    printf("win size %dx%d\n",win_w,win_h);
    set_size_request(win_w,win_h);
    default_brightness = 1;

    clear();
}

GtkDisplay::~GtkDisplay() {
}

void GtkDisplay::setBrightness(int b){
    default_brightness = b&0x1F;
}


int GtkDisplay::get_button(){
    return button;
}


void GtkDisplay::set_button(){
    printf("button set\n");
    button = button+1;
}


void GtkDisplay::clr_button(){
    button = 0;
}

void GtkDisplay::clear() {
    MorphoColor c;
    c.R = 0;
    c.B = 0;
    c.G = 0;
    for(int x=0;x<leds_x;++x)
        for(int y=0;y<leds_y;++y)
            pixelRGB_xy(x,y,c);
}

double GtkDisplay::getR_xy(int x, int y){
    double val = data_draw[(x+y*leds_x)*4+0]/255.00;
    //printf("\t <= pixel %0d.%0d = %2f.X.X\n",x,y,val);
    return val;
}
double GtkDisplay::getG_xy(int x, int y){
    double val = data_draw[(x+y*leds_x)*4+1]/255.00;
    //printf("\t <= pixel %0d.%0d = X.%2f.X\n",x,y,val);
    return val;
}
double GtkDisplay::getB_xy(int x, int y){
    double val = data_draw[(x+y*leds_x)*4+2]/255.00;
    //printf("\t <= pixel %0d.%0d = X.X.%2f\n",x,y,val);
    return val;
}
double GtkDisplay::getBR_xy(int x, int y){
    double val = data_draw[(x+y*leds_x)*4+3]/31.00*0.50;
    val = 0.5 + val;
    // if( x==6 && y==0 )
    //     printf("\t <= pixel %0d.%0d = %2f\n",x,y,val);
    return val;
}

void GtkDisplay::pixelRGB_xy(int x, int y, MorphoColor c){
    {
        m_Mutex.lock();
        //printf("\t => gtk %0d.%0d = %0d.%0d.%0d\n",x,y,R,G,B);
        data[(x+y*leds_x)*4+0] = c.R;
        data[(x+y*leds_x)*4+1] = c.G;
        data[(x+y*leds_x)*4+2] = c.B;
        data[(x+y*leds_x)*4+3] = default_brightness;
        m_Mutex.unlock();
    }
}

void GtkDisplay::pixelBRGB_xy(int x, int y, MorphoColor c){
    {
        m_Mutex.lock();
        //printf("\t => gtk %0d.%0d = %0d.%0d.%0d\n",x,y,R,G,B);
        data[(x+y*leds_x)*4+0] = c.R;
        data[(x+y*leds_x)*4+1] = c.G;
        data[(x+y*leds_x)*4+2] = c.B;
        data[(x+y*leds_x)*4+3] = c.br&0x1F;
        m_Mutex.unlock();
    }
}


void GtkDisplay::sleep(int ms){
    if(button==0) {
        if( ms<Constants::MORPHO_DISPLAY_MIN_SLEEP  )
            ms = 10;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

void GtkDisplay::show(){
    //queue_draw();
    // force our program to redraw the entire clock.
    auto win = get_window();
    if (win && button==0) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}


bool GtkDisplay::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    // This is where we draw on the window
    Gtk::Allocation allocation = get_allocation();
    const float dx = allocation.get_width()/leds_x;
    const float dy = allocation.get_height()/leds_y;

    m_Mutex.lock();
    for(int x=0;x<leds_x*leds_y*4;++x)
        data_draw[x] = data[x];


    // printf("\tpixel %d.%d\n",allocation.get_width(),allocation.get_height());

    // coordinates for the center of the window
    float xc, yc;
    cr->save();
    cr->set_source_rgba(0.1, 0.1, 0.1, 0.9);   // green
    cr->paint();

    cr->set_line_width(0);  // outline thickness changes
                            // with window size

    for(int x=0;x<leds_x;++x)
        for(int y=0;y<leds_y;++y) {
            xc = x*dx+dx/2;
            yc = y*dy+dy/2;
            // printf("\tpixel %0d.%0d => %2f.%2f\n",x,y,xc,yc);
            cr->save();
            cr->arc(xc, yc, ratio/2, 0.0, 2.0*M_PI);        // full circle
            cr->set_source_rgba( getR_xy(x,y), getG_xy(x,y), getB_xy(x,y), getBR_xy(x,y));    // partially translucent
            cr->fill_preserve();
            cr->restore();  // back to opaque black
            cr->stroke();
        }

    m_Mutex.unlock();
    return true;
}
