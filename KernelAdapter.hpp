#include <memory>

#include "DeviceManager.hpp"

typedef unsigned char uchar;

class KernelAdapter {
public:
    //Initialize
    KernelAdapter(Device& device);
    ~KernelAdapter();

    //Kernels
    //2D
    float* GEN_Value2(
        float m_frequency,              // |
        int m_interp,                   // | IN : class members
        int m_seed,                     // |

        size_t size_x, size_t size_y,   // |
        float scale_x, float scale_y,   // | IN : Parameters
        float offset_x, float offset_y  // |
    );
    float* GEN_ValueFractal2(
        float m_frequency, int m_fractalType,                                      // |
        int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
        int m_interp,                                                              // | IN : class members
        int m_seed,                                                                // |

        size_t size_x, size_t size_y,                                              // |
        float scale_x, float scale_y,                                              // | IN : Parameters
        float offset_x, float offset_y                                             // |
    );
    float* GEN_Perlin2(
        float m_frequency,                                // |
        int m_interp,                                     // | IN : class members
        int m_seed,                                       // |

        size_t size_x, size_t size_y,                     // |
        float scale_x, float scale_y,                     // | IN : Parameters
        float offset_x, float offset_y                    // |
    );
    float* GEN_PerlinFractal2(
        float m_frequency, int m_fractalType,                                      // |
        int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
        int m_interp,                                                              // | IN : class members
        int m_seed,                                                                // |

        size_t size_x, size_t size_y,                                              // |
        float scale_x, float scale_y,                                              // | IN : Parameters
        float offset_x, float offset_y                                             // |
    );
    float* GEN_Simplex2(
        float m_frequency,                    // |
        int m_seed,                           // | IN : class members

        size_t size_x, size_t size_y,         // |
        float scale_x, float scale_y,         // | IN : Parameters
        float offset_x, float offset_y        // |
    );
    float* GEN_SimplexFractal2(
        float m_frequency, int m_fractalType,                                      // |
        int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // | IN : class members
        int m_seed,                                                                // |

        size_t size_x, size_t size_y,                                              // |
        float scale_x, float scale_y,                                              // | IN : Parameters
        float offset_x, float offset_y                                             // |
    );
    float* GEN_Cellular2(
        float m_frequency,                                        // |
        int m_cellularDistanceFunction, int m_cellularReturnType, // | IN : class members
        int m_seed,                                               // |

        size_t size_x, size_t size_y,                             // |
        float scale_x, float scale_y,                             // | IN : Parameters
        float offset_x, float offset_y                            // |
    );
    float* GEN_WhiteNoise2(
        int m_seed,                     // IN : class members

        size_t size_x, size_t size_y,   // |
        float scale_x, float scale_y,   // | IN : Parameters
        float offset_x, float offset_y  // |
    );
    float* GEN_WhiteNoiseInt2(
        int m_seed,                     // IN : class members

        size_t size_x, size_t size_y,   // |
        int scale_x, int scale_y,       // | IN : Parameters
        int offset_x, int offset_y      // |
    );

    //3D
    float* GEN_Value3(
        float m_frequency,                              // |
        int m_interp,                                   // | IN : class members
        int m_seed,                                     // |

        size_t size_x, size_t size_y, size_t size_z,    // |
        float scale_x, float scale_y, float scale_z,    // | IN : Parameters
        float offset_x, float offset_y, float offset_z  // |
    );
    float* GEN_ValueFractal3(
        float m_frequency, int m_fractalType,                                      // |
        int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
        int m_interp,                                                              // | IN : class members
        int m_seed,                                                                // |

        size_t size_x, size_t size_y, size_t size_z,                               // |
        float scale_x, float scale_y, float scale_z,                               // | IN : Parameters
        float offset_x, float offset_y, float offset_z                             // |
    );
    float* GEN_Perlin3(
        float m_frequency,                                // |
        int m_interp,                                     // | IN : class members
        int m_seed,                                       // |

        size_t size_x, size_t size_y, size_t size_z,      // |
        float scale_x, float scale_y, float scale_z,      // | IN : Parameters
        float offset_x, float offset_y, float offset_z    // |
    );
    float* GEN_PerlinFractal3(
        float m_frequency, int m_fractalType,                                      // |
        int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
        int m_interp,                                                              // | IN : class members
        int m_seed,                                                                // |

        size_t size_x, size_t size_y, size_t size_z,                               // |
        float scale_x, float scale_y, float scale_z,                               // | IN : Parameters
        float offset_x, float offset_y, float offset_z                             // |
    );
    float* GEN_Simplex3(
        float m_frequency,                                // |
        int m_seed,                                       // | IN : class members

        size_t size_x, size_t size_y, size_t size_z,      // |
        float scale_x, float scale_y, float scale_z,      // | IN : Parameters
        float offset_x, float offset_y, float offset_z    // |
    );
    float* GEN_SimplexFractal3(
        float m_frequency, int m_fractalType,                                      // |
        int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // | IN : class members
        int m_seed,                                                                // |

        size_t size_x, size_t size_y, size_t size_z,                               // |
        float scale_x, float scale_y, float scale_z,                               // | IN : Parameters
        float offset_x, float offset_y, float offset_z                             // |
    );
    float* GEN_Cellular3(
        float m_frequency,                                        // |
        int m_cellularDistanceFunction, int m_cellularReturnType, // | IN : class members
        int m_seed,                                               // |

        size_t size_x, size_t size_y, size_t size_z,              // |
        float scale_x, float scale_y, float scale_z,              // | IN : Parameters
        float offset_x, float offset_y, float offset_z            // |
    );
    float* GEN_WhiteNoise3(
        int m_seed,                                     // IN : class members

        size_t size_x, size_t size_y, size_t size_z,    // |
        float scale_x, float scale_y, float scale_z,    // | IN : Parameters
        float offset_x, float offset_y, float offset_z  // |
    );
    float* GEN_WhiteNoiseInt3(
        int m_seed,                                     // IN : class members

        size_t size_x, size_t size_y, size_t size_z,    // |
        int scale_x, int scale_y, int scale_z,          // | IN : Parameters
        int offset_x, int offset_y, int offset_z        // |
    );

    //4D
    float* GEN_Simplex4(
        float m_frequency,                                              // |
        int m_seed,                                                     // | IN : class members

        size_t size_x, size_t size_y, size_t size_z, size_t size_w,     // |
        float scale_x, float scale_y, float scale_z, float scale_w,     // | IN : Parameters
        float offset_x, float offset_y, float offset_z, float offset_w  // |
    );
    float* GEN_WhiteNoise4(
        int m_seed,                                                     // IN : class members

        size_t size_x, size_t size_y, size_t size_z, size_t size_w,     // |
        float scale_x, float scale_y, float scale_z, float scale_w,     // | IN : Parameters
        float offset_x, float offset_y, float offset_z, float offset_w  // |
    );
    float* GEN_WhiteNoiseInt4(
        int m_seed,                                                 // IN : class members

        size_t size_x, size_t size_y, size_t size_z, size_t size_w, // |
        int scale_x, int scale_y, int scale_z, int scale_w,         // | IN : Parameters
        int offset_x, int offset_y, int offset_z, int offset_w      // |
    );

    /*//Perturb
    //2D
    void DO_Perturb2(
        float m_perturbAmp, float m_frequency,        // |
        int m_interp,                                 // | IN : class members
        int m_seed,                                // |

        size_t size_m,                                // IN : Parameters

        float* arr_x, float* arr_y                    // IN-OUT : Noise matrices
    );
    void DO_PerturbFractal2(
        float m_perturbAmp, float m_fractalBounding, float m_frequency, int m_octaves, float m_lacunarity, float m_gain, // |
        int m_interp,                                                                                                    // | IN : class members
        int m_seed,                                                                                                   // |

        size_t size_m,                                                                                                   // IN : Parameters

        float* arr_x, float* arr_y                                                                                       // IN-OUT : Noise matrices
    );

    //3D
    void DO_Perturb3(
        float m_perturbAmp, float m_frequency,        // |
        int m_interp,                                 // | IN : class members
        int m_seed,                                // |

        size_t size_m,                                // IN : Parameters

        float* arr_x, float* arr_y, float* arr_z      // IN-OUT : Noise matrices
    );
    void DO_PerturbFractal3(
        float m_perturbAmp, float m_fractalBounding, float m_frequency, int m_octaves, float m_lacunarity, float m_gain, // |
        int m_interp,                                                                                                    // | IN : class members
        int m_seed,                                                                                                   // |

        size_t size_m,                                                                                                   // IN : Parameters

        float* arr_x, float* arr_y, float* arr_z                                                                         // IN-OUT : Noise matrices
    );*/

private:
    class impl;
    std::unique_ptr<impl> pimpl;
    //std::unique_ptr<cl::Device> device = nullptr;
    //cl::Context context;
    //cl::Kernel* kernels = nullptr;
};
