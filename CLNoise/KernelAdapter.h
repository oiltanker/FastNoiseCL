// KernelAdapter.hpp
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

#include <memory>

#include "DeviceManager.h"

struct Snapshot {
    int m_seed;
    float m_frequency;
    float m_perturbFrequency;
    float m_cellularJitter;
    int m_cellularDistanceIndex0;
    int m_cellularDistanceIndex1;
    int m_smoothing;
    int m_noiseType;

    int m_octaves;
    float m_lacunarity;
    float m_gain;
    int m_fractalType;

    float m_fractalBounding;

    int m_cellularDistanceFunction;
    int m_cellularReturnType;

    float m_perturbSeed;
    float m_perturbAmp;
    int m_perturb;
    int m_perturbOctaves;
    float m_perturbGain;
    float m_perturbLacunarity;

    float m_perturbBounding;
    int m_perturbSmoothing;
};

class KernelAdapter {
public:
    //Initialize
    KernelAdapter(const Device& device);
    ~KernelAdapter();

    //Kernels
    //2D
    void GEN_Value2(
        Snapshot param,               // IN : class members

        size_t sizeX, size_t sizeY,   // |
        float scaleX, float scaleY,   // | IN : Parameters
        float offsetX, float offsetY, // |


        float* result
    );
    void GEN_ValueFractal2(
        Snapshot param,               // IN : class members

        size_t sizeX, size_t sizeY,   // |
        float scaleX, float scaleY,   // | IN : Parameters
        float offsetX, float offsetY, // |

        float* result
    );
    void GEN_Perlin2(
        Snapshot param,               // IN : class members

        size_t sizeX, size_t sizeY,   // |
        float scaleX, float scaleY,   // | IN : Parameters
        float offsetX, float offsetY, // |

        float* result
    );
    void GEN_PerlinFractal2(
        Snapshot param,               // IN : class members

        size_t sizeX, size_t sizeY,   // |
        float scaleX, float scaleY,   // | IN : Parameters
        float offsetX, float offsetY, // |

        float* result
    );
    void GEN_Simplex2(
        Snapshot param,               // IN : class members

        size_t sizeX, size_t sizeY,   // |
        float scaleX, float scaleY,   // | IN : Parameters
        float offsetX, float offsetY, // |

        float* result
    );
    void GEN_SimplexFractal2(
        Snapshot param,               // IN : class members

        size_t sizeX, size_t sizeY,   // |
        float scaleX, float scaleY,   // | IN : Parameters
        float offsetX, float offsetY, // |

        float* result
    );
    void GEN_Cellular2(
        Snapshot param,               // IN : class members

        size_t sizeX, size_t sizeY,   // |
        float scaleX, float scaleY,   // | IN : Parameters
        float offsetX, float offsetY, // |

        float* result
    );
    void GEN_WhiteNoise2(
        Snapshot param,               // IN : class members

        size_t sizeX, size_t sizeY,   // |
        float scaleX, float scaleY,   // | IN : Parameters
        float offsetX, float offsetY, // |

        float* result
    );

    //3D
    void GEN_Value3(
        Snapshot param,                              // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ,    // |
        float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, // |

        float* result
    );
    void GEN_ValueFractal3(
        Snapshot param,                              // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ,    // |
        float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, // |

        float* result
    );
    void GEN_Perlin3(
        Snapshot param,                              // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ,    // |
        float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, // |

        float* result
    );
    void GEN_PerlinFractal3(
        Snapshot param,                              // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ,    // |
        float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, // |

        float* result
    );
    void GEN_Simplex3(
        Snapshot param,                              // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ,    // |
        float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, // |

        float* result
    );
    void GEN_SimplexFractal3(
        Snapshot param,                              // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ,    // |
        float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, // |

        float* result
    );
    void GEN_Cellular3(
        Snapshot param,                              // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ,    // |
        float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, // |

        float* result
    );
    void GEN_WhiteNoise3(
        Snapshot param,                              // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ,    // |
        float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, // |

        float* result
    );

    //4D
    void GEN_Simplex4(
        Snapshot param,                                             // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ, size_t sizeW,     // |
        float scaleX, float scaleY, float scaleZ, float scaleW,     // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, float offsetW, // |

        float* result
    );
    void GEN_WhiteNoise4(
        Snapshot param,                                             // IN : class members

        size_t sizeX, size_t sizeY, size_t sizeZ, size_t sizeW,     // |
        float scaleX, float scaleY, float scaleZ, float scaleW,     // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, float offsetW, // |

        float* result
    );

    //NoiseLookup
    void GEN_Lookup_Cellular2(
        Snapshot* params, size_t size_p, // IN : members of all classes

        size_t sizeX, size_t sizeY,      // |
        float scaleX, float scaleY,      // | IN : Parameters
        float offsetX, float offsetY,    // |

        float* result
    );
    void GEN_Lookup_Cellular3(
        Snapshot* params, size_t size_p,             // IN : members of all classes

        size_t sizeX, size_t sizeY, size_t sizeZ,    // |
        float scaleX, float scaleY, float scaleZ,    // | IN : Parameters
        float offsetX, float offsetY, float offsetZ, // |

        float* result
    );

private:
    class impl;
    impl& rimpl;

    class simpl;
    static simpl& rsimpl;
};
