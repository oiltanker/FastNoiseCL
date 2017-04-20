// FastNoiseCLCL.cpp
//
// MIT License
//
// Copyright(c) 2016 Oiltanker
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

#ifndef FastNoiseCL_H
#define FastNoiseCL_H

#include <cstddef>
#include <memory>
#include "DeviceManager.hpp"
#include "KernelAdapter.hpp"

class FastNoiseCL
{
private:
    template<typename T>
    struct RangeContainer {
        std::size_t size;
        T offset;
        T step;

        RangeContainer(std::size_t size, T offset, T step) {
            this->size = size;
            this->offset = offset;
            this->step = step;
        }
        RangeContainer& operator= (RangeContainer& r) {
            this->size = r.size;
            this->offset = r.offset;
            this->step = r.step;

            return *this;
        }
    };
public:
	FastNoiseCL(Device& device, int seed = 1337) {
	    SetSeed(seed);
	    CalculateFractalBounding();
	    PrepareDevice(device);
    };
	~FastNoiseCL() { delete m_cellularNoiseLookup; }

	typedef RangeContainer<float> Range;
	typedef RangeContainer<int> RangeInt;
	static Range NullRange;
	static RangeInt NullRangeInt;

	enum NoiseType { Value, ValueFractal, Perlin, PerlinFractal, Simplex, SimplexFractal, Cellular, WhiteNoise };
	enum Interp { Linear, Hermite, Quintic };
	enum FractalType { FBM, Billow, RigidMulti };
	enum CellularDistanceFunction { Euclidean, Manhattan, Natural };
	enum CellularReturnType { CellValue, NoiseLookup, Distance, Distance2, Distance2Add, Distance2Sub, Distance2Mul, Distance2Div };

	// Returns seed used for all noise types
	void SetSeed(int seed);

	// Sets seed used for all noise types
	// Default: 1337
	int GetSeed(void) const { return m_seed; }

	// Sets frequency for all noise types
	// Default: 0.01
	void SetFrequency(float frequency) { m_frequency = frequency; }

	// Changes the interpolation method used to smooth between noise values
	// Possible interpolation methods (lowest to highest quality) :
	// - Linear
	// - Hermite
	// - Quintic
	// Used in Value, Perlin Noise and Position Warping
	// Default: Quintic
	void SetInterp(Interp interp) { m_interp = interp; }

	// Sets noise return type of GetNoise(...)
	// Default: Simplex
	void SetNoiseType(NoiseType noiseType) { m_noiseType = noiseType; }

	// Sets octave count for all fractal noise types
	// Default: 3
	void SetFractalOctaves(int octaves) { m_octaves = octaves; CalculateFractalBounding(); }

	// Sets octave lacunarity for all fractal noise types
	// Default: 2.0
	void SetFractalLacunarity(float lacunarity) { m_lacunarity = lacunarity; }

	// Sets octave gain for all fractal noise types
	// Default: 0.5
	void SetFractalGain(float gain) { m_gain = gain; CalculateFractalBounding(); }

	// Sets method for combining octaves in all fractal noise types
	// Default: FBM
	void SetFractalType(FractalType fractalType) { m_fractalType = fractalType; }

	// Sets return type from cellular noise calculations
	// Note: NoiseLookup requires another FastNoiseCL object be set with SetCellularNoiseLookup() to function
	// Default: CellValue
	void SetCellularDistanceFunction(CellularDistanceFunction cellularDistanceFunction) { m_cellularDistanceFunction = cellularDistanceFunction; }

	// Sets distance function used in cellular noise calculations
	// Default: Euclidean
	void SetCellularReturnType(CellularReturnType cellularReturnType) { m_cellularReturnType = cellularReturnType; }

	// Noise used to calculate a cell value if cellular return type is NoiseLookup
	// The lookup value is acquired through GetNoise() so ensure you SetNoiseType() on the noise lookup, value, Perlin or simplex is recommended
	void SetCellularNoiseLookup(FastNoiseCL* noise) { m_cellularNoiseLookup = noise; }

	// Sets the maximum warp distance from original location when using Perturb{Fractal}(...)
	// Default: 1.0
	void SetPerturbAmp(float perturbAmp) { m_perturbAmp = perturbAmp / 0.45f; }

	//OpenCL generation
	//2D
	float* GetValue(Range x, Range y);
	float* GetValueFractal(Range x, Range y);

	float* GetPerlin(Range x, Range y);
	float* GetPerlinFractal(Range x, Range y);

	float* GetSimplex(Range x, Range y);
	float* GetSimplexFractal(Range x, Range y);

	float* GetCellular(Range x, Range y);

	float* GetWhiteNoise(Range x, Range y);
	float* GetWhiteNoiseInt(RangeInt x, RangeInt y);

	float* GetNoise(Range x, Range y);

	//I don't rally know man
	/*void Perturb(float& x, float& y);
	void PerturbFractal(float& x, float& y);*/

	//3D
	float* GetValue(Range x, Range y, Range z);
	float* GetValueFractal(Range x, Range y, Range z);

	float* GetPerlin(Range x, Range y, Range z);
	float* GetPerlinFractal(Range x, Range y, Range z);

	float* GetSimplex(Range x, Range y, Range z);
	float* GetSimplexFractal(Range x, Range y, Range z);

	float* GetCellular(Range x, Range y, Range z);

	float* GetWhiteNoise(Range x, Range y, Range z);
	float* GetWhiteNoiseInt(RangeInt x, RangeInt y, RangeInt z);

	float* GetNoise(Range x, Range y, Range z);

	//I don't rally know man
	/*void Perturb(float& x, float& y, float& z);
	void PerturbFractal(float& x, float& y, float& z);*/

	//4D
	float* GetSimplex(Range x, Range y, Range z, Range w);

	float* GetWhiteNoise(Range x, Range y, Range z, Range w);
	float* GetWhiteNoiseInt(RangeInt x, RangeInt y, RangeInt z, RangeInt w);

protected:
	unsigned char m_perm[512];
	unsigned char m_perm12[512];

	int m_seed = 1337;
	float m_frequency = 0.01f;
	Interp m_interp = Quintic;
	NoiseType m_noiseType = Simplex;

	int m_octaves = 3;
	float m_lacunarity = 2.0f;
	float m_gain = 0.5f;
	FractalType m_fractalType = FBM;

	float m_fractalBounding;
	void CalculateFractalBounding()
	{
		float amp = m_gain;
		float ampFractal = 1.0f;
		for (unsigned int i = 1; i < m_octaves; i++)
		{
			ampFractal += amp;
			amp *= m_gain;
		}
		m_fractalBounding = 1.0f / ampFractal;
	}

	CellularDistanceFunction m_cellularDistanceFunction = Euclidean;
	CellularReturnType m_cellularReturnType = CellValue;
	FastNoiseCL* m_cellularNoiseLookup = nullptr;

	float m_perturbAmp = 1.0f / 0.45f;

    std::unique_ptr<KernelAdapter> kernel_adapter;

private:
    void PrepareDevice(Device& device);
};
#endif
