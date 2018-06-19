// Generator.cpp
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

#include "Generator.h"
#include "KernelAdapter.h"

#include <math.h>
#include <assert.h>
#include <random>
#include <vector>
#include <algorithm>

#define FN_CELLULAR_INDEX_MAX 3

// NoiseBuffer
NoiseBuffer::NoiseBuffer(size_t size, float* data) {
    this->size = size;
    this->data = data;
}
NoiseBuffer::~NoiseBuffer() {
    if (size) delete[] data;
}

// Generator::impl
class Generator::impl {
public:
    KernelAdapter* m_kernelAdapter;
    Snapshot createSnapshot(const Noise* noise) const;
    std::vector<Snapshot> buildSnapshotChain() const;

    impl(const Generator* generator) {
        m_generator = generator;
        m_kernelAdapter = nullptr;
    }
    ~impl() {
        if (m_kernelAdapter) delete m_kernelAdapter;
    }
private:
    const Generator* m_generator;
};
Snapshot Generator::impl::createSnapshot(const Noise* noise) const {
    Snapshot snap;

    if (noise) {
        const Noise& n = *noise;

        snap.m_seed =  n.getSeed();
        snap.m_frequency = n.getFrequency();
        snap.m_cellularJitter = n.getCellularJitter();
        snap.m_cellularDistanceIndex0 = n.getcellularDistanceIndex0();
        snap.m_cellularDistanceIndex1 = n.getcellularDistanceIndex1();
        snap.m_smoothing = static_cast<int>(n.getSmoothingFunction());
        snap.m_noiseType = static_cast<int>(n.getNoiseType());

        if (n.getFractal()) {
            snap.m_octaves = n.getFractal()->getOctaves();
            snap.m_lacunarity = n.getFractal()->getLacunarity();
            snap.m_gain = n.getFractal()->getGain();
            snap.m_fractalBounding = n.getFractal()->getBounding();
        }

        snap.m_fractalType = static_cast<int>(n.getFractalType());

        snap.m_cellularDistanceFunction = static_cast<int>(n.getCellularDistanceFunction());
        snap.m_cellularReturnType = static_cast<int>(n.getCellularReturnType());

        if (n.getPerturb()) {
            const Perturb& p = *n.getPerturb();
            snap.m_perturb = static_cast<int>(p.getPerturbType());
            snap.m_perturbAmp = p.getAmplitude();
            snap.m_perturbFrequency = p.getFrequency();

            if (snap.m_perturb == 2) {
                if (p.getFractal()) {
                    snap.m_perturbBounding = p.getFractal()->getBounding();
                    snap.m_perturbGain = p.getFractal()->getGain();
                    snap.m_perturbLacunarity = p.getFractal()->getLacunarity();
                    snap.m_perturbOctaves = p.getFractal()->getOctaves();
                } else snap.m_perturb = 1;
            }

            snap.m_perturbSeed = p.getSeed();
            snap.m_perturbSmoothing = static_cast<int>(p.getSmoothingFunction());
        } else snap.m_perturb = 0;
    }

    return snap;
}
std::vector<Snapshot> Generator::impl::buildSnapshotChain() const {
    std::vector<Snapshot> params;
    const Noise* fnp = m_generator->m_noise;

    while (fnp != nullptr) {
        Snapshot s = createSnapshot(fnp);
        params.push_back(s);
        fnp = fnp->getCellularNoiseLookup();
    }
    params[0].m_noiseType = static_cast<int>(NoiseType::Cellular);

    return params;
}

// Generator
// initialization
Generator::Generator(const Device& device) : rimpl(*(new impl(this))){
    m_noise = nullptr;

    m_buffer = nullptr;
    m_bufSize = 0;

    prepareDevice(device);
};
Generator::~Generator() {
    delete &rimpl;
}

// Generation
template <typename T1, typename T2>
NoiseBuffer Get2D(float* m_buffer, size_t m_bufSize, KernelAdapter* m_kernelAdapter, Snapshot snapshot, const T1& x, const T1& y, void (KernelAdapter::*nf) (Snapshot, size_t, size_t, T2, T2, T2, T2, float*)) {
    (m_kernelAdapter->*nf)(
        snapshot,

        x.size, y.size,
        x.step, y.step,
        x.offset, y.offset,

        m_buffer
    );

    return NoiseBuffer(m_bufSize, m_buffer);
}
template <typename T1, typename T2>
NoiseBuffer Get3D(float* m_buffer, size_t m_bufSize, KernelAdapter* m_kernelAdapter, Snapshot snapshot, const T1& x, const T1& y, const T1& z, void (KernelAdapter::*nf) (Snapshot, size_t, size_t, size_t, T2, T2, T2, T2, T2, T2, float*)) {
    (m_kernelAdapter->*nf)(
        snapshot,

        x.size, y.size, z.size,
        x.step, y.step, z.step,
        x.offset, y.offset, z.offset,

        m_buffer
    );

    return NoiseBuffer(m_bufSize, m_buffer);
}
template <typename T1, typename T2>
NoiseBuffer Get4D(float* m_buffer, size_t m_bufSize, KernelAdapter* m_kernelAdapter, Snapshot snapshot, const T1& x, const T1& y, const T1& z, const T1& w, void (KernelAdapter::*nf) (Snapshot, size_t, size_t, size_t, size_t, T2, T2, T2, T2, T2, T2, T2, T2, float*)) {
    (m_kernelAdapter->*nf)(
        snapshot,

        x.size, y.size, z.size, w.size,
        x.step, y.step, z.step, w.step,
        x.offset, y.offset, z.offset, w.offset,

        m_buffer
    );

    return NoiseBuffer(m_bufSize, m_buffer);
}
bool Generator::prepare(const size_t size) {
    if (size == 0) return false;
    if (!rimpl.m_kernelAdapter) return false;
    prepareBuffer(size);
    return true;
}
// 2D
NoiseBuffer Generator::getNoise(const Range& x, const Range& y) {
    if (m_noise && !prepare(x.size * y.size)) return NoiseBuffer(0, nullptr);

    void (KernelAdapter::*nf) (Snapshot, size_t, size_t, float, float, float, float, float*) = nullptr;
    switch(m_noise->getNoiseType()) {
    case NoiseType::Cellular:
        if (m_noise->getCellularReturnType() != CellularReturnType::NoiseLookup) {
            nf = KernelAdapter::GEN_Cellular2;
        } else {
            std::vector<Snapshot> chain = rimpl.buildSnapshotChain();

            rimpl.m_kernelAdapter->GEN_Lookup_Cellular2(
                chain.data(), chain.size(),

                x.size, y.size,
                x.step, y.step,
                x.offset, y.offset,

                m_buffer
            );

            return NoiseBuffer(m_bufSize, m_buffer);
        }
        break;
    case NoiseType::Perlin:
        nf = KernelAdapter::GEN_Perlin2;
        break;
    case NoiseType::PerlinFractal:
        nf = KernelAdapter::GEN_PerlinFractal2;
        break;
    case NoiseType::Simplex:
        nf = KernelAdapter::GEN_Simplex2;
        break;
    case NoiseType::SimplexFractal:
        nf = KernelAdapter::GEN_SimplexFractal2;
        break;
    case NoiseType::Value:
        nf = KernelAdapter::GEN_Value2;
        break;
    case NoiseType::ValueFractal:
        nf = KernelAdapter::GEN_ValueFractal2;
        break;
    case NoiseType::WhiteNoise:
        nf = KernelAdapter::GEN_WhiteNoise2;
        break;
    }
    return Get2D(m_buffer, m_bufSize, rimpl.m_kernelAdapter, rimpl.createSnapshot(m_noise), x, y, nf);
}

// 3D
NoiseBuffer Generator::getNoise(const Range& x, const Range& y, const Range& z) {
    if (m_noise && !prepare(x.size * y.size * z.size)) return NoiseBuffer(0, nullptr);

    void (KernelAdapter::*nf) (Snapshot, size_t, size_t, size_t, float, float, float, float, float, float, float*) = nullptr;
    switch(m_noise->getNoiseType()) {
    case NoiseType::Cellular:
        if (m_noise->getCellularReturnType() == CellularReturnType::NoiseLookup) {
            std::vector<Snapshot> chain = rimpl.buildSnapshotChain();

            rimpl.m_kernelAdapter->GEN_Lookup_Cellular3(
                chain.data(), chain.size(),

                x.size, y.size, z.size,
                x.step, y.step, z.step,
                x.offset, y.offset, z.offset,

                m_buffer
            );

            return NoiseBuffer(m_bufSize, m_buffer);
        } else {
            nf = KernelAdapter::GEN_Cellular3;
        }
        break;
    case NoiseType::Perlin:
        nf = KernelAdapter::GEN_Perlin3;
        break;
    case NoiseType::PerlinFractal:
        nf = KernelAdapter::GEN_PerlinFractal3;
        break;
    case NoiseType::Simplex:
        nf = KernelAdapter::GEN_Simplex3;
        break;
    case NoiseType::SimplexFractal:
        nf = KernelAdapter::GEN_SimplexFractal3;
        break;
    case NoiseType::Value:
        nf = KernelAdapter::GEN_Value3;
        break;
    case NoiseType::ValueFractal:
        nf = KernelAdapter::GEN_ValueFractal3;
        break;
    case NoiseType::WhiteNoise:
        nf = KernelAdapter::GEN_WhiteNoise3;
        break;
    }

    return Get3D(m_buffer, m_bufSize, rimpl.m_kernelAdapter, rimpl.createSnapshot(m_noise), x, y, z, nf);
}

// 4D
NoiseBuffer Generator::getNoise(const Range& x, const Range& y, const Range& z, const Range& w) {
    if (m_noise && !prepare(x.size * y.size * z.size)) return NoiseBuffer(0, nullptr);

    void (KernelAdapter::*nf) (Snapshot, size_t, size_t, size_t, size_t, float, float, float, float, float, float, float, float, float*) = nullptr;
    switch(m_noise->getNoiseType()) {
    case NoiseType::Simplex:
        nf = KernelAdapter::GEN_Simplex4;
        break;
    case NoiseType::WhiteNoise:
        nf = KernelAdapter::GEN_WhiteNoise4;
        break;
    default:
        return NoiseBuffer(0, nullptr);
    }

    return Get4D(m_buffer, m_bufSize, rimpl.m_kernelAdapter, rimpl.createSnapshot(m_noise), x, y, z, w, nf);
}

// Getters/Setters
void Generator::setNoise(Noise* noise) {
    m_noise = noise;
}
Noise* Generator::getNoise() const {
    return m_noise;
}

// Misc
void Generator::prepareDevice(const Device& device) {
    rimpl.m_kernelAdapter = new KernelAdapter(device);
}

void Generator::prepareBuffer(size_t size) {
    m_bufSize = size;
    m_buffer = new float[m_bufSize];
}

template<typename T>
RangeContainer<T>::RangeContainer(std::size_t size, T offset, T step) {
    this->size = size;
    this->offset = offset;
    this->step = step;
}
template<typename T>
RangeContainer<T>& RangeContainer<T>::operator= (RangeContainer& r) {
    this->size = r.size;
    this->offset = r.offset;
    this->step = r.step;

    return *this;
}
template class RangeContainer<float>;
template class RangeContainer<int>;
