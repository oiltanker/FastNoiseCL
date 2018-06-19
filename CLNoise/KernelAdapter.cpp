// KernelAdapter.cpp
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

#include "KernelAdapter.h"

#include <CL/cl.hpp>
#include <vector>
#include <assert.h>
#include <map>

#include <string>

using namespace std;

const char *getErrorString(cl_int error) {
    switch(error) {
    // run-time and JIT compiler errors
    case 0:
        return "CL_SUCCESS";
    case -1:
        return "CL_DEVICE_NOT_FOUND";
    case -2:
        return "CL_DEVICE_NOT_AVAILABLE";
    case -3:
        return "CL_COMPILER_NOT_AVAILABLE";
    case -4:
        return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5:
        return "CL_OUT_OF_RESOURCES";
    case -6:
        return "CL_OUT_OF_HOST_MEMORY";
    case -7:
        return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8:
        return "CL_MEM_COPY_OVERLAP";
    case -9:
        return "CL_IMAGE_FORMAT_MISMATCH";
    case -10:
        return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11:
        return "CL_BUILD_PROGRAM_FAILURE";
    case -12:
        return "CL_MAP_FAILURE";
    case -13:
        return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14:
        return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15:
        return "CL_COMPILE_PROGRAM_FAILURE";
    case -16:
        return "CL_LINKER_NOT_AVAILABLE";
    case -17:
        return "CL_LINK_PROGRAM_FAILURE";
    case -18:
        return "CL_DEVICE_PARTITION_FAILED";
    case -19:
        return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // compile-time errors
    case -30:
        return "CL_INVALID_VALUE";
    case -31:
        return "CL_INVALID_DEVICE_TYPE";
    case -32:
        return "CL_INVALID_PLATFORM";
    case -33:
        return "CL_INVALID_DEVICE";
    case -34:
        return "CL_INVALID_CONTEXT";
    case -35:
        return "CL_INVALID_QUEUE_PROPERTIES";
    case -36:
        return "CL_INVALID_COMMAND_QUEUE";
    case -37:
        return "CL_INVALID_HOST_PTR";
    case -38:
        return "CL_INVALID_MEM_OBJECT";
    case -39:
        return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40:
        return "CL_INVALID_IMAGE_SIZE";
    case -41:
        return "CL_INVALID_SAMPLER";
    case -42:
        return "CL_INVALID_BINARY";
    case -43:
        return "CL_INVALID_BUILD_OPTIONS";
    case -44:
        return "CL_INVALID_PROGRAM";
    case -45:
        return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46:
        return "CL_INVALID_KERNEL_NAME";
    case -47:
        return "CL_INVALID_KERNEL_DEFINITION";
    case -48:
        return "CL_INVALID_KERNEL";
    case -49:
        return "CL_INVALID_ARG_INDEX";
    case -50:
        return "CL_INVALID_ARG_VALUE";
    case -51:
        return "CL_INVALID_ARG_SIZE";
    case -52:
        return "CL_INVALID_KERNEL_ARGS";
    case -53:
        return "CL_INVALID_WORK_DIMENSION";
    case -54:
        return "CL_INVALID_WORK_GROUP_SIZE";
    case -55:
        return "CL_INVALID_WORK_ITEM_SIZE";
    case -56:
        return "CL_INVALID_GLOBAL_OFFSET";
    case -57:
        return "CL_INVALID_EVENT_WAIT_LIST";
    case -58:
        return "CL_INVALID_EVENT";
    case -59:
        return "CL_INVALID_OPERATION";
    case -60:
        return "CL_INVALID_GL_OBJECT";
    case -61:
        return "CL_INVALID_BUFFER_SIZE";
    case -62:
        return "CL_INVALID_MIP_LEVEL";
    case -63:
        return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64:
        return "CL_INVALID_PROPERTY";
    case -65:
        return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66:
        return "CL_INVALID_COMPILER_OPTIONS";
    case -67:
        return "CL_INVALID_LINKER_OPTIONS";
    case -68:
        return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // extension errors
    case -1000:
        return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001:
        return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002:
        return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003:
        return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004:
        return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005:
        return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default:
        return "Unknown OpenCL error";
    }
}

const string src =
#include "Noise.cl"
    ;
#define KERNEL_COUNT 20
const char* kernel_names[KERNEL_COUNT] = {
    "GEN_Value2",
    "GEN_ValueFractal2",
    "GEN_Perlin2",
    "GEN_PerlinFractal2",
    "GEN_Simplex2",
    "GEN_SimplexFractal2",
    "GEN_Cellular2",
    "GEN_WhiteNoise2",
    "GEN_Value3",
    "GEN_ValueFractal3",
    "GEN_Perlin3",
    "GEN_PerlinFractal3",
    "GEN_Simplex3",
    "GEN_SimplexFractal3",
    "GEN_Cellular3",
    "GEN_WhiteNoise3",
    "GEN_Simplex4",
    "GEN_WhiteNoise4",
    "GEN_Lookup_Cellular2",
    "GEN_Lookup_Cellular3"
};
enum Kernel {
    VALUE2 = 0,
    VALUEFRACTAL2 = 1,
    PERLIN2 = 2,
    PERLINFRACTAL2 = 3,
    SIMPLEX2 = 4,
    SIMPLEXFRACTAL2 = 5,
    CELLULAR2 = 6,
    WHITENOISE2 = 7,
    VALUE3 = 8,
    VALUEFRACTAL3 = 9,
    PERLIN3 = 10,
    PERLINFRACTAL3 = 11,
    SIMPLEX3 = 12,
    SIMPLEXFRACTAL3 = 13,
    CELLULAR3 = 14,
    WHITENOISE3 = 15,
    SIMPLEX4 = 16,
    WHITENOISE4 = 17,
    LOOKUP_CELLULAR2 = 18,
    LOOKUP_CELLULAR3 = 19,
};

//Initialize
class KernelAdapter::impl {
public:
    cl::Context m_context;
    cl::Kernel* m_kernels = nullptr;
    cl::CommandQueue m_cmdQueue;

    impl() {}
    ~impl() {
        if (m_kernels != nullptr) delete[] m_kernels;
    }
};

class KernelAdapter::simpl {
public:
    impl& getImpl(void* dev) {
        auto i = m_impls.find(dev);
        if (i == m_impls.end()) {
            m_impls[dev] = impl();
        }

        return m_impls[dev];
    }
private:
    map<void*, impl> m_impls;
};
KernelAdapter::simpl& KernelAdapter::rsimpl = *(new simpl);

KernelAdapter::KernelAdapter(const Device& dev) : rimpl(rsimpl.getImpl(dev.getDevicePtr())) {
    if (rimpl.m_kernels) return;

    cl::Device& device = *(cl::Device*)dev.getDevicePtr();

    assert(&device != nullptr);
    rimpl.m_context = cl::Context(device);
    rimpl.m_cmdQueue = cl::CommandQueue(rimpl.m_context, device);

    cl::Program::Sources source(1, make_pair(src.c_str(), src.length() + 1));
    cl::Program program(rimpl.m_context, source);
    auto err = program.build("-cl-std=CL1.2");
    assert(err == CL_SUCCESS);

    rimpl.m_kernels = new cl::Kernel[KERNEL_COUNT];
    for (size_t i = 0; i < KERNEL_COUNT; i++) {
        rimpl.m_kernels[i] = cl::Kernel(program, kernel_names[i], &err);
        assert(err == CL_SUCCESS);
    }
}
KernelAdapter::~KernelAdapter() {}

//Kernels

template <typename T>
void exec_kernel_2D(
    cl::Kernel& kernel,           // |
    cl::Context& context,         // | IN : KernelAdapter::impl
    cl::CommandQueue& cmdQueue,   // |

    Snapshot param,                // IN : class members

    size_t sizeX, size_t sizeY,   // |
    T scaleX, T scaleY,           // | IN : Parameters
    T offsetX, T offsetY,         // |

    float* result
) {
    //Configure stuff
    cl_int err;
    size_t msize = sizeX * sizeY;

    //Create buffers
    cl::Buffer buf_result(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(Snapshot), &param);
    kernel.setArg(1, sizeof(size_t), &sizeX);
    kernel.setArg(2, sizeof(size_t), &sizeY);
    kernel.setArg(3, sizeof(T), &scaleX);
    kernel.setArg(4, sizeof(T), &scaleY);
    kernel.setArg(5, sizeof(T), &offsetX);
    kernel.setArg(6, sizeof(T), &offsetY);
    kernel.setArg(7, buf_result);

    //Execute task
    err = cmdQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmdQueue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);
}
template <typename T>
void exec_kernel_3D(
    cl::Kernel& kernel,                          // |
    cl::Context& context,                        // | IN : KernelAdapter::impl
    cl::CommandQueue& cmdQueue,                  // |

    Snapshot param,                              // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    T scaleX, T scaleY, T scaleZ,                // | IN : Parameters
    T offsetX, T offsetY, T offsetZ,             // |

    float* result
) {
    //Configure stuff
    cl_int err;
    size_t msize = sizeX * sizeY * sizeZ;

    //Create buffers
    cl::Buffer buf_result(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(Snapshot), &param);
    kernel.setArg(1, sizeof(size_t), &sizeX);
    kernel.setArg(2, sizeof(size_t), &sizeY);
    kernel.setArg(3, sizeof(size_t), &sizeZ);
    kernel.setArg(4, sizeof(T), &scaleX);
    kernel.setArg(5, sizeof(T), &scaleY);
    kernel.setArg(6, sizeof(T), &scaleZ);
    kernel.setArg(7, sizeof(T), &offsetX);
    kernel.setArg(8, sizeof(T), &offsetY);
    kernel.setArg(9, sizeof(T), &offsetZ);
    kernel.setArg(10, buf_result);

    //Execute task
    err = cmdQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmdQueue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);
}
template <typename T>
void exec_kernel_4D(
    cl::Kernel& kernel,                                         // |
    cl::Context& context,                                       // | IN : KernelAdapter::impl
    cl::CommandQueue& cmdQueue,                                 // |

    Snapshot param,                                             // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ, size_t sizeW,     // |
    T scaleX, T scaleY, T scaleZ, T scaleW,                     // | IN : Parameters
    T offsetX, T offsetY, T offsetZ, T offsetW,                 // |

    float* result
) {
    //Configure stuff
    cl_int err;
    size_t msize = sizeX * sizeY * sizeZ * sizeW;

    //Create buffers
    cl::Buffer buf_result(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(Snapshot), &param);
    kernel.setArg(1, sizeof(size_t), &sizeX);
    kernel.setArg(2, sizeof(size_t), &sizeY);
    kernel.setArg(3, sizeof(size_t), &sizeZ);
    kernel.setArg(4, sizeof(size_t), &sizeW);
    kernel.setArg(5, sizeof(float), &scaleX);
    kernel.setArg(6, sizeof(float), &scaleY);
    kernel.setArg(7, sizeof(float), &scaleZ);
    kernel.setArg(8, sizeof(float), &scaleW);
    kernel.setArg(9, sizeof(float), &offsetX);
    kernel.setArg(10, sizeof(float), &offsetY);
    kernel.setArg(11, sizeof(float), &offsetZ);
    kernel.setArg(12, sizeof(float), &offsetW);
    kernel.setArg(13, buf_result);

    //Execute task
    err = cmdQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmdQueue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);
}

//2D
void KernelAdapter::GEN_Value2(
    Snapshot param,               // IN : class members

    size_t sizeX, size_t sizeY,   // |
    float scaleX, float scaleY,   // | IN : Parameters
    float offsetX, float offsetY, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[VALUE2]);
    exec_kernel_2D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, scaleX, scaleY, offsetX, offsetY, result);
}
void KernelAdapter::GEN_ValueFractal2(
    Snapshot param,               // IN : class members

    size_t sizeX, size_t sizeY,   // |
    float scaleX, float scaleY,   // | IN : Parameters
    float offsetX, float offsetY, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[VALUEFRACTAL2]);
    exec_kernel_2D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, scaleX, scaleY, offsetX, offsetY, result);
}
void KernelAdapter::GEN_Perlin2(
    Snapshot param,               // IN : class members

    size_t sizeX, size_t sizeY,   // |
    float scaleX, float scaleY,   // | IN : Parameters
    float offsetX, float offsetY, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[PERLIN2]);
    exec_kernel_2D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, scaleX, scaleY, offsetX, offsetY, result);
}
void KernelAdapter::GEN_PerlinFractal2(
    Snapshot param,               // IN : class members

    size_t sizeX, size_t sizeY,   // |
    float scaleX, float scaleY,   // | IN : Parameters
    float offsetX, float offsetY, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[PERLINFRACTAL2]);
    exec_kernel_2D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, scaleX, scaleY, offsetX, offsetY, result);
}
void KernelAdapter::GEN_Simplex2(
    Snapshot param,               // IN : class members

    size_t sizeX, size_t sizeY,   // |
    float scaleX, float scaleY,   // | IN : Parameters
    float offsetX, float offsetY, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[SIMPLEX2]);
    exec_kernel_2D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, scaleX, scaleY, offsetX, offsetY, result);
}
void KernelAdapter::GEN_SimplexFractal2(
    Snapshot param,               // IN : class members

    size_t sizeX, size_t sizeY,   // |
    float scaleX, float scaleY,   // | IN : Parameters
    float offsetX, float offsetY, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[SIMPLEXFRACTAL2]);
    exec_kernel_2D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, scaleX, scaleY, offsetX, offsetY, result);
}
void KernelAdapter::GEN_Cellular2(
    Snapshot param,               // IN : class members

    size_t sizeX, size_t sizeY,   // |
    float scaleX, float scaleY,   // | IN : Parameters
    float offsetX, float offsetY, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[CELLULAR2]);
    exec_kernel_2D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, scaleX, scaleY, offsetX, offsetY, result);
}
void KernelAdapter::GEN_WhiteNoise2(
    Snapshot param,               // IN : class members

    size_t sizeX, size_t sizeY,   // |
    float scaleX, float scaleY,   // | IN : Parameters
    float offsetX, float offsetY, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[WHITENOISE2]);
    exec_kernel_2D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, scaleX, scaleY, offsetX, offsetY, result);
}

//3D
void KernelAdapter::GEN_Value3(
    Snapshot param,                              // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[VALUE3]);
    exec_kernel_3D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, offsetX, offsetY, offsetZ, result);
}
void KernelAdapter::GEN_ValueFractal3(
    Snapshot param,                              // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[VALUEFRACTAL3]);
    exec_kernel_3D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, offsetX, offsetY, offsetZ, result);
}
void KernelAdapter::GEN_Perlin3(
    Snapshot param,                              // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[PERLIN3]);
    exec_kernel_3D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, offsetX, offsetY, offsetZ, result);
}
void KernelAdapter::GEN_PerlinFractal3(
    Snapshot param,                              // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[PERLINFRACTAL3]);
    exec_kernel_3D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, offsetX, offsetY, offsetZ, result);
}
void KernelAdapter::GEN_Simplex3(
    Snapshot param,                              // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[SIMPLEX3]);
    exec_kernel_3D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, offsetX, offsetY, offsetZ, result);
}
void KernelAdapter::GEN_SimplexFractal3(
    Snapshot param,                              // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[SIMPLEXFRACTAL3]);
    exec_kernel_3D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, offsetX, offsetY, offsetZ, result);
}
void KernelAdapter::GEN_Cellular3(
    Snapshot param,                              // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[CELLULAR3]);
    exec_kernel_3D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, offsetX, offsetY, offsetZ, result);
}
void KernelAdapter::GEN_WhiteNoise3(
    Snapshot param,                              // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[WHITENOISE3]);
    exec_kernel_3D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, scaleX, scaleY, scaleZ, offsetX, offsetY, offsetZ, result);
}

//4D
void KernelAdapter::GEN_Simplex4(
    Snapshot param,                                             // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ, size_t sizeW,     // |
    float scaleX, float scaleY, float scaleZ, float scaleW,     // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, float offsetW, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[SIMPLEX4]);
    exec_kernel_4D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, sizeW, scaleX, scaleY, scaleZ, scaleW, offsetX, offsetY, offsetZ, offsetW, result);
}
void KernelAdapter::GEN_WhiteNoise4(
    Snapshot param,                                             // IN : class members

    size_t sizeX, size_t sizeY, size_t sizeZ, size_t sizeW,     // |
    float scaleX, float scaleY, float scaleZ, float scaleW,     // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, float offsetW, // |

    float* result
) {
    cl::Kernel kernel(rimpl.m_kernels[WHITENOISE4]);
    exec_kernel_4D<float>(kernel, rimpl.m_context, rimpl.m_cmdQueue, param, sizeX, sizeY, sizeZ, sizeW, scaleX, scaleY, scaleZ, scaleW, offsetX, offsetY, offsetZ, offsetW, result);
}

//NoiseLookup
void KernelAdapter::GEN_Lookup_Cellular2(
    Snapshot* params, size_t size_p, // IN : members of all classes

    size_t sizeX, size_t sizeY,      // |
    float scaleX, float scaleY,      // | IN : Parameters
    float offsetX, float offsetY,    // |

    float* result
) {
    //Configure stuff
    cl_int err;
    size_t msize = sizeX * sizeY;

    //Get CL objects
    cl::Kernel kernel(rimpl.m_kernels[LOOKUP_CELLULAR2]);

    //Create buffers
    cl::Buffer buf_param(rimpl.m_context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(Snapshot) * size_p, params, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(rimpl.m_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, buf_param);
    kernel.setArg(1, sizeof(size_t), &size_p);
    kernel.setArg(2, sizeof(size_t), &sizeX);
    kernel.setArg(3, sizeof(size_t), &sizeY);
    kernel.setArg(4, sizeof(float), &scaleX);
    kernel.setArg(5, sizeof(float), &scaleY);
    kernel.setArg(6, sizeof(float), &offsetX);
    kernel.setArg(7, sizeof(float), &offsetY);
    kernel.setArg(8, buf_result);

    //Execute task
    err = rimpl.m_cmdQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = rimpl.m_cmdQueue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);
}
void KernelAdapter::GEN_Lookup_Cellular3(
    Snapshot* params, size_t size_p,             // IN : members of all classes

    size_t sizeX, size_t sizeY, size_t sizeZ,    // |
    float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
    float offsetX, float offsetY, float offsetZ, // |

    float* result
) {
    //Configure stuff
    cl_int err;
    size_t msize = sizeX * sizeY * sizeZ;

    //Get CL objects
    cl::Kernel kernel(rimpl.m_kernels[LOOKUP_CELLULAR3]);

    //Create buffers
    cl::Buffer buf_param(rimpl.m_context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(Snapshot) * size_p, params, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(rimpl.m_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, buf_param);
    kernel.setArg(1, sizeof(size_t), &size_p);
    kernel.setArg(2, sizeof(size_t), &sizeX);
    kernel.setArg(3, sizeof(size_t), &sizeY);
    kernel.setArg(4, sizeof(size_t), &sizeZ);
    kernel.setArg(5, sizeof(float), &scaleX);
    kernel.setArg(6, sizeof(float), &scaleY);
    kernel.setArg(7, sizeof(float), &scaleZ);
    kernel.setArg(8, sizeof(float), &offsetX);
    kernel.setArg(9, sizeof(float), &offsetY);
    kernel.setArg(10, sizeof(float), &offsetZ);
    kernel.setArg(11, buf_result);

    //Execute task
    err = rimpl.m_cmdQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = rimpl.m_cmdQueue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);
}
