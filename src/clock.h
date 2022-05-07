#ifndef _CLOCK_H
#define _CLOCK_H

#include <iostream>
#include <thread>
#include <mutex>

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

        std::mutex clock_lock;

    public:
        Clock();
        void start();
        void stop();
        void reset();
        void set_ticks(clock_tick_t ticks);
        void set_freq(float freq);
        float get_freq();
        int get_period_us();
        clock_tick_t get_ticks();
        void wait(float ticks);
        void wait_until(float ticks);
        ~Clock();
    };
}
#endif