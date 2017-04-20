#include "DeviceManager.hpp"

#include <CL/cl.hpp>

using namespace std;

Device::Device(void* cl_device_ptr) {
    this->cl_device_ptr = cl_device_ptr;
    cl::Device& dev = *((cl::Device*)cl_device_ptr);

    auto type = dev.getInfo<CL_DEVICE_TYPE>();
    switch(type) {
    case CL_DEVICE_TYPE_DEFAULT:
        device_info.type = Info::DeviceType::Default;
        break;
    case CL_DEVICE_TYPE_CPU:
        device_info.type = Info::DeviceType::CPU;
        break;
    case CL_DEVICE_TYPE_GPU:
        device_info.type = Info::DeviceType::GPU;
        break;
    case CL_DEVICE_TYPE_ACCELERATOR:
        device_info.type = Info::DeviceType::Accelerator;
        break;
    case CL_DEVICE_TYPE_CUSTOM:
        device_info.type = Info::DeviceType::Custom;
        break;
    default:
        break;
    }
    device_info.name = dev.getInfo<CL_DEVICE_NAME>();
    device_info.vendor = dev.getInfo<CL_DEVICE_VENDOR>();
    device_info.version = dev.getInfo<CL_DEVICE_VERSION>();
}

Device::Info Device::getInfo() {
    return device_info;
}
void* Device::getDevicePtr() {
    return cl_device_ptr;
}

vector<Device> get_devices() {
    vector<Device> devices;

    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Device my_gpu;

    for (size_t i = 0; i < platforms.size(); i++) {
        auto pl = platforms[i];
        vector<cl::Device>* dev = new vector<cl::Device>();
        pl.getDevices(CL_DEVICE_TYPE_ALL, dev);

        for (size_t j = 0; j < dev->size(); j++) {
            devices.push_back(
                Device( (void*)&(dev->at(i)) )
            );
        }
    }

    return devices;
}

string Device::Info::toString() {
    string res;

    res += "NAME: " + this->name + "\nVENDOR: " + this->vendor + "\nTYPE: ";
    switch (this->type) {
    case Device::Info::Default:
        res += "Default";
        break;
    case Device::Info::CPU:
        res += "CPU";
        break;
    case Device::Info::GPU:
        res += "GPU";
        break;
    case Device::Info::Accelerator:
        res += "Accelerator";
        break;
    case Device::Info::Custom:
        res += "Custom";
        break;
    }
    res += "\nVERSION: " + this->version + "\n";

    return res;
}
