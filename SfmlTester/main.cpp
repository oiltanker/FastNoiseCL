#include <SFML/Graphics.hpp>
#include <windows.h>

#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

#include"resources.h"
#include"res_wrk.h"
#include "FastNoiseCL/FastNoiseCL.h"
#include "Adapter/Adapter.hpp"

#include <CL/cl.hpp>

#define DSFML_Quiet_Destructors
#define SFML_Quiet_Destructors

using namespace std;

sf::RenderWindow app(sf::VideoMode(400, 400), "SFML window", sf::Style::Titlebar | sf::Style::Close);

int main() {
    //Load resources
    Res Res_app_image(APP_IMAGE, RES_FILE);
    Res Res_arial(ARIAL, RES_FILE);

    // Create the main window
    sf::Font font;
    sf::Image app_image;

    if (!font.loadFromMemory(Res_arial.pointer, Res_arial.size))
        return EXIT_FAILURE;
    if (!app_image.loadFromMemory(Res_app_image.pointer, Res_app_image.size))
        return EXIT_FAILURE;

    app.setIcon(app_image.getSize().x, app_image.getSize().y, app_image.getPixelsPtr());

    //Create sprite to draw
    sf::Texture texture;
    sf::Sprite sprite;

    size_t size_x = app.getSize().x;
    size_t size_y = app.getSize().y;
    texture.loadFromImage(app_image, sf::IntRect(0, 0, size_x, size_y));
    texture.setSmooth(true);

    sprite.setTexture(texture);
    sprite.setOrigin(0, 0);
    sprite.setPosition(0, 0);
    sprite.setScale(float(size_x) / sprite.getTextureRect().width, float(size_y) / sprite.getTextureRect().height);

    sf::Image noise_image;

    vector<Device> devices = get_devices();
    Device::Info info = devices[0].getInfo();
    cout << info.toString();

    Adapter adptr(devices[0]);
    FastNoiseCL fn(devices[0]);
    fn.SetPeturbType(FastNoiseCL::PerturbType::Fractal);
    fn.SetPerturbAmp(10);

    FastNoiseCL fnl;
    fnl.SetNoiseType(FastNoiseCL::NoiseType::Simplex);
    fnl.SetFrequency(0.1);

    fn.SetCellularNoiseLookup(&fnl);
    fn.SetCellularReturnType(FastNoiseCL::CellularReturnType::NoiseLookup);
    fn.SetCellularDistanceFunction(FastNoiseCL::CellularDistanceFunction::Natural);
    fn.SetFrequency(0.04);
	// Start the game loop
	int off_z = 0;
	sf::Clock clock;
    while (app.isOpen()) {
        // Process events
        sf::Event event;
        while (app.pollEvent(event)) {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                app.close();
            }
            // Window resized : resize
            if (event.type == sf::Event::Resized) {
                app.setView(sf::View(sf::FloatRect(0, 0, app.getSize().x, app.getSize().y))); /// Resize view for window "app"
            }
        }
        if (!app.isOpen()) break;

        // Clear screen
        app.clear();

        // Draw the sprite
        float* noise = fn.GetCellular(
            FastNoiseCL::Range(size_x , 0, 1),
            FastNoiseCL::Range(size_y, 0, 1),
            FastNoiseCL::Range(1, off_z, 1)
        );
        off_z++;
        sf::Color* pixels = adptr.to_shade(noise, size_x * size_y * 1);

        noise_image.create(size_x, size_y, (sf::Uint8*)(pixels));
        texture.loadFromImage(noise_image, sf::IntRect(0, 0, size_x, size_y));
        app.draw(sprite);
        delete[] noise;
        delete[] pixels;

        // Update the window
        app.display();

        double elps_tm = clock.getElapsedTime().asMicroseconds();
        double time_left = 16000 - elps_tm;
        if (time_left > 0) sf::sleep(sf::microseconds(time_left));
        clock.restart();
    }

    return EXIT_SUCCESS;
}
