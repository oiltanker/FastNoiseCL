// DeviceManager.h
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

#ifndef DEVICE_MANAGER
#define DEVICE_MANAGER

#include <string>
#include <vector>

enum class DeviceType {
    Default,
    CPU,
    GPU,
    Accelerator,
    Custom,
    None
};

//! \brief class that holds information about and OpenCl pointer to a device available for parallel computing
class Device {
public:
    //! \brief holds information about the device available for parallel computing
    class Info {
    public:
        DeviceType type;
        std::string name;
        std::string vendor;
        std::string version;

        std::string toString() const;
    };

    Device(void* pCLDevice);
    ~Device();

    const Info& getInfo() const ;
    void* getDevicePtr() const;

    //! \brief Get all the devices available for parallel computing
    static const std::vector<Device>& getDevices();
private:
    void* m_pCLDevice;
    Info m_deviceInfo;

    static void* m_devicePtrs;
    static std::vector<Device> m_devices;
};

#endif // DEVICE_MANAGER
