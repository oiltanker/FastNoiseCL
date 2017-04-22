#ifndef ADAPTER
#define ADAPTER

#include <memory>
#include <SFML/Graphics/Color.hpp>
#include "../FastNoiseCL/DeviceManager.hpp"

typedef unsigned char uchar;

class Adapter {
public:
    //Initialize
    Adapter(Device& device);
    ~Adapter();

    sf::Color* to_shade(float* noise, size_t msize);

private:
    class impl;
    std::unique_ptr<impl> pimpl;
};

#endif // ADAPTER
