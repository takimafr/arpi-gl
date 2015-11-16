/*
 * Copyright (C) 2015  eBusiness Information
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "common/Timer.hpp"
#include "utils/Log.hpp"

constexpr char TAG[] = "Timer";

namespace dma {

    //-----------------------------------------------------------
    Timer::Timer() :
                        mDT(0.0f),
                        mLastTime(0.0f)
    {}


    //-----------------------------------------------------------
    Timer::~Timer() {

    }


    //-----------------------------------------------------------
    void Timer::reset(){
        Log::trace(TAG, "Resetting Timer");
        mDT = 0.0f;
        mLastTime = now();
    }


    //-----------------------------------------------------------
    void Timer::update(){
        double currentTime = now();
        mDT = (float) (currentTime - mLastTime);
        mLastTime = currentTime;
    }


    //-----------------------------------------------------------
    double Timer::now(){
        timespec timeVal;
        clock_gettime(CLOCK_MONOTONIC, &timeVal);
        return timeVal.tv_sec + (timeVal.tv_nsec * 1.0e-9);
    }


    //-----------------------------------------------------------
    float Timer::dt(){
        return mDT;
    }


    //-----------------------------------------------------------
    float Timer::liveDT(){
        return (float) (now() - mLastTime);
    }


}
