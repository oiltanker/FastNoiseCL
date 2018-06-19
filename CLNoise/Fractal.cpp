#include "Fractal.h"

Fractal::Fractal() : Fractal(3, 2.0f, 0.5f) {}
Fractal::Fractal(const int octaves, const float lacunarity, const float gain) {
    m_octaves = octaves;
    m_lacunarity = lacunarity;
    m_gain = gain;
    CalculateBounding();
}

void Fractal::setOctaves(const int octaves) {
    m_octaves = octaves;
    CalculateBounding();
}
void Fractal::setLacunarity(const float lacunarity) {
    m_lacunarity = lacunarity;
}
void Fractal::setGain(const float gain) {
    m_gain = gain;
    CalculateBounding();
}

int Fractal::getOctaves() const {
    return m_octaves;
}
float Fractal::getLacunarity() const {
    return m_lacunarity;
}
float Fractal::getGain() const {
    return m_gain;
}
float Fractal::getBounding() const {
    return m_bounding;
}

void Fractal::CalculateBounding() {
    float amp = m_gain;
    float ampFractal = 1.0f;
    for (int i = 1; i < m_octaves; i++) {
        ampFractal += amp;
        amp *= m_gain;
    }
    m_bounding = 1.0f / ampFractal;
}
