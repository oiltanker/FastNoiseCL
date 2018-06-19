#include "Perturb.h"

Perturb::Perturb(const int seed) {
    m_seed = seed;
    m_frequency = 0.01f;

    m_seed = seed;
    m_amplitude = 1.0f;
    m_type = PerturbType::None;
    m_smoothing = Smoothing::Quintic;

    m_fractal = nullptr;
}

void Perturb::setSeed(const int seed) {
    m_seed = seed;
}
int Perturb::getSeed() const {
    return m_seed;
}
void Perturb::setFrequency(const float frequency) {
    m_frequency = frequency;
}
void Perturb::setAmplitude(const float amplitude) {
    m_amplitude = amplitude;
}
void Perturb::setSmoothingFunction(const Smoothing smoothing) {
    m_smoothing = smoothing;
}
void Perturb::setPerturbType(const PerturbType type) {
    m_type = type;
}
void Perturb::setFractal(Fractal*  fractal) {
    m_fractal = fractal;
}


float Perturb::getFrequency() const {
    return m_frequency;
}
float Perturb::getAmplitude() const {
    return m_amplitude;
}
Smoothing Perturb::getSmoothingFunction() const {
    return m_smoothing;
}
PerturbType Perturb::getPerturbType() const {
    return m_type;
}
Fractal* Perturb::getFractal() const {
    return m_fractal;
}
