R"====(
// to_image.cl
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

typedef struct {
	uchar red;
	uchar green;
	uchar blue;
	uchar alpha;
} RGBA;

RGBA to_shade(float x, float min, float max) {
	x = (x - min) / (max - min);
	uchar l = x * 255;

	RGBA c;
	c.red = l;
	c.green = l;
	c.blue = l;
	c.alpha = 255;

	return c;
}

RGBA to_color(float x1, float x2, float x3, float min, float max) {
	x1 = (x1 - min) / (max - min);
	x2 = (x2 - min) / (max - min);
	x3 = (x3 - min) / (max - min);

	uchar l1 = x1 * 255;
	uchar l2 = x2 * 255;
	uchar l3 = x3 * 255;

	RGBA c;
	c.red = l1;
	c.green = l2;
	c.blue = l3;
	c.alpha = 255;

	return c;
}

// Kernels
__kernel void GEN_shade(
    float min, float max,// IN : parameters

	__global float* noise, // IN : noise matrix
	__global RGBA* color   // OUT : color matrix (pixels)
) {
	ulong i = get_global_id(0);
	color[i] = to_shade(noise[i], min, max);
}

__kernel void GEN_color(
    float min, float max,// IN : parameters

	__global float* noise1, // |
	__global float* noise2, // | IN : noise matrices
	__global float* noise3, // |

	__global RGBA* color   // OUT : color matrix (pixels)
) {
	ulong i = get_global_id(0);
	color[i] = to_color(noise1[i], noise2[i], noise3[i], min, max);
}


)===="
