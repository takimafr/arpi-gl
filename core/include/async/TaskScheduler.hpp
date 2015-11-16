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
 * TaskScheduler.hpp
 *
 *  Created on: 20 juil. 2015
 *      Author: excilys
 */

#ifndef _TASKSCHEDULER_HPP_
#define _TASKSCHEDULER_HPP_

#include <list>
#include <map>
#include <mutex>



namespace dma {

    class TaskScheduler {
    public:

        TaskScheduler();

        /**
         * Destroys this scheduler, cancelling the pending tasks it has in queue.
         */
        virtual ~TaskScheduler();
        void operator<<(std::function<void()> task);

        /* ***
         * Same as flush
         */
        void operator()();

        /**
         * Execute all pending tasks
         */
        int flush();

        /**
         * Cancel all pending tasks of all scheduler instances.
         */
        int cancelAll();


    private:
        /* ***
         * ATTRIBUTES
         */

        std::list<std::function<void()>>  mTasks;
        std::mutex mLock;
    };

} /* namespace dma */

#endif /* _TASKSCHEDULER_HPP_ */
