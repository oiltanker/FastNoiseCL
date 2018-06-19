// Adapter.cpp
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

#include "Adapter.hpp"

#include <CL/cl.hpp>
#include <vector>
#include <assert.h>

#include <string>

using namespace std;

const string src =
#include "to_image.cl"
    ;
#define KERNEL_COUNT 2
const char* kernel_nms[KERNEL_COUNT] = {
    "GEN_shade",
    "GEN_color"
};

//Initialize
class Adapter::impl {
public:
    cl::CommandQueue cmd_queue;
    cl::Context context;
    cl::Kernel* kernels = nullptr;

    impl() {}
    ~impl() {
        //if (kernels != nullptr) delete[] kernels;
    }
};

Adapter::Adapter(Device& dev) : pimpl( new impl) {
    cl::Device& device = *(cl::Device*)dev.getDevicePtr();

    assert(&device != nullptr);
    pimpl->context = cl::Context(device);
    pimpl->cmd_queue = cl::CommandQueue(pimpl->context, device);

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
sf::Color* Adapter::to_shade(float* noise, size_t msize, float nmin, float nmax) {
    //Configure stuff
    cl_int err;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[0]);

    //Create buffers
    cl::Buffer buf_noise(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(float) * msize, noise, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(sf::Color) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, nmin);
    kernel.setArg(1, nmax);
    kernel.setArg(2, buf_noise);
    kernel.setArg(3, buf_result);

    //Execute task
    sf::Color* result = new sf::Color[msize];
    err = pimpl->cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = pimpl->cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(sf::Color) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}

sf::Color* Adapter::to_color(float* noise1, float* noise2, float* noise3, size_t msize, float nmin, float nmax) {
    //Configure stuff
    cl_int err;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[1]);

    //Create buffers
    cl::Buffer buf_noise1(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(float) * msize, noise1, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_noise2(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(float) * msize, noise2, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_noise3(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(float) * msize, noise3, &err);
    assert(err == CL_SUCCESS);

    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(sf::Color) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, nmin);
    kernel.setArg(1, nmax);
    kernel.setArg(2, buf_noise1);
    kernel.setArg(3, buf_noise2);
    kernel.setArg(4, buf_noise3);
    kernel.setArg(5, buf_result);

    //Execute task
    sf::Color* result = new sf::Color[msize];
    err = pimpl->cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = pimpl->cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(sf::Color) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}

