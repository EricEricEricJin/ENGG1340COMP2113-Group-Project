#ifndef _CLOCK_H
#define _CLOCK_H

#include <iostream>
#include <thread>

namespace game
{
    typedef uint64_t clock_tick_t;

    class Clock
    {
    private:
        float frequency;
        clock_tick_t ticks;
        int period_us;
        void _thread_loop();
        bool running;
        std::thread *thread_obj;

    public:
        Clock();
        void start();
        void stop();
        void reset();
        void set_freq(float freq);
        float get_freq();
        clock_tick_t get_ticks();
        void wait(int _ticks);
        ~Clock();
    };
}
#endif