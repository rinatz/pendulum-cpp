// MIT License
//
// Copyright (c) 2022 IDA Kenichiro
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef PENDULUM_EXCEPTIONS_H_
#define PENDULUM_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace pendulum {

class PendulumException : public std::exception {
   public:
    PendulumException() = default;
    explicit PendulumException(const std::string& error) : error_(error) {}
    PendulumException(const PendulumException&) = default;
    PendulumException(PendulumException&&) = default;

    virtual ~PendulumException() = default;

    PendulumException& operator=(const PendulumException&) = default;
    PendulumException& operator=(PendulumException&&) = default;

    const char* what() const noexcept override { return error_.c_str(); }

   private:
    std::string error_;
};

class InvalidUnit : public PendulumException {
   public:
    InvalidUnit() = default;
    explicit InvalidUnit(const std::string& error) : PendulumException(error) {}
    InvalidUnit(const InvalidUnit&) = default;
    InvalidUnit(InvalidUnit&&) = default;

    virtual ~InvalidUnit() = default;

    InvalidUnit& operator=(const InvalidUnit&) = default;
    InvalidUnit& operator=(InvalidUnit&&) = default;
};

class InvalidTimezone : public PendulumException {
   public:
    InvalidTimezone() = default;
    explicit InvalidTimezone(const std::string& error) : PendulumException(error) {}
    InvalidTimezone(const InvalidTimezone&) = default;
    InvalidTimezone(InvalidTimezone&&) = default;

    virtual ~InvalidTimezone() = default;

    InvalidTimezone& operator=(const InvalidTimezone&) = default;
    InvalidTimezone& operator=(InvalidTimezone&&) = default;
};

class UnsupportedFormat : public PendulumException {
   public:
    UnsupportedFormat() = default;
    explicit UnsupportedFormat(const std::string& error) : PendulumException(error) {}
    UnsupportedFormat(const UnsupportedFormat&) = default;
    UnsupportedFormat(UnsupportedFormat&&) = default;

    virtual ~UnsupportedFormat() = default;

    UnsupportedFormat& operator=(const UnsupportedFormat&) = default;
    UnsupportedFormat& operator=(UnsupportedFormat&&) = default;
};

}  // namespace pendulum

#endif  // PENDULUM_EXCEPTIONS_H_
