// Noise.h
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

#ifndef Noise_H
#define Noise_H

#include "enums.h"
#include "Fractal.h"
#include "Perturb.h"

class Noise {
public:
    //! \brief Create FastNoseCL object
    Noise(const int seed = 1337);

    /*! \brief Sets seed used for all noise types
     * Default: 1337
     */
    void setSeed(const int seed);

    //! \brief Returns seed used for all noise types
    int getSeed() const;

    /*! \brief Sets frequency for all noise types
     * Default: 0.01
     */
    void setFrequency(const float frequency);

    /*! \brief Changes the smoothing function method used to interpolate between noise values
     * Used in Value, Perlin Noise
     * Default: Quintic
     * \param Possible interpolation methods (lowest to highest quality) :
     * Linear, Hermite, Quintic.
     */
    void setSmoothingFunction(const Smoothing smoothing);

    /*! \brief Sets noise return type of getNoise(...)
     * Default: Simplex
     * \param Possible noise types:
     * Perlin, PerlinFractal, Simplex, SimplexFractal, Cellular, WhiteNoise.
     */
    void setNoiseType(const NoiseType noiseType);

    /*! \brief Sets method for combining octaves in all fractal noise types
     * Default: FBM
     * \param Possible fractal types:
     * FBM, Billow, RigidMulti.
     */
    void setFractalType(const FractalType fractalType);

    /*! \brief Sets return type from cellular noise calculations
     * Note: NoiseLookup requires another FastNoiseCL object be set with setCellularNoiseLookup() to function
     * Default: CellValue
     * \param Possible distance function types:
     * Euclidean, Manhattan, Natural.
     */
    void setCellularDistanceFunction(const CellularDistanceFunction cellularDistanceFunction);

    /*! \brief Sets distance function used in cellular noise calculations
     * Default: Euclidean
     * \param Possible return types:
     * CellValue, NoiseLookup, Distance, Distance2, Distance2Add, Distance2Sub, Distance2Mul, Distance2Div.
     */
    void setCellularReturnType(const CellularReturnType cellularReturnType);

    /*! \brief Noise used to calculate a cell value if cellular return type is NoiseLookup.
     * The lookup value is acquired through getNoise() so ensure you setNoiseType() on the noise lookup, value, Perlin or simplex is recommended
     */
    void setCellularNoiseLookup(Noise* noise);

    /*! \brief Sets random deviation multiplier for CellularNoise
     * Default: 1.0 (bound between 0.0 and 1.0)
     */
    void setCellularJitter(const float cellularJitter);
    /*! \brief Determines which distances will be used for calculation in CellularNoise
     * Default: 0 and 1
     */
    void setCellularDistance2Indices(const int cellularDistanceIndex0, const int cellularDistanceIndex1);
    //! \brief Sets Perturb object for generator to use
    void setPerturb(Perturb* perturb);
    void setFractal(Fractal*  fractal);

    float getFrequency() const;
    Smoothing getSmoothingFunction() const;
    NoiseType getNoiseType() const;
    int getFractalOctaves() const;
    float getFractalLacunarity() const;
    float getFractalGain() const;
    FractalType getFractalType() const;
    CellularDistanceFunction getCellularDistanceFunction() const;
    CellularReturnType getCellularReturnType() const;
    Noise* getCellularNoiseLookup() const;
    float getCellularJitter() const;
    int getcellularDistanceIndex0() const;
    int getcellularDistanceIndex1() const;
    Perturb* getPerturb() const;
    Fractal* getFractal() const;

protected:
    int m_seed;
    float m_frequency;
    float m_cellularJitter;
    int m_cellularDistanceIndex0;
    int m_cellularDistanceIndex1;
    Smoothing m_smoothing;
    NoiseType m_noiseType;

    FractalType m_fractalType;

    Perturb* m_perturb;
    Fractal* m_fractal;

    CellularDistanceFunction m_cellularDistanceFunction;
    CellularReturnType m_cellularReturnType;
    Noise* m_cellularNoiseLookup;
};

#endif
