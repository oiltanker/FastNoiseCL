#ifndef DEVICE_MANAGER
#define DEVICE_MANAGER

#include <string>
#include <vector>

class Device {
public:
    struct Info{
        enum DeviceType {
            Default,
            CPU,
            GPU,
            Accelerator,
            Custom
        };

        DeviceType type;
        std::string name;
        std::string vendor;
        std::string version;

        std::string toString();
    };

    Device(void* cl_device_ptr);
    Info getInfo();
    void* getDevicePtr();
private:
    void* cl_device_ptr;
    Info device_info;
};

std::vector<Device> get_devices();

#endif // DEVICE_MANAGER
