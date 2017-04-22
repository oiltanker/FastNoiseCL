#include "Adapter.hpp"

#include <CL/cl.hpp>
#include <vector>
#include <assert.h>

#include <string>

using namespace std;

const string src =
    #include "../to_image.cl"
;
#define KERNEL_COUNT 1
const char* kernel_nms[KERNEL_COUNT] = {
    "GEN_shade"
};

//Initialize
class Adapter::impl {
public:
    std::unique_ptr<cl::Device> device = nullptr;
    cl::Context context;
    cl::Kernel* kernels = nullptr;

    impl() {}
    ~impl() {
        if (kernels != nullptr) delete[] kernels;
    }
};

Adapter::Adapter(Device& dev) : pimpl( new impl) {

    assert(&dev != nullptr);
    pimpl->device = unique_ptr<cl::Device>((cl::Device*)(dev.getDevicePtr()));
    pimpl->context = cl::Context(*pimpl->device);

    cl::Program::Sources source(1, make_pair(src.c_str(), src.length() + 1));
    cl::Program program(pimpl->context, source);
    auto err = program.build("-cl-std=CL1.2");
    assert(err == CL_SUCCESS);

    pimpl->kernels = new cl::Kernel[KERNEL_COUNT];
    for (size_t i = 0; i < KERNEL_COUNT; i++) {
        pimpl->kernels[i] = cl::Kernel(program, kernel_nms[i], &err);
        assert(err == CL_SUCCESS);
    }
}
Adapter::~Adapter() {}

//Kernels
sf::Color* Adapter::to_shade(float* noise, size_t msize) {
    //Configure stuff
    cl_int err;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[0]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_noise(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(float) * msize, noise, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(sf::Color) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, buf_noise);
    kernel.setArg(1, buf_result);

    //Execute task
    sf::Color* result = new sf::Color[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(sf::Color) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
