#include <CL/cl.hpp>

//Initialize
void load_kernels(/*const char* src, const size_t len*/);

//Kernels
//2D
float* GEN_Value2(
    float m_frequency,              // |
    int m_interp,                   // | IN : class members
    cl_uchar* m_perm,               // |

    size_t size_x, size_t size_y,   // |
    float scale_x, float scale_y,   // | IN : Parameters
    float offset_x, float offset_y  // |
);
float* GEN_ValueFractal2(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
    int m_interp,                                                              // | IN : class members
    cl_uchar* m_perm,                                                          // |

    size_t size_x, size_t size_y,                                              // |
    float scale_x, float scale_y,                                              // | IN : Parameters
    float offset_x, float offset_y                                             // |
);
float* GEN_Perlin2(
    float m_frequency,                                // |
    int m_interp,                                     // | IN : class members
    cl_uchar* m_perm, cl_uchar* m_perm12,             // |

    size_t size_x, size_t size_y,                     // |
    float scale_x, float scale_y,                     // | IN : Parameters
    float offset_x, float offset_y                    // |
);
float* GEN_PerlinFractal2(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
    int m_interp,                                                              // | IN : class members
    cl_uchar* m_perm,  cl_uchar* m_perm12,                                     // |

    size_t size_x, size_t size_y,                                              // |
    float scale_x, float scale_y,                                              // | IN : Parameters
    float offset_x, float offset_y                                             // |
);
float* GEN_Simplex2(
    float m_frequency,                    // |
    cl_uchar* m_perm, cl_uchar* m_perm12, // | IN : class members

    size_t size_x, size_t size_y,         // |
    float scale_x, float scale_y,         // | IN : Parameters
    float offset_x, float offset_y        // |
);
float* GEN_SimplexFractal2(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // | IN : class members
    cl_uchar* m_perm,  cl_uchar* m_perm12,                                     // |

    size_t size_x, size_t size_y,                                              // |
    float scale_x, float scale_y,                                              // | IN : Parameters
    float offset_x, float offset_y                                             // |
);
float* GEN_Cellular2(
    float m_frequency,                                        // |
    int m_cellularDistanceFunction, int m_cellularReturnType, // | IN : class members
    cl_uchar* m_perm, int m_seed,                             // |

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
    cl_uchar* m_perm,                               // |

    size_t size_x, size_t size_y, size_t size_z,    // |
    float scale_x, float scale_y, float scale_z,    // | IN : Parameters
    float offset_x, float offset_y, float offset_z  // |
);
float* GEN_ValueFractal3(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
    int m_interp,                                                              // | IN : class members
    cl_uchar* m_perm,                                                          // |

    size_t size_x, size_t size_y, size_t size_z,                               // |
    float scale_x, float scale_y, float scale_z,                               // | IN : Parameters
    float offset_x, float offset_y, float offset_z                             // |
);
float* GEN_Perlin3(
    float m_frequency,                                // |
    int m_interp,                                     // | IN : class members
    cl_uchar* m_perm, cl_uchar* m_perm12,             // |

    size_t size_x, size_t size_y, size_t size_z,      // |
    float scale_x, float scale_y, float scale_z,      // | IN : Parameters
    float offset_x, float offset_y, float offset_z    // |
);
float* GEN_PerlinFractal3(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // |
    int m_interp,                                                              // | IN : class members
    cl_uchar* m_perm,  cl_uchar* m_perm12,                                     // |

    size_t size_x, size_t size_y, size_t size_z,                               // |
    float scale_x, float scale_y, float scale_z,                               // | IN : Parameters
    float offset_x, float offset_y, float offset_z                             // |
);
float* GEN_Simplex3(
    float m_frequency,                                // |
    cl_uchar* m_perm, cl_uchar* m_perm12,             // | IN : class members

    size_t size_x, size_t size_y, size_t size_z,      // |
    float scale_x, float scale_y, float scale_z,      // | IN : Parameters
    float offset_x, float offset_y, float offset_z    // |
);
float* GEN_SimplexFractal3(
    float m_frequency, int m_fractalType,                                      // |
    int m_octaves, float m_lacunarity, float m_gain, float m_fractalBounding,  // | IN : class members
    cl_uchar* m_perm,  cl_uchar* m_perm12,                                     // |

    size_t size_x, size_t size_y, size_t size_z,                               // |
    float scale_x, float scale_y, float scale_z,                               // | IN : Parameters
    float offset_x, float offset_y, float offset_z                             // |
);
float* GEN_Cellular3(
    float m_frequency,                                        // |
    int m_cellularDistanceFunction, int m_cellularReturnType, // | IN : class members
    cl_uchar* m_perm, int m_seed,                             // |

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
    cl_uchar* m_perm,                                               // | IN : class members

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
