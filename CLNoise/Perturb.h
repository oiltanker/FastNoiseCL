// Perturb.h
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

#ifndef Perturb_H
#define Perturb_H

#include "enums.h"
#include "Fractal.h"

class Perturb {
public:
    Perturb(const int seed = 1337);

    /*! \brief Sets seed used by perturb
     * Default: 1337
     */
    void setSeed(const int seed);

    //! \brief Returns seed used by perturb
    int getSeed() const;

    /*! \brief Sets frequency for Perturb
     * Default: 0.01
     */
    void setFrequency(const float amplitude);

    /*! \brief Sets the maximum warp distance from original location when using Perturb{Fractal}(...).
     * Default: 1.0
     */
    void setAmplitude(const float frequency);

    /*! \brief Changes the smoothing function method used to interpolate between noise values
     * Default: Quintic
     * \param Possible interpolation methods (lowest to highest quality) :
     * Linear, Hermite, Quintic.
     */
    void setSmoothingFunction(const Smoothing smoothing);

    /*! \brief Sets perturb function type.
     * Default: None
     * \param Possible perturb types:
     * None, Single, Fractal.
     */
    void setPerturbType(const PerturbType type);
    void setFractal(Fractal*  fractal);

    float getFrequency() const;
    float getAmplitude() const;
    Smoothing getSmoothingFunction() const;
    PerturbType getPerturbType() const;
    Fractal* getFractal() const;

protected:
    int m_seed;
    float m_amplitude;
    float m_frequency;
    PerturbType m_type;
    Smoothing m_smoothing;

    Fractal* m_fractal;
};

#endif
