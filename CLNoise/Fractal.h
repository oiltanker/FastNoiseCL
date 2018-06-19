// Fractal.h
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

#ifndef Fractal_H
#define Fractal_H

class Fractal {
public:
    Fractal();
    Fractal(const int octaves, const float lacunarity, const float gain);

    /*! \brief Sets octave count for fractal perturb type
     * Default: 3
     */
    void setOctaves(const int octaves);
    /*! \brief Sets octave lacunarity for fractal perturb type
     * Default: 2.0
     */
    void setLacunarity(const float lacunarity);
    /*! \brief Sets octave gain for fractal perturb type
     * Default: 0.5
     */
    void setGain(const float gain);

    int getOctaves() const;
    float getLacunarity() const;
    float getGain() const;
    float getBounding() const;

protected:
    int m_octaves;
    float m_lacunarity;
    float m_gain;
    float m_bounding;

    void CalculateBounding();
};

#endif
