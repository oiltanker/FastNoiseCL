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
#include <vector>

void FastNoiseCL::SetSeed(int seed)
{
	m_seed = seed;

	std::mt19937 gen(seed);
}

//OpenCL generation
FastNoiseCL::Range FastNoiseCL::NullRange(0.0f, 0.0f, 0.0f);
FastNoiseCL::RangeInt FastNoiseCL::NullRangeInt(0, 0, 0);

//2D
float* FastNoiseCL::GetValue(Range x, Range y) {
    return kernel_adapter->GEN_Value2(
        CreateSnapshot(),

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}
float* FastNoiseCL::GetValueFractal(Range x, Range y) {
    return kernel_adapter->GEN_ValueFractal2(
        CreateSnapshot(),

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}

float* FastNoiseCL::GetPerlin(Range x, Range y) {
    return kernel_adapter->GEN_Perlin2(
        CreateSnapshot(),

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}
float* FastNoiseCL::GetPerlinFractal(Range x, Range y) {
    return kernel_adapter->GEN_PerlinFractal2(
        CreateSnapshot(),

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}

float* FastNoiseCL::GetSimplex(Range x, Range y) {
    return kernel_adapter->GEN_Simplex2(
        CreateSnapshot(),

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}
float* FastNoiseCL::GetSimplexFractal(Range x, Range y) {
    return kernel_adapter->GEN_SimplexFractal2(
        CreateSnapshot(),

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}

float* FastNoiseCL::GetCellular(Range x, Range y) {
    if (m_cellularReturnType != NoiseLookup) {
        return kernel_adapter->GEN_Cellular2(
            CreateSnapshot(),

            x.size, y.size,
            x.step, y.step,
            x.offset, y.offset
        );
    } else {
        std::vector<Snapshot> params;
        FastNoiseCL* fnp = this;

        while (fnp != nullptr) {
            Snapshot s = fnp->CreateSnapshot();
            params.push_back(s);
            fnp = fnp->m_cellularNoiseLookup;
        }
        params[0].m_noiseType = static_cast<int>(Cellular);

        return kernel_adapter->GEN_Lookup_Cellular2(
            params.data(), params.size(),

            x.size, y.size,
            x.step, y.step,
            x.offset, y.offset
        );
    }
}

float* FastNoiseCL::GetWhiteNoise(Range x, Range y) {
    return kernel_adapter->GEN_WhiteNoise2(
        CreateSnapshot(),

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}
float* FastNoiseCL::GetWhiteNoiseInt(RangeInt x, RangeInt y) {
    return kernel_adapter->GEN_WhiteNoiseInt2(
        CreateSnapshot(),

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset
    );
}

//float* FastNoiseCL::GetNoise(Range x, Range y);

//3D
float* FastNoiseCL::GetValue(Range x, Range y, Range z) {
    return kernel_adapter->GEN_Value3(
        CreateSnapshot(),

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}
float* FastNoiseCL::GetValueFractal(Range x, Range y, Range z) {
    return kernel_adapter->GEN_ValueFractal3(
        CreateSnapshot(),

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}

float* FastNoiseCL::GetPerlin(Range x, Range y, Range z) {
    return kernel_adapter->GEN_Perlin3(
        CreateSnapshot(),

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}
float* FastNoiseCL::GetPerlinFractal(Range x, Range y, Range z) {
    return kernel_adapter->GEN_PerlinFractal3(
        CreateSnapshot(),

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}

float* FastNoiseCL::GetSimplex(Range x, Range y, Range z) {
    return kernel_adapter->GEN_Simplex3(
        CreateSnapshot(),

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}
float* FastNoiseCL::GetSimplexFractal(Range x, Range y, Range z) {
    return kernel_adapter->GEN_SimplexFractal3(
        CreateSnapshot(),

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}

float* FastNoiseCL::GetCellular(Range x, Range y, Range z) {
    if (m_cellularReturnType != NoiseLookup) {
        return kernel_adapter->GEN_Cellular3(
            CreateSnapshot(),

            x.size, y.size, z.size,
            x.step, y.step, z.step,
            x.offset, y.offset, z.offset
        );
    } else {
        std::vector<Snapshot> params;
        FastNoiseCL* fnp = this;

        while (fnp != nullptr) {
            Snapshot s = fnp->CreateSnapshot();
            params.push_back(s);
            fnp = fnp->m_cellularNoiseLookup;
        }
        params[0].m_noiseType = static_cast<int>(Cellular);

        return kernel_adapter->GEN_Lookup_Cellular3(
            params.data(), params.size(),

            x.size, y.size, z.size,
            x.step, y.step, z.step,
            x.offset, y.offset, z.offset
        );
    }
}

float* FastNoiseCL::GetWhiteNoise(Range x, Range y, Range z) {
    return kernel_adapter->GEN_WhiteNoise3(
        CreateSnapshot(),

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}
float* FastNoiseCL::GetWhiteNoiseInt(RangeInt x, RangeInt y, RangeInt z) {
    return kernel_adapter->GEN_WhiteNoiseInt3(
        CreateSnapshot(),

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset
    );
}

//float* FastNoiseCL::GetNoise(Range x, Range y, Range z);

//4D
float* FastNoiseCL::GetSimplex(Range x, Range y, Range z, Range w) {
    return kernel_adapter->GEN_Simplex4(
        CreateSnapshot(),

        x.size, y.size, z.size, w.size,
        x.step, y.step, z.step, w.step,
        x.offset, y.offset, z.offset, w.offset
    );
}

float* FastNoiseCL::GetWhiteNoise(Range x, Range y, Range z, Range w) {
    return kernel_adapter->GEN_WhiteNoise4(
        CreateSnapshot(),

        x.size, y.size, z.size, w.size,
        x.step, y.step, z.step, w.step,
        x.offset, y.offset, z.offset, w.offset
    );
}
float* FastNoiseCL::GetWhiteNoiseInt(RangeInt x, RangeInt y, RangeInt z, RangeInt w) {
    return kernel_adapter->GEN_WhiteNoiseInt4(
        CreateSnapshot(),

        x.size, y.size, z.size, w.size,
        x.step, y.step, z.step, w.step,
        x.offset, y.offset, z.offset, w.offset
    );
}

Snapshot FastNoiseCL::CreateSnapshot() {
    Snapshot snap;

    snap.m_seed =  m_seed;
    snap.m_frequency = m_frequency;
    snap.m_interp = static_cast<int>(m_interp);
    snap.m_noiseType = static_cast<int>(m_noiseType);

    snap.m_octaves = m_octaves;
    snap.m_lacunarity = m_lacunarity;
    snap.m_gain = m_gain;
    snap.m_fractalType = static_cast<int>(m_fractalType);

    snap.m_fractalBounding = m_fractalBounding;

    snap.m_cellularDistanceFunction = static_cast<int>(m_cellularDistanceFunction);
    snap.m_cellularReturnType = static_cast<int>(m_cellularReturnType);

    snap.m_perturbAmp = m_perturbAmp;
    snap.m_perturb = static_cast<int>(m_perturb);

    return snap;
}

void FastNoiseCL::PrepareDevice(Device& device) {
    kernel_adapter = std::unique_ptr<KernelAdapter>(new KernelAdapter(device));
}
