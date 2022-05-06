#include "clock.h"
#include <unistd.h>

namespace game
{
    Clock::Clock()
    {
        ticks = 0;
        thread_obj = nullptr;
    }

    void Clock::start()
    {
        if (!running)
        {
            running = true;
            thread_obj = new std::thread([=]
                                         { _thread_loop(); });
        }
    }

    void Clock::stop()
    {
        running = false;
        if (thread_obj && thread_obj->joinable())
        {
            thread_obj->join();
            delete thread_obj;
            thread_obj = nullptr;
        }
    }

    void Clock::reset() { ticks = 0; }

    void Clock::set_ticks(clock_tick_t ticks)
    {
        this->ticks = ticks;
    }

    void Clock::set_freq(float freq)
    {
        frequency = freq;
        period_us = 1'000'000 / frequency;
    }

    int Clock::get_period_us() { return period_us; }

    float Clock::get_freq() { return frequency; }

    clock_tick_t Clock::get_ticks() { return ticks; }

    void Clock::wait(float ticks) { usleep(period_us * ticks); }

    void Clock::wait_until(float ticks) { usleep(period_us * (ticks - this->ticks)); }

    void Clock::_thread_loop()
    {
        while (running)
        {
            usleep(period_us);
            ticks++;
        }
    }

    Clock::~Clock()
    {
        stop();
    }

}