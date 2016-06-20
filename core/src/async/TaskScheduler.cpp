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



/*
 * TaskScheduler.cpp
 *
 *  Created on: 20 juil. 2015
 *      Author: excilys
 */
#include <algorithm>

#include "async/TaskScheduler.hpp"

namespace dma {


    TaskScheduler::TaskScheduler() {
        
    }


    TaskScheduler::~TaskScheduler() {
        cancelAll();
    }



    void TaskScheduler::operator<<(std::function<void()> task) {
        std::lock_guard<std::mutex> guard(mLock);
        mTasks.push_back(task);
    }



    int TaskScheduler::flush() {
        std::lock_guard<std::mutex> guard(mLock);
        int count = 0;
        while (!mTasks.empty()) {
            auto& task = mTasks.front();
            task();
            ++count;
            mTasks.pop_front();
        }
        return count;
    }



    int TaskScheduler::cancelAll() {
        std::lock_guard<std::mutex> guard(mLock);
        int count = (int) mTasks.size();
        mTasks.clear();
        return count;
    }

    void TaskScheduler::operator()() {
        flush();
    }

} /* namespace dma */
