// DeviceManager.cpp
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

#include "DeviceManager.h"

#include <CL/cl.hpp>
#include <cstring>

using namespace std;

vector<Device> collect_devices(vector<cl::Device>* dev) {
    vector<Device> devices;

    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for (size_t i = 0; i < platforms.size(); i++) {
        auto pl = platforms[i];

        pl.getDevices(CL_DEVICE_TYPE_ALL, dev);

        for (size_t j = 0; j < dev->size(); j++) {
            devices.push_back( Device((void*)&dev->at(i)) );
        }
    }

    return devices;
}

void* Device::m_devicePtrs = (void*) new vector<cl::Device>();
vector<Device> Device::m_devices = collect_devices((vector<cl::Device>*)m_devicePtrs);

Device::Device(void* pCLDevice) {
    m_pCLDevice = pCLDevice;
    cl::Device& dev = *(cl::Device*)m_pCLDevice;

    auto type = dev.getInfo<CL_DEVICE_TYPE>();
    switch(type) {
    case CL_DEVICE_TYPE_DEFAULT:
        m_deviceInfo.type = DeviceType::Default;
        break;
    case CL_DEVICE_TYPE_CPU:
        m_deviceInfo.type = DeviceType::CPU;
        break;
    case CL_DEVICE_TYPE_GPU:
        m_deviceInfo.type = DeviceType::GPU;
        break;
    case CL_DEVICE_TYPE_ACCELERATOR:
        m_deviceInfo.type = DeviceType::Accelerator;
        break;
    case CL_DEVICE_TYPE_CUSTOM:
        m_deviceInfo.type = DeviceType::Custom;
        break;
    default:
        break;
    }
    m_deviceInfo.name = dev.getInfo<CL_DEVICE_NAME>();
    m_deviceInfo.vendor = dev.getInfo<CL_DEVICE_VENDOR>();
    m_deviceInfo.version = dev.getInfo<CL_DEVICE_VERSION>();
}
Device::~Device() {}

const Device::Info& Device::getInfo() const {
    return m_deviceInfo;
}
void* Device::getDevicePtr() const {
    return m_pCLDevice;
}

const vector<Device>& Device::getDevices() {
    return m_devices;
}

string Device::Info::toString() const {
    string res;

    res += "NAME: " + this->name + "\nVENDOR: " + this->vendor + "\nTYPE: ";
    switch (this->type) {
    case DeviceType::Default:
        res += "Default";
        break;
    case DeviceType::CPU:
        res += "CPU";
        break;
    case DeviceType::GPU:
        res += "GPU";
        break;
    case DeviceType::Accelerator:
        res += "Accelerator";
        break;
    case DeviceType::Custom:
        res += "Custom";
        break;
    case DeviceType::None:
        res += "Not a device";
        break;
    }
    res += "\nVERSION: " + this->version + "\n";

    return res;
}
