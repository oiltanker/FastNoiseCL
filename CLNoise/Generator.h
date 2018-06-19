// Generator.h
//
// MIT License
//
// Copyright(c) 2017 Oiltanker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The developer's email is mentioned on GitHub profile
//

#ifndef Generator_H
#define Generator_H

#include <cstdlib>
#include "DeviceManager.h"
#include "Noise.h"

template<typename T>
class RangeContainer {
public:
    std::size_t size;
    T offset;
    T step;

    RangeContainer(std::size_t size, T offset, T step);
    RangeContainer& operator= (RangeContainer& r);
};
//! \brief contains information about range of coordinate floating-point values to be used in generation
typedef RangeContainer<float> Range;

//! \brief stores results of noise get functions
class NoiseBuffer {
public:
    size_t size = 0;
    float* data = nullptr;

    NoiseBuffer(size_t size, float* data);
    ~NoiseBuffer();
};

class Generator {
public:
    //! \brief Create generator
    Generator(const Device& device);
    ~Generator();

    //! \brief Sets Noise object for generator to use
    void setNoise(Noise* noise);
    Noise* getNoise() const;

    // Generation
    //2D
    NoiseBuffer getNoise(const Range& x, const Range& y);

    //3D
    NoiseBuffer getNoise(const Range& x, const Range& y, const Range& z);

    //4D
    //! \brief Only works with noise types of Simplex of WhiteNoise
    NoiseBuffer getNoise(const Range& x, const Range& y, const Range& z, const Range& w);

protected:
    float* m_buffer;
    size_t m_bufSize;

    Noise* m_noise;

private:
    bool prepare(const size_t size);
    void prepareBuffer(size_t size);
    void prepareDevice(const Device& device);

    class impl;
    impl& rimpl;
};

#endif
