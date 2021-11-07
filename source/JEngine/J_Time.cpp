#include "J_Time.h"

/// J_TIMER ///////////////////////////////////////////////////////////////////

void J_Timer::start ()
{
    // Starts the timer.
    started = true, paused = false;
    startTicks = SDL_GetTicks(), pauseTicks = 0;
}

void J_Timer::stop ()
{
    // Stop and reset the timer.
    started = false, paused = false;
    startTicks = 0, pauseTicks = 0;
}



void J_Timer::pause ()
{
    // If the timer is unpaused then pause it and vice versa.
    if (started)
    {
        if (paused)
        {
            paused = false;
            startTicks = SDL_GetTicks() - pauseTicks;
            pauseTicks = 0;
        }
        else
        {
            paused = true;
            pauseTicks = SDL_GetTicks() - startTicks;
            startTicks = 0;
        }
    }
}



Uint32 J_Timer::getTicks () const
{
    // Return how long the timer has been running for, in ticks.
	Uint32 ticks = 0;
	if (started) { ticks = (paused) ? pauseTicks : (SDL_GetTicks() - startTicks); }

	return ticks;
}



bool operator== (const J_Timer& a_lhs, const J_Timer& a_rhs) { return (a_lhs.getTicks() == a_rhs.getTicks()) ? true : false; }
bool operator!= (const J_Timer& a_lhs, const J_Timer& a_rhs) { return !operator==(a_lhs, a_rhs); }
bool operator< (const J_Timer& a_lhs, const J_Timer& a_rhs) { return (a_lhs.getTicks() < a_rhs.getTicks()) ? true : false; }
bool operator> (const J_Timer& a_lhs, const J_Timer& a_rhs) { return operator<(a_rhs, a_lhs); }
bool operator<= (const J_Timer& a_lhs, const J_Timer& a_rhs) { return !operator>(a_lhs, a_rhs); }
bool operator>= (const J_Timer& a_lhs, const J_Timer& a_rhs) { return !operator<(a_lhs, a_rhs); }

/// J_TIMER ///////////////////////////////////////////////////////////////////



/// J_TIME ////////////////////////////////////////////////////////////////////

const int J_Time::SECOND = 1000;
double J_Time::delta = 0.0;



void J_Time::setDelta (const double a_delta)
{
    delta = a_delta;
}



double J_Time::getDelta ()
{
    return delta;
}

Uint32 J_Time::getTime ()
{
    return SDL_GetTicks();
}

/// J_TIME ////////////////////////////////////////////////////////////////////
