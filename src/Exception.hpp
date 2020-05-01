/*
 *  Drm Exception class
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * Copyright (C) 2016 EPAM Systems Inc.
 *
 */

#ifndef SRC_EXCEPTION_HPP_
#define SRC_EXCEPTION_HPP_

#include <ilm/ilm_types.h>

#include <string>

/*******************************************************************************
 * Exception generated by DisplayManager.
 * @ingroup DisplayManager
 ******************************************************************************/
class DmException : public std::exception {
public:
    /**
     * @param msg error message
     */
    explicit DmException(const std::string& msg, int error = ILM_FAILED)
        : mMsg(msg + " (Error: " + std::to_string(error) + ")"){};

    virtual ~DmException() {}

    /**
     * returns error message
     */
    const char* what() const throw() override { return mMsg.c_str(); };

private:
    std::string mMsg;
};

#endif /* SRC_EXCEPTION_HPP_ */
