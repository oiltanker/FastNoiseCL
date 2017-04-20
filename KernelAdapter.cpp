#include "KernelAdapter.hpp"

#include <CL/cl.hpp>
#include <vector>
#include <assert.h>

#include<string>

using namespace std;

const char *getErrorString(cl_int error) {
    switch(error){
    // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
}

const string src =
    #include "FastNoise.cl"
;
#define KERNEL_COUNT 21
const char* kernel_names[KERNEL_COUNT] = {
    "GEN_Value2",
    "GEN_ValueFractal2",
    "GEN_Perlin2",
    "GEN_PerlinFractal2",
    "GEN_Simplex2",
    "GEN_SimplexFractal2",
    "GEN_Cellular2",
    "GEN_WhiteNoise2",
    "GEN_WhiteNoiseInt2",
    "GEN_Value3",
    "GEN_ValueFractal3",
    "GEN_Perlin3",
    "GEN_PerlinFractal3",
    "GEN_Simplex3",
    "GEN_SimplexFractal3",
    "GEN_Cellular3",
    "GEN_WhiteNoise3",
    "GEN_WhiteNoiseInt3",
    "GEN_Simplex4",
    "GEN_WhiteNoise4",
    "GEN_WhiteNoiseInt4",
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
    WHITENOISEINT2 = 8,
    VALUE3 = 9,
    VALUEFRACTAL3 = 10,
    PERLIN3 = 11,
    PERLINFRACTAL3 = 12,
    SIMPLEX3 = 13,
    SIMPLEXFRACTAL3 = 14,
    CELLULAR3 = 15,
    WHITENOISE3 = 16,
    WHITENOISEINT3 = 17,
    SIMPLEX4 = 18,
    WHITENOISE4 = 19,
    WHITENOISEINT4 = 20,
};

//Initialize
class KernelAdapter::impl {
public:
    std::unique_ptr<cl::Device> device = nullptr;
    cl::Context context;
    cl::Kernel* kernels = nullptr;

    impl() {}
    ~impl() {
        if (kernels != nullptr) delete[] kernels;
    }
};

KernelAdapter::KernelAdapter(Device& dev) : pimpl( new impl) {

    assert(&dev != nullptr);
    pimpl->device = unique_ptr<cl::Device>((cl::Device*)(dev.getDevicePtr()));
    pimpl->context = cl::Context(*pimpl->device);

    cl::Program::Sources source(1, make_pair(src.c_str(), src.length() + 1));
    cl::Program program(pimpl->context, source);
    auto err = program.build("-cl-std=CL1.2");
    assert(err == CL_SUCCESS);

    pimpl->kernels = new cl::Kernel[KERNEL_COUNT];
    for (size_t i = 0; i < KERNEL_COUNT; i++) {
        pimpl->kernels[i] = cl::Kernel(program, kernel_names[i], &err);
        assert(err == CL_SUCCESS);
    }
}
KernelAdapter::~KernelAdapter() {}

//Kernels
//2D
float* KernelAdapter::GEN_Value2(
    float m_frequency,              // |
    int m_interp,                   // | IN : class members
    cl_uchar* m_perm,               // |

    size_t size_x, size_t size_y,   // |
    float scale_x, float scale_y,   // | IN : Parameters
    float offset_x, float offset_y  // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[VALUE2]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_interp);
    kernel.setArg(2, buf_m_perm);
    kernel.setArg(3, sizeof(size_t), &size_x);
    kernel.setArg(4, sizeof(size_t), &size_y);
    kernel.setArg(5, sizeof(float), &scale_x);
    kernel.setArg(6, sizeof(float), &scale_y);
    kernel.setArg(7, sizeof(float), &offset_x);
    kernel.setArg(8, sizeof(float), &offset_y);
    kernel.setArg(9, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_ValueFractal2(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
    int m_interp,                                                              // | IN : class members
    cl_uchar* m_perm,                                                          // |

    size_t size_x, size_t size_y,                                              // |
    float scale_x, float scale_y,                                              // | IN : Parameters
    float offset_x, float offset_y                                             // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[VALUEFRACTAL2]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(int), &m_fractalType);
    kernel.setArg(1, sizeof(float), &m_frequency);
    kernel.setArg(2, sizeof(float), &m_lacunarity);
    kernel.setArg(3, sizeof(float), &m_gain);
    kernel.setArg(4, sizeof(int), &m_octaves);
    kernel.setArg(5, sizeof(float), &m_fractalBounding);
    kernel.setArg(6, sizeof(int), &m_interp);
    kernel.setArg(7, buf_m_perm);
    kernel.setArg(8, sizeof(size_t), &size_x);
    kernel.setArg(9, sizeof(size_t), &size_y);
    kernel.setArg(10, sizeof(float), &scale_x);
    kernel.setArg(11, sizeof(float), &scale_y);
    kernel.setArg(12, sizeof(float), &offset_x);
    kernel.setArg(13, sizeof(float), &offset_y);
    kernel.setArg(14, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_Perlin2(
    float m_frequency,                                // |
    int m_interp,                                     // | IN : class members
    cl_uchar* m_perm, cl_uchar* m_perm12,             // |

    size_t size_x, size_t size_y,                     // |
    float scale_x, float scale_y,                     // | IN : Parameters
    float offset_x, float offset_y                    // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[PERLIN2]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_m_perm12(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm12, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_interp);
    kernel.setArg(2, buf_m_perm);
    kernel.setArg(3, buf_m_perm12);
    kernel.setArg(4, sizeof(size_t), &size_x);
    kernel.setArg(5, sizeof(size_t), &size_y);
    kernel.setArg(6, sizeof(float), &scale_x);
    kernel.setArg(7, sizeof(float), &scale_y);
    kernel.setArg(8, sizeof(float), &offset_x);
    kernel.setArg(9, sizeof(float), &offset_y);
    kernel.setArg(10, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_PerlinFractal2(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
    int m_interp,                                                              // | IN : class members
    cl_uchar* m_perm,  cl_uchar* m_perm12,                                     // |

    size_t size_x, size_t size_y,                                              // |
    float scale_x, float scale_y,                                              // | IN : Parameters
    float offset_x, float offset_y                                             // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[PERLINFRACTAL2]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_m_perm12(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm12, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_fractalType);
    kernel.setArg(2, sizeof(int), &m_octaves);
    kernel.setArg(3, sizeof(float), &m_lacunarity);
    kernel.setArg(4, sizeof(float), &m_gain);
    kernel.setArg(5, sizeof(float), &m_fractalBounding);
    kernel.setArg(6, sizeof(int), &m_interp);
    kernel.setArg(7, buf_m_perm);
    kernel.setArg(8, buf_m_perm12);
    kernel.setArg(9, sizeof(size_t), &size_x);
    kernel.setArg(10, sizeof(size_t), &size_y);
    kernel.setArg(11, sizeof(float), &scale_x);
    kernel.setArg(12, sizeof(float), &scale_y);
    kernel.setArg(13, sizeof(float), &offset_x);
    kernel.setArg(14, sizeof(float), &offset_y);
    kernel.setArg(15, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_Simplex2(
    float m_frequency,                                // |
    cl_uchar* m_perm, cl_uchar* m_perm12,             // | IN : class members

    size_t size_x, size_t size_y,                     // |
    float scale_x, float scale_y,                     // | IN : Parameters
    float offset_x, float offset_y                    // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[SIMPLEX2]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_m_perm12(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm12, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, buf_m_perm);
    kernel.setArg(2, buf_m_perm12);
    kernel.setArg(3, sizeof(size_t), &size_x);
    kernel.setArg(4, sizeof(size_t), &size_y);
    kernel.setArg(5, sizeof(float), &scale_x);
    kernel.setArg(6, sizeof(float), &scale_y);
    kernel.setArg(7, sizeof(float), &offset_x);
    kernel.setArg(8, sizeof(float), &offset_y);
    kernel.setArg(9, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_SimplexFractal2(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // | IN : class members
    cl_uchar* m_perm,  cl_uchar* m_perm12,                                     // |

    size_t size_x, size_t size_y,                                              // |
    float scale_x, float scale_y,                                              // | IN : Parameters
    float offset_x, float offset_y                                             // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[SIMPLEXFRACTAL2]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_m_perm12(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm12, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_fractalType);
    kernel.setArg(2, sizeof(int), &m_octaves);
    kernel.setArg(3, sizeof(float), &m_lacunarity);
    kernel.setArg(4, sizeof(float), &m_gain);
    kernel.setArg(5, sizeof(float), &m_fractalBounding);
    kernel.setArg(6, buf_m_perm);
    kernel.setArg(7, buf_m_perm12);
    kernel.setArg(8, sizeof(size_t), &size_x);
    kernel.setArg(9, sizeof(size_t), &size_y);
    kernel.setArg(10, sizeof(float), &scale_x);
    kernel.setArg(11, sizeof(float), &scale_y);
    kernel.setArg(12, sizeof(float), &offset_x);
    kernel.setArg(13, sizeof(float), &offset_y);
    kernel.setArg(14, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_Cellular2(
    float m_frequency,                                        // |
    int m_cellularDistanceFunction, int m_cellularReturnType, // | IN : class members
    cl_uchar* m_perm, int m_seed,                             // |

    size_t size_x, size_t size_y,                             // |
    float scale_x, float scale_y,                             // | IN : Parameters
    float offset_x, float offset_y                            // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[CELLULAR2]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_cellularDistanceFunction);
    kernel.setArg(2, sizeof(int), &m_cellularReturnType);
    kernel.setArg(3, buf_m_perm);
    kernel.setArg(4, sizeof(int), &m_seed);
    kernel.setArg(5, sizeof(size_t), &size_x);
    kernel.setArg(6, sizeof(size_t), &size_y);
    kernel.setArg(7, sizeof(float), &scale_x);
    kernel.setArg(8, sizeof(float), &scale_y);
    kernel.setArg(9, sizeof(float), &offset_x);
    kernel.setArg(10, sizeof(float), &offset_y);
    kernel.setArg(11, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_WhiteNoise2(
    int m_seed,                     // IN : class members

    size_t size_x, size_t size_y,   // |
    float scale_x, float scale_y,   // | IN : Parameters
    float offset_x, float offset_y  // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[WHITENOISE2]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(int), &m_seed);
    kernel.setArg(1, sizeof(size_t), &size_x);
    kernel.setArg(2, sizeof(size_t), &size_y);
    kernel.setArg(3, sizeof(float), &scale_x);
    kernel.setArg(4, sizeof(float), &scale_y);
    kernel.setArg(5, sizeof(float), &offset_x);
    kernel.setArg(6, sizeof(float), &offset_y);
    kernel.setArg(7, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_WhiteNoiseInt2(
    int m_seed,                     // IN : class members

    size_t size_x, size_t size_y,   // |
    int scale_x, int scale_y,       // | IN : Parameters
    int offset_x, int offset_y      // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[WHITENOISEINT2]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(int), &m_seed);
    kernel.setArg(1, sizeof(size_t), &size_x);
    kernel.setArg(2, sizeof(size_t), &size_y);
    kernel.setArg(3, sizeof(int), &scale_x);
    kernel.setArg(4, sizeof(int), &scale_y);
    kernel.setArg(5, sizeof(int), &offset_x);
    kernel.setArg(6, sizeof(int), &offset_y);
    kernel.setArg(7, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}

//3D
float* KernelAdapter::GEN_Value3(
    float m_frequency,                              // |
    int m_interp,                                   // | IN : class members
    cl_uchar* m_perm,                               // |

    size_t size_x, size_t size_y, size_t size_z,    // |
    float scale_x, float scale_y, float scale_z,    // | IN : Parameters
    float offset_x, float offset_y, float offset_z  // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[VALUE3]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_interp);
    kernel.setArg(2, buf_m_perm);
    kernel.setArg(3, sizeof(size_t), &size_x);
    kernel.setArg(4, sizeof(size_t), &size_y);
    kernel.setArg(5, sizeof(size_t), &size_z);
    kernel.setArg(6, sizeof(float), &scale_x);
    kernel.setArg(7, sizeof(float), &scale_y);
    kernel.setArg(8, sizeof(float), &scale_z);
    kernel.setArg(9, sizeof(float), &offset_x);
    kernel.setArg(10, sizeof(float), &offset_y);
    kernel.setArg(11, sizeof(float), &offset_z);
    kernel.setArg(12, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_ValueFractal3(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
    int m_interp,                                                              // | IN : class members
    cl_uchar* m_perm,                                                          // |

    size_t size_x, size_t size_y, size_t size_z,                               // |
    float scale_x, float scale_y, float scale_z,                               // | IN : Parameters
    float offset_x, float offset_y, float offset_z                             // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[VALUEFRACTAL3]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_fractalType);
    kernel.setArg(2, sizeof(float), &m_lacunarity);
    kernel.setArg(3, sizeof(float), &m_gain);
    kernel.setArg(4, sizeof(int), &m_octaves);
    kernel.setArg(5, sizeof(float), &m_fractalBounding);
    kernel.setArg(6, sizeof(int), &m_interp);
    kernel.setArg(7, buf_m_perm);
    kernel.setArg(8, sizeof(size_t), &size_x);
    kernel.setArg(9, sizeof(size_t), &size_y);
    kernel.setArg(10, sizeof(size_t), &size_z);
    kernel.setArg(11, sizeof(float), &scale_x);
    kernel.setArg(12, sizeof(float), &scale_y);
    kernel.setArg(13, sizeof(float), &scale_z);
    kernel.setArg(14, sizeof(float), &offset_x);
    kernel.setArg(15, sizeof(float), &offset_y);
    kernel.setArg(16, sizeof(float), &offset_z);
    kernel.setArg(17, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_Perlin3(
    float m_frequency,                                // |
    int m_interp,                                     // | IN : class members
    cl_uchar* m_perm, cl_uchar* m_perm12,             // |

    size_t size_x, size_t size_y, size_t size_z,      // |
    float scale_x, float scale_y, float scale_z,      // | IN : Parameters
    float offset_x, float offset_y, float offset_z    // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[PERLIN3]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_m_perm12(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm12, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_interp);
    kernel.setArg(2, buf_m_perm);
    kernel.setArg(3, buf_m_perm12);
    kernel.setArg(4, sizeof(size_t), &size_x);
    kernel.setArg(5, sizeof(size_t), &size_y);
    kernel.setArg(6, sizeof(size_t), &size_z);
    kernel.setArg(7, sizeof(float), &scale_x);
    kernel.setArg(8, sizeof(float), &scale_y);
    kernel.setArg(9, sizeof(float), &scale_z);
    kernel.setArg(10, sizeof(float), &offset_x);
    kernel.setArg(11, sizeof(float), &offset_y);
    kernel.setArg(12, sizeof(float), &offset_z);
    kernel.setArg(13, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_PerlinFractal3(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
    int m_interp,                                                              // | IN : class members
    cl_uchar* m_perm,  cl_uchar* m_perm12,                                     // |

    size_t size_x, size_t size_y, size_t size_z,                               // |
    float scale_x, float scale_y, float scale_z,                               // | IN : Parameters
    float offset_x, float offset_y, float offset_z                             // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[PERLINFRACTAL3]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_m_perm12(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm12, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_fractalType);
    kernel.setArg(2, sizeof(int), &m_octaves);
    kernel.setArg(3, sizeof(float), &m_lacunarity);
    kernel.setArg(4, sizeof(float), &m_gain);
    kernel.setArg(5, sizeof(float), &m_fractalBounding);
    kernel.setArg(6, sizeof(int), &m_interp);
    kernel.setArg(7, buf_m_perm);
    kernel.setArg(8, buf_m_perm12);
    kernel.setArg(9, sizeof(size_t), &size_x);
    kernel.setArg(10, sizeof(size_t), &size_y);
    kernel.setArg(11, sizeof(size_t), &size_z);
    kernel.setArg(12, sizeof(float), &scale_x);
    kernel.setArg(13, sizeof(float), &scale_y);
    kernel.setArg(14, sizeof(float), &scale_z);
    kernel.setArg(15, sizeof(float), &offset_x);
    kernel.setArg(16, sizeof(float), &offset_y);
    kernel.setArg(17, sizeof(float), &offset_z);
    kernel.setArg(18, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_Simplex3(
    float m_frequency,                                // |
    cl_uchar* m_perm, cl_uchar* m_perm12,             // | IN : class members

    size_t size_x, size_t size_y, size_t size_z,      // |
    float scale_x, float scale_y, float scale_z,      // | IN : Parameters
    float offset_x, float offset_y, float offset_z    // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[SIMPLEX3]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_m_perm12(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm12, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, buf_m_perm);
    kernel.setArg(2, buf_m_perm12);
    kernel.setArg(3, sizeof(size_t), &size_x);
    kernel.setArg(4, sizeof(size_t), &size_y);
    kernel.setArg(5, sizeof(size_t), &size_z);
    kernel.setArg(6, sizeof(float), &scale_x);
    kernel.setArg(7, sizeof(float), &scale_y);
    kernel.setArg(8, sizeof(float), &scale_z);
    kernel.setArg(9, sizeof(float), &offset_x);
    kernel.setArg(10, sizeof(float), &offset_y);
    kernel.setArg(11, sizeof(float), &offset_z);
    kernel.setArg(12, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_SimplexFractal3(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // | IN : class members
    cl_uchar* m_perm,  cl_uchar* m_perm12,                                     // |

    size_t size_x, size_t size_y, size_t size_z,                               // |
    float scale_x, float scale_y, float scale_z,                               // | IN : Parameters
    float offset_x, float offset_y, float offset_z                             // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[SIMPLEXFRACTAL3]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_m_perm12(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm12, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_fractalType);
    kernel.setArg(2, sizeof(int), &m_octaves);
    kernel.setArg(3, sizeof(float), &m_lacunarity);
    kernel.setArg(4, sizeof(float), &m_gain);
    kernel.setArg(5, sizeof(float), &m_fractalBounding);
    kernel.setArg(6, buf_m_perm);
    kernel.setArg(7, buf_m_perm12);
    kernel.setArg(8, sizeof(size_t), &size_x);
    kernel.setArg(9, sizeof(size_t), &size_y);
    kernel.setArg(10, sizeof(size_t), &size_z);
    kernel.setArg(11, sizeof(float), &scale_x);
    kernel.setArg(12, sizeof(float), &scale_y);
    kernel.setArg(13, sizeof(float), &scale_z);
    kernel.setArg(14, sizeof(float), &offset_x);
    kernel.setArg(15, sizeof(float), &offset_y);
    kernel.setArg(16, sizeof(float), &offset_z);
    kernel.setArg(17, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_Cellular3(
    float m_frequency,                                        // |
    int m_cellularDistanceFunction, int m_cellularReturnType, // | IN : class members
    cl_uchar* m_perm, int m_seed,                             // |

    size_t size_x, size_t size_y, size_t size_z,              // |
    float scale_x, float scale_y, float scale_z,              // | IN : Parameters
    float offset_x, float offset_y, float offset_z            // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[CELLULAR3]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, sizeof(int), &m_cellularDistanceFunction);
    kernel.setArg(2, sizeof(int), &m_cellularReturnType);
    kernel.setArg(3, buf_m_perm);
    kernel.setArg(4, sizeof(int), &m_seed);
    kernel.setArg(5, sizeof(size_t), &size_x);
    kernel.setArg(6, sizeof(size_t), &size_y);
    kernel.setArg(7, sizeof(size_t), &size_z);
    kernel.setArg(8, sizeof(float), &scale_x);
    kernel.setArg(9, sizeof(float), &scale_y);
    kernel.setArg(10, sizeof(float), &scale_z);
    kernel.setArg(11, sizeof(float), &offset_x);
    kernel.setArg(12, sizeof(float), &offset_y);
    kernel.setArg(13, sizeof(float), &offset_z);
    kernel.setArg(14, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_WhiteNoise3(
    int m_seed,                                     // IN : class members

    size_t size_x, size_t size_y, size_t size_z,    // |
    float scale_x, float scale_y, float scale_z,    // | IN : Parameters
    float offset_x, float offset_y, float offset_z  // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[WHITENOISE3]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(int), &m_seed);
    kernel.setArg(1, sizeof(size_t), &size_x);
    kernel.setArg(2, sizeof(size_t), &size_y);
    kernel.setArg(3, sizeof(size_t), &size_z);
    kernel.setArg(4, sizeof(float), &scale_x);
    kernel.setArg(5, sizeof(float), &scale_y);
    kernel.setArg(6, sizeof(float), &scale_z);
    kernel.setArg(7, sizeof(float), &offset_x);
    kernel.setArg(8, sizeof(float), &offset_y);
    kernel.setArg(9, sizeof(float), &offset_z);
    kernel.setArg(10, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_WhiteNoiseInt3(
    int m_seed,                                     // IN : class members

    size_t size_x, size_t size_y, size_t size_z,    // |
    int scale_x, int scale_y, int scale_z,          // | IN : Parameters
    int offset_x, int offset_y, int offset_z        // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[WHITENOISEINT3]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(int), &m_seed);
    kernel.setArg(1, sizeof(size_t), &size_x);
    kernel.setArg(2, sizeof(size_t), &size_y);
    kernel.setArg(3, sizeof(size_t), &size_z);
    kernel.setArg(4, sizeof(int), &scale_x);
    kernel.setArg(5, sizeof(int), &scale_y);
    kernel.setArg(6, sizeof(int), &scale_z);
    kernel.setArg(7, sizeof(int), &offset_x);
    kernel.setArg(8, sizeof(int), &offset_y);
    kernel.setArg(9, sizeof(int), &offset_z);
    kernel.setArg(10, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}

//4D
float* KernelAdapter::GEN_Simplex4(
    float m_frequency,                                              // |
    cl_uchar* m_perm,                                               // | IN : class members

    size_t size_x, size_t size_y, size_t size_z, size_t size_w,     // |
    float scale_x, float scale_y, float scale_z, float scale_w,     // | IN : Parameters
    float offset_x, float offset_y, float offset_z, float offset_w  // |
) {
        //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z * size_w;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[SIMPLEX4]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_m_perm(pimpl->context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(cl_uchar) * 512, m_perm, &err);
    assert(err == CL_SUCCESS);
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(float), &m_frequency);
    kernel.setArg(1, buf_m_perm);
    kernel.setArg(2, sizeof(size_t), &size_x);
    kernel.setArg(3, sizeof(size_t), &size_y);
    kernel.setArg(4, sizeof(size_t), &size_z);
    kernel.setArg(5, sizeof(size_t), &size_w);
    kernel.setArg(6, sizeof(float), &scale_x);
    kernel.setArg(7, sizeof(float), &scale_y);
    kernel.setArg(8, sizeof(float), &scale_z);
    kernel.setArg(9, sizeof(float), &scale_w);
    kernel.setArg(10, sizeof(float), &offset_x);
    kernel.setArg(11, sizeof(float), &offset_y);
    kernel.setArg(12, sizeof(float), &offset_z);
    kernel.setArg(13, sizeof(float), &offset_w);
    kernel.setArg(14, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_WhiteNoise4(
    int m_seed,                                                     // IN : class members

    size_t size_x, size_t size_y, size_t size_z, size_t size_w,     // |
    float scale_x, float scale_y, float scale_z, float scale_w,     // | IN : Parameters
    float offset_x, float offset_y, float offset_z, float offset_w  // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[WHITENOISE4]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(int), &m_seed);
    kernel.setArg(1, sizeof(size_t), &size_x);
    kernel.setArg(2, sizeof(size_t), &size_y);
    kernel.setArg(3, sizeof(size_t), &size_z);
    kernel.setArg(4, sizeof(size_t), &size_w);
    kernel.setArg(5, sizeof(float), &scale_x);
    kernel.setArg(6, sizeof(float), &scale_y);
    kernel.setArg(7, sizeof(float), &scale_z);
    kernel.setArg(8, sizeof(float), &scale_w);
    kernel.setArg(9, sizeof(float), &offset_x);
    kernel.setArg(10, sizeof(float), &offset_y);
    kernel.setArg(11, sizeof(float), &offset_z);
    kernel.setArg(12, sizeof(float), &offset_w);
    kernel.setArg(13, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
float* KernelAdapter::GEN_WhiteNoiseInt4(
    int m_seed,                                                 // IN : class members

    size_t size_x, size_t size_y, size_t size_z, size_t size_w, // |
    int scale_x, int scale_y, int scale_z, int scale_w,         // | IN : Parameters
    int offset_x, int offset_y, int offset_z, int offset_w      // |
) {
    //Configure stuff
    cl_int err;
    size_t msize = size_x * size_y * size_z * size_w;

    //Get CL objects
    cl::Kernel kernel(pimpl->kernels[WHITENOISEINT4]);
    cl::CommandQueue cmd_queue(pimpl->context, *pimpl->device);

    //Create buffers
    cl::Buffer buf_result(pimpl->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * msize, nullptr, &err);
    assert(err == CL_SUCCESS);

    //Prepare kernel
    kernel.setArg(0, sizeof(int), &m_seed);
    kernel.setArg(1, sizeof(size_t), &size_x);
    kernel.setArg(2, sizeof(size_t), &size_y);
    kernel.setArg(3, sizeof(size_t), &size_z);
    kernel.setArg(4, sizeof(size_t), &size_w);
    kernel.setArg(5, sizeof(int), &scale_x);
    kernel.setArg(6, sizeof(int), &scale_y);
    kernel.setArg(7, sizeof(int), &scale_z);
    kernel.setArg(8, sizeof(int), &scale_w);
    kernel.setArg(9, sizeof(int), &offset_x);
    kernel.setArg(10, sizeof(int), &offset_y);
    kernel.setArg(11, sizeof(int), &offset_z);
    kernel.setArg(12, sizeof(int), &offset_w);
    kernel.setArg(13, buf_result);

    //Execute task
    float* result = new float[msize];
    err = cmd_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(msize));
    assert(err == CL_SUCCESS);
    err = cmd_queue.enqueueReadBuffer(buf_result, CL_TRUE, 0, sizeof(float) * msize, result);
    assert(err == CL_SUCCESS);

    return result;
}
