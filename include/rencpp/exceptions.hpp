#ifndef RENCPP_EXCEPTIONS_HPP
#define RENCPP_EXCEPTIONS_HPP

//
// exceptions.hpp
// This file is part of RenCpp
// Copyright (C) 2015 HostileFork.com
//
// Licensed under the Boost License, Version 1.0 (the "License")
//
//      http://www.boost.org/LICENSE_1_0.txt
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied.  See the License for the specific language governing
// permissions and limitations under the License.
//
// See http://rencpp.hostilefork.com for more information on this project
//

#include <exception>
#include <sstream>

#include "values.hpp"

//
// This is where to place any custom exceptions that are part of the contract
// between Value-based operations and the user, who may wish to catch these
// named exceptions and handle them.
//
// Exactly how many exceptions will be exposed is up for debate, as opposed
// to handling the exceptions inside of the code on the other side of the
// binding and returning a ren::Error.  Also: is it necessary to recreate
// the entire error! spectrum with a matching exception, or just catch all
// with one that gives you an error! ?
//

namespace ren {


//
// Both Ren runtime code that is written in C++ and that is not written in
// C++ is able to raise evaluation errors.  If C++ code, you do this by
// throwing the ren::Error directly.  However, if you "bubble up" to C++
// code which is making a call into the runtime, the exception that emerges
// should derive from std::exception.
//
class evaluation_error : public std::exception {
private:
    Error errorValue;
    std::string whatString;

public:
    evaluation_error (Error const & error) :
        errorValue (error),
        whatString (to_string(errorValue))
    {
    }

    char const * what() const noexcept override {
        return whatString.c_str();
    }

    Error error() const noexcept {
        return errorValue;
    }
};


class exit_command : public std::exception {
private:
    int codeValue;
    std::string whatString;

public:
    exit_command (int code) :
        codeValue (code)
    {
        std::ostringstream ss;
        ss << "ren::exit_command(" << code << ")";
        whatString = ss.str();
    }

    char const * what() const noexcept override {
        return whatString.c_str();
    }

    int code() const {
        return codeValue;
    }
};


// What should the interface for cancellations of evaluations be?  How might
// timeouts or quotas of operations be managed?
//
// https://github.com/hostilefork/rencpp/issues/19

class evaluation_cancelled : public std::exception {
public:
    evaluation_cancelled ()
    {
    }

    char const * what() const noexcept override {
        return "ren::evaluation_cancelled";
    }
};

}

#endif
