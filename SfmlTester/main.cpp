// main.cpp
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

#include <SFML/Graphics.hpp>
#include <windows.h>

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <algorithm>

#include"resources.h"
#include"res_wrk.h"
#include "CLNoise/CLNoise.h"
#include "Adapter/Adapter.hpp"

#define DSFML_Quiet_Destructors
#define SFML_Quiet_Destructors

using namespace std;

sf::RenderWindow app(sf::VideoMode(600, 600), "Noise window", sf::Style::Titlebar | sf::Style::Close);
size_t noise_type = 0;
const size_t noise_type_count = /*11*/10;

void change(int& off_z, Noise& fn, int& gain) {
    off_z++;
    gain++;
    //fn.SetPeturbLacunarity(gain / 250.0f);
    //fn.SetPeturbGain(gain / 250.0f);
    //fn.SetPerturbAmplitude(gain / 10.0f);
}

void noise_setup(Noise& n1, Noise& n2, Perturb& pt, Fractal& f1, Fractal& f2) {
    switch (noise_type) {
    case 0: /// Simplex
        n1.setFrequency(0.005);
        n1.setNoiseType(NoiseType::SimplexFractal);
        f1.setOctaves(5);
        pt.setPerturbType(PerturbType::None);
        n1.setFractalType(FractalType::FBM);
        break;
    case 1: /// Cellular - Lookup
        n2.setNoiseType(NoiseType::Simplex);
        n2.setFrequency(0.1);

        n1.setNoiseType(NoiseType::Cellular);
        n1.setCellularJitter(1);
        n1.setCellularNoiseLookup(&n2);
        n1.setCellularReturnType(CellularReturnType::NoiseLookup);
        n1.setCellularDistanceFunction(CellularDistanceFunction::Natural);
        n1.setFrequency(0.02);

        pt.setPerturbType(PerturbType::Fractal);
        pt.setFrequency(0.05);
        pt.setAmplitude(20);

        f2.setOctaves(5);
        f2.setGain(0.5);
        f2.setLacunarity(2);
        break;
    case 2: /// Cellular - Distance2Add
        n1.setFrequency(0.02);
        n1.setCellularJitter(1);
        n1.setNoiseType(NoiseType::Cellular);
        n1.setCellularDistanceFunction(CellularDistanceFunction::Natural);
        n1.setCellularReturnType(CellularReturnType::Distance2Add);
        pt.setPerturbType(PerturbType::None);
        n1.setCellularDistance2Indices(0, 1);
        break;
    case 3: /// Value
        n1.setFrequency(0.05);
        n1.setNoiseType(NoiseType::Value);
        pt.setPerturbType(PerturbType::None);
        break;
    case 4: /// WhiteNoise
        n1.setNoiseType(NoiseType::WhiteNoise);
        pt.setPerturbType(PerturbType::None);
        break;
    case 5: /// Colored
        n1.setFrequency(0.005);
        n1.setNoiseType(NoiseType::SimplexFractal);
        f1.setOctaves(3);
        pt.setPerturbType(PerturbType::None);
        break;
    case 6: /// simple Perlin
        n1.setFrequency(0.025);
        n1.setNoiseType(NoiseType::Perlin);
        pt.setPerturbType(PerturbType::None);
        break;
    case 7: /// simple Simplex
        n1.setFrequency(0.02);
        n1.setNoiseType(NoiseType::Simplex);
        pt.setPerturbType(PerturbType::Fractal);
        //pt.setAmplitude(45);
        pt.setAmplitude(1100);
        pt.setFrequency(0.015);

        f2.setOctaves(10);
        f2.setLacunarity(1.0);
        f2.setGain(1.2);
        break;
    case 8: /// simple Cellular
        n1.setFrequency(0.02);
        n1.setCellularJitter(0.5);
        n1.setNoiseType(NoiseType::Cellular);
        n1.setCellularDistanceFunction(CellularDistanceFunction::Euclidean);
        n1.setCellularReturnType(CellularReturnType::CellValue);
        pt.setPerturbType(PerturbType::None);
        break;
    case 9:
        n1.setFrequency(0.01);
        n1.setCellularJitter(0.9);
        n1.setNoiseType(NoiseType::Cellular);
        n1.setCellularDistanceFunction(CellularDistanceFunction::Natural);
        n1.setCellularDistance2Indices(2, 3);
        n1.setCellularReturnType(CellularReturnType::Distance2Sub);
        pt.setPerturbType(PerturbType::None);
        break;
    }
}
string nt_to_string() {
    switch (noise_type) {
    case 0: /// Simplex
        return "Simplex Fractal\n(5 octaves, FBM)";
    case 1: /// Cellular - Lookup
        return "Cellular with lookup and fractal perturb\n(siplex noise, 5 perturb octaves)";
    case 2: /// Cellular - Distance2Add
        return "Cellular with 2 distances\n(Addition, 2 smallest distances)";
    case 3: /// Value
        return "Value noise";
    case 4: /// WhiteNoise
        return "White noise";
    case 5: /// Colored
        return "3 Simplex fratals\n(each 5 octaves, FBM)";
    case 6: /// simple Perlin
        return "Perlin noise";
    case 7: /// simple Simplex
        return "Simplex noise with fractal perturb\n(10 octaves, high amplitude and gain)";
    case 8: /// simple Cellular
        return "Cellular noise\n(Voronoi diagram, 0.5 jitter)";
    case 9: /// Cellular - Distance2Sub
        return "Cellular noise\n(Subtraction, 2 largest distances)";
    }
}

void draw_text(sf::Text& txt, float shift, const sf::Color& color) {
    txt.setColor(sf::Color::Black);
    txt.move(-shift, -shift);
    app.draw(txt);
    txt.move(2*shift, 2*shift);
    app.draw(txt);
    txt.move(0, -shift);
    app.draw(txt);
    txt.move(-2*shift, 0);
    app.draw(txt);
    txt.setColor(color);
    txt.move(shift, 0);
    app.draw(txt);
}

int main() {
    /// Load resources
    Res Res_app_image(APP_IMAGE, RES_FILE);
    Res Res_arial(ARIAL, RES_FILE);

    /// Create the main window
    sf::Font font;
    sf::Image app_image;

    if (!font.loadFromMemory(Res_arial.pointer, Res_arial.size))
        return EXIT_FAILURE;
    if (!app_image.loadFromMemory(Res_app_image.pointer, Res_app_image.size))
        return EXIT_FAILURE;

    app.setIcon(app_image.getSize().x, app_image.getSize().y, app_image.getPixelsPtr());

    /// Create sprite to draw
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Image noise_image;
    sf::Text fps;
    sf::Text ntt;
    bool show_text = true;

    fps.setColor(sf::Color::Red);
    fps.setFont(font);
    fps.setPosition(0, 0);
    fps.setOrigin(0, 0);

    ntt.setColor(sf::Color::Red);
    ntt.setScale(0.8, 0.8);
    ntt.setFont(font);
    ntt.setPosition(0, 50);
    ntt.setOrigin(0, 0);
    ntt.setString(nt_to_string());
    ntt.setStyle(sf::Text::Bold);

    size_t size_x = app.getSize().x * 1;
    size_t size_y = app.getSize().y * 1;
    //texture.setSmooth(true);

    sprite.setOrigin(0, 0);
    sprite.setPosition(0, 0);


    vector<Device> devices = Device::getDevices();
    for (size_t i = 0; i < devices.size(); i++) {
        Device::Info info = devices[i].getInfo();
        cout << info.toString();
        cout << "\n\n";
    }

    Device* gpu = nullptr;
    for (size_t i = 0; i < devices.size(); i++)
        if (devices[i].getInfo().type == DeviceType::GPU)
            gpu = &devices[i];

    if (gpu == nullptr) {
        gpu = &devices[0];
    }
    Generator g(*gpu);
    Adapter adptr(*gpu);

    Noise n1;
    Noise n2;
    Perturb pt;
    Fractal f1;
    Fractal f2;

    g.setNoise(&n1);
    n1.setPerturb(&pt);
    n1.setFractal(&f1);
    pt.setFractal(&f2);

    noise_setup(n1, n2, pt, f1, f2);

    /// Start the window loop
    int off_z = 0, gain = 0;
    sf::Clock clock;
    while (app.isOpen()) {
        /// Process events
        sf::Event event;
        while (app.pollEvent(event)) {
            /// Close window : exit
            if (event.type == sf::Event::Closed) {
                app.close();
            }
            /// Window resized : resize
            if (event.type == sf::Event::Resized) {
                app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y))); /// Resize view for window "app"
            }
            /// Mouse button clicked : change noise
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    noise_type++;
                    if (noise_type == noise_type_count) noise_type = 0;
                    noise_setup(n1, n2, pt, f1, f2);
                    ntt.setString(nt_to_string());
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (noise_type == 0) noise_type = noise_type_count - 1;
                    else noise_type--;
                    noise_setup(n1, n2, pt, f1, f2);
                    ntt.setString(nt_to_string());
                }
            }
            /// Key pressed : hide text
            if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    show_text^= true;
            }
        }
        if (!app.isOpen()) break;

        /// Clear screen
        app.clear();

        /// Draw the sprite
        sf::Color* pixels = nullptr;
        if (noise_type != 5) {
            NoiseBuffer b = g.getNoise(
                                  Range(size_x, 0, 1),
                                  Range(size_y, 0, 1),
                                  Range(1, off_z, 1)
                              );

            switch (noise_type) {
            case 2: /// Cellular - Distance2Add
                pixels = adptr.to_shade(b.data, size_x * size_y * 1, 0, 7);
                break;
            case 9: /// Cellular - Distance2Sub
                pixels = adptr.to_shade(b.data, size_x * size_y * 1, 0, 4);
                break;
            default:
                pixels = adptr.to_shade(b.data, size_x * size_y * 1, -1, 1);
                break;
            }
        } else {
            n1.setSeed(1);
            NoiseBuffer b1 = g.getNoise(
                                   Range(size_x, 0, 1),
                                   Range(size_y, 0, 1),
                                   Range(1, off_z, 1)
                               );
            n1.setSeed(100);
            NoiseBuffer b2 = g.getNoise(
                                   Range(size_x, 0, 1),
                                   Range(size_y, 0, 1),
                                   Range(1, off_z, 1)
                               );
            n1.setSeed(1337);
            NoiseBuffer b3 = g.getNoise(
                                   Range(size_x, 0, 1),
                                   Range(size_y, 0, 1),
                                   Range(1, off_z, 1)
                               );

            pixels = adptr.to_color(b1.data, b2.data, b3.data, size_x * size_y * 1, -1, 1);\
        }
        change(off_z, n1, gain);

        noise_image.create(size_x, size_y, (sf::Uint8*)(pixels));
        texture.loadFromImage(noise_image, sf::IntRect(0, 0, size_x, size_y));
        sprite.setTexture(texture);
        sprite.setScale(
            app.getSize().x / float(texture.getSize().x),
            app.getSize().y / float(texture.getSize().y)
        );

        app.draw(sprite);
        if (pixels) delete[] pixels;
        if (show_text) {
            draw_text(fps, 1, sf::Color::Red);
            draw_text(ntt, 1, sf::Color::Yellow);
        }

        /// Update the window
        app.display();

        double elps_tm = clock.getElapsedTime().asMicroseconds();
        double time_left = 16000 - elps_tm;
        if (time_left > 0) sf::sleep(sf::microseconds(time_left));
        fps.setString("FPS: " + to_string(1000 / clock.getElapsedTime().asMilliseconds()));
        clock.restart();
    }

    return EXIT_SUCCESS;
}
