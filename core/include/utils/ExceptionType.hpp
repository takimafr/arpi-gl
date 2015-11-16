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
 * ExceptionType.h
 *
 *  Created on: 20 juil. 2015
 *      Author: excilys
 */

#ifndef _EXCEPTIONTYPE_HPP_
#define _EXCEPTIONTYPE_HPP_

namespace dma {

    enum ExceptionType {
        NO_SUCH_ELEMENT, INVALID_VALUE, PARSE_ERROR, OPENGL, IO, INVALID_FILE, UNKNOWN, MEMORY, INITIALIZATION, ALREADY_EXISTS,

        NO_EXCEPTION = Status::STATUS_OK
    };

} /* namespace dma */

#endif /* _EXCEPTIONTYPE_HPP_ */
