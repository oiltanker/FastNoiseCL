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

#include "FastNoiseCL.h"
#include <math.h>
#include <assert.h>
#include <random>

void FastNoiseCL::SetSeed(int seed)
{
	m_seed = seed;

	std::mt19937 gen(seed);

	for (int i = 0; i < 256; i++)
		m_perm[i] = i;

	for (int j = 0; j < 256; j++)
	{
		std::uniform_int_distribution<> dis(0, 256 - j);
		int k = dis(gen) + j;
		int l = m_perm[j];
		m_perm[j] = m_perm[j + 256] = m_perm[k];
		m_perm[k] = l;
		m_perm12[j] = m_perm12[j + 256] = m_perm[j] % 12;
	}
}

//OpenCL generation
FastNoiseCL::Range FastNoiseCL::NullRange(0.0f, 0.0f, 0.0f);
FastNoiseCL::RangeInt FastNoiseCL::NullRangeInt(0, 0, 0);

//2D
float* FastNoiseCL::GetValue(Range x, Range y) {
    return kernel_adapter->GEN_Value2(
        m_frequency,
        static_cast<int>(m_interp),
        m_perm,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}
float* FastNoiseCL::GetValueFractal(Range x, Range y) {
    return kernel_adapter->GEN_ValueFractal2(
        m_frequency, static_cast<int>(m_fractalType),
        m_octaves, m_lacunarity, m_gain, m_fractalBounding,
        static_cast<int>(m_interp),
        m_perm,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}

float* FastNoiseCL::GetPerlin(Range x, Range y) {
    return kernel_adapter->GEN_Perlin2(
        m_frequency,
        static_cast<int>(m_interp),
        m_perm, m_perm12,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}
float* FastNoiseCL::GetPerlinFractal(Range x, Range y) {
    return kernel_adapter->GEN_PerlinFractal2(
        m_frequency, static_cast<int>(m_fractalType),
        m_octaves, m_lacunarity, m_gain, m_fractalBounding,
        static_cast<int>(m_interp),
        m_perm,  m_perm12,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}

float* FastNoiseCL::GetSimplex(Range x, Range y) {
    return kernel_adapter->GEN_Simplex2(
        m_frequency,
        m_perm, m_perm12,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}
float* FastNoiseCL::GetSimplexFractal(Range x, Range y) {
    return kernel_adapter->GEN_SimplexFractal2(
        m_frequency, static_cast<int>(m_fractalType),
        m_octaves, m_lacunarity, m_gain, m_fractalBounding,
        m_perm, m_perm12,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}

float* FastNoiseCL::GetCellular(Range x, Range y) {
    return kernel_adapter->GEN_Cellular2(
        m_frequency,
        static_cast<int>(m_cellularDistanceFunction), static_cast<int>(m_cellularReturnType),
        m_perm, m_seed,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}

float* FastNoiseCL::GetWhiteNoise(Range x, Range y) {
    return kernel_adapter->GEN_WhiteNoise2(
        m_seed,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}
float* FastNoiseCL::GetWhiteNoiseInt(RangeInt x, RangeInt y) {
    return kernel_adapter->GEN_WhiteNoiseInt2(
        m_seed,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}

//float* FastNoiseCL::GetNoise(Range x, Range y);

//3D
float* FastNoiseCL::GetValue(Range x, Range y, Range z) {
    return kernel_adapter->GEN_Value3(
        m_frequency,
        static_cast<int>(m_interp),
        m_perm,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}
float* FastNoiseCL::GetValueFractal(Range x, Range y, Range z) {
    return kernel_adapter->GEN_ValueFractal3(
        m_frequency, static_cast<int>(m_fractalType),
        m_octaves, m_lacunarity, m_gain, m_fractalBounding,
        static_cast<int>(m_interp),
        m_perm,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}

float* FastNoiseCL::GetPerlin(Range x, Range y, Range z) {
    return kernel_adapter->GEN_Perlin3(
        m_frequency,
        static_cast<int>(m_interp),
        m_perm, m_perm12,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}
float* FastNoiseCL::GetPerlinFractal(Range x, Range y, Range z) {
    return kernel_adapter->GEN_PerlinFractal3(
        m_frequency, static_cast<int>(m_fractalType),
        m_octaves, m_lacunarity, m_gain, m_fractalBounding,
        static_cast<int>(m_interp),
        m_perm,  m_perm12,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}

float* FastNoiseCL::GetSimplex(Range x, Range y, Range z) {
    return kernel_adapter->GEN_Simplex3(
        m_frequency,
        m_perm, m_perm12,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}
float* FastNoiseCL::GetSimplexFractal(Range x, Range y, Range z) {
    return kernel_adapter->GEN_SimplexFractal3(
        m_frequency, static_cast<int>(m_fractalType),
        m_octaves, m_lacunarity, m_gain, m_fractalBounding,
        m_perm, m_perm12,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}

float* FastNoiseCL::GetCellular(Range x, Range y, Range z) {
    return kernel_adapter->GEN_Cellular3(
        m_frequency,
        static_cast<int>(m_cellularDistanceFunction), static_cast<int>(m_cellularReturnType),
        m_perm, m_seed,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}

float* FastNoiseCL::GetWhiteNoise(Range x, Range y, Range z) {
    return kernel_adapter->GEN_WhiteNoise3(
        m_seed,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}
float* FastNoiseCL::GetWhiteNoiseInt(RangeInt x, RangeInt y, RangeInt z) {
    return kernel_adapter->GEN_WhiteNoiseInt3(
        m_seed,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}

//float* FastNoiseCL::GetNoise(Range x, Range y, Range z);

//4D
float* FastNoiseCL::GetSimplex(Range x, Range y, Range z, Range w) {
    return kernel_adapter->GEN_Simplex4(
        m_frequency,
        m_perm,

        x.size, y.size, z.size, w.size,
        x.step, y.step, z.step, w.step,
        x.offset, y.offset, z.offset, w.offset
    );
}

float* FastNoiseCL::GetWhiteNoise(Range x, Range y, Range z, Range w) {
    return kernel_adapter->GEN_WhiteNoise4(
        m_seed,

        x.size, y.size, z.size, w.size,
        x.step, y.step, z.step, w.step,
        x.offset, y.offset, z.offset, w.offset
    );
}
float* FastNoiseCL::GetWhiteNoiseInt(RangeInt x, RangeInt y, RangeInt z, RangeInt w) {
    return kernel_adapter->GEN_WhiteNoiseInt4(
        m_seed,

        x.size, y.size, z.size, w.size,
        x.step, y.step, z.step, w.step,
        x.offset, y.offset, z.offset, w.offset
    );
}

void FastNoiseCL::PrepareDevice(Device& device) {
    kernel_adapter = std::unique_ptr<KernelAdapter>(new KernelAdapter(device));
}
