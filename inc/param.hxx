/*
 * Created Date: Wednesday June 7th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Wednesday June 7th 2023 4:46:23 pm
 * Modified By: DefinitelyNotAGirl@github (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <cstdint>
#include <string>
#include <list>

#define CARGPARSE_HANDLER_ARGS uint64_t argnum, std::list<std::string>& args

namespace cargparse
{
    typedef void handlertype;
    typedef void(*handlerFunction)(CARGPARSE_HANDLER_ARGS);

    class param
    {
    friend class instance;
    protected:
        bool needsSpace;
        uint64_t argnum;
        std::string prefix;
        handlertype(*handler)(CARGPARSE_HANDLER_ARGS);

    public:
        param(bool needsSpace, uint64_t argnum, std::string prefix, handlerFunction handler);
    };

    //example: -I/inc/blub/
    //needsSpace = false
    //argnum = 1
    //prefix = -I

    //example: -I /inc/blub/
    //needsSpace = true
    //argnum = 1
    //prefix = -I

    //example: -include /inc/blub/
    //needsSpace = true
    //argnum = 1
    //prefix = -include
}