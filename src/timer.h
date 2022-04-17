#include <iostream>
#include <thread>

namespace game
{
    typedef uint64_t timer_tick_t;
 
    class Clock
    {
    private:
        float frequency;
        timer_tick_t ticks;
        int period_us;
        void _thread_loop();
        bool running;
        std::thread* thread_obj;

    public:
        Clock();
        void start();
        void stop();
        void set_freq(float freq);
        float get_freq();
        timer_tick_t get_ticks();
        void wait(int _ticks);
        ~Clock();
    };
}