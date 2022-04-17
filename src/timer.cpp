#include "timer.h"
#include <unistd.h>

namespace game
{
    Clock::Clock()
    {
        ticks = 0;
    }

    void Clock::start()
    {
        running = true;
        thread_obj = new std::thread([=]
                                     { _thread_loop(); });
    }

    void Clock::stop()
    {
        running = false;
        thread_obj->join();
        delete thread_obj;
    }

    void Clock::reset() { ticks = 0; }

    void Clock::set_freq(float freq)
    {
        frequency = freq;
        period_us = 1'000'000 / frequency;
    }

    float Clock::get_freq() { return frequency; }

    timer_tick_t Clock::get_ticks() { return ticks; }

    void Clock::wait(int _ticks) { usleep(period_us * _ticks); }

    void Clock::_thread_loop()
    {
        while (running)
        {
            usleep(period_us);
            ticks++;
        }
    }

    Clock::~Clock() {}

}