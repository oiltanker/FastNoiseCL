// Noise.cpp
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

#include "Noise.h"

#include <algorithm>

#define FN_CELLULAR_INDEX_MAX 3

// initialization
Noise::Noise(const int seed) {
    m_seed = seed;
    m_frequency = 0.01f;
    m_cellularJitter = 1.0f;
    m_cellularDistanceIndex0 = 0;
    m_cellularDistanceIndex1 = 1;

    m_smoothing = Smoothing::Quintic;
    m_noiseType = NoiseType::Simplex;

    m_fractalType = FractalType::FBM;

    m_perturb = nullptr;
    m_fractal = nullptr;

    m_cellularDistanceFunction = CellularDistanceFunction::Euclidean;
    m_cellularReturnType = CellularReturnType::CellValue;
    m_cellularNoiseLookup = nullptr;
};

// Getters/Setters
void Noise::setCellularDistance2Indices(const int cellularDistanceIndex0, const int cellularDistanceIndex1) {
    m_cellularDistanceIndex0 = std::min(cellularDistanceIndex0, cellularDistanceIndex1);
    m_cellularDistanceIndex1 = std::max(cellularDistanceIndex0, cellularDistanceIndex1);

    m_cellularDistanceIndex0 = std::min(std::max(m_cellularDistanceIndex0, 0), FN_CELLULAR_INDEX_MAX);
    m_cellularDistanceIndex1 = std::min(std::max(m_cellularDistanceIndex1, 0), FN_CELLULAR_INDEX_MAX);
}
void Noise::setCellularJitter(const float cellularJitter) {
    m_cellularJitter = cellularJitter;
}
void Noise::setCellularNoiseLookup(Noise* noise) {
    m_cellularNoiseLookup = noise;
}
void Noise::setCellularReturnType(const CellularReturnType cellularReturnType) {
    m_cellularReturnType = cellularReturnType;
}
void Noise::setCellularDistanceFunction(const CellularDistanceFunction cellularDistanceFunction) {
    m_cellularDistanceFunction = cellularDistanceFunction;
}
void Noise::setFractalType(const FractalType fractalType) {
    m_fractalType = fractalType;
}
void Noise::setNoiseType(const NoiseType noiseType) {
    m_noiseType = noiseType;
}
void Noise::setSmoothingFunction(const Smoothing smoothing) {
    m_smoothing = smoothing;
}
void Noise::setFrequency(const float frequency) {
    m_frequency = frequency;
}
void Noise::setSeed(const int seed) {
    m_seed = seed;
}
void Noise::setPerturb(Perturb* perturb) {
    m_perturb = perturb;
}
void Noise::setFractal(Fractal*  fractal) {
    m_fractal = fractal;
}

Perturb* Noise::getPerturb() const {
    return m_perturb;
}
int Noise::getSeed() const {
    return m_seed;
}
float Noise::getFrequency() const {
    return m_frequency;
}
Smoothing Noise::getSmoothingFunction() const {
    return m_smoothing;
}
NoiseType Noise::getNoiseType() const {
    return m_noiseType;
}
FractalType Noise::getFractalType() const {
    return m_fractalType;
}
CellularDistanceFunction Noise::getCellularDistanceFunction() const {
    return m_cellularDistanceFunction;
}
CellularReturnType Noise::getCellularReturnType() const {
    return m_cellularReturnType;
}
Noise* Noise::getCellularNoiseLookup() const {
    return m_cellularNoiseLookup;
}
float Noise::getCellularJitter() const {
    return m_cellularJitter;
}
int Noise::getcellularDistanceIndex0() const {
    return m_cellularDistanceIndex0;
}
int Noise::getcellularDistanceIndex1() const {
    return m_cellularDistanceIndex1;
}
Fractal* Noise::getFractal() const {
    return m_fractal;
}
