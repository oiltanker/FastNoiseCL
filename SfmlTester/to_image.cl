R"====(


typedef struct {
	uchar red;
	uchar green;
	uchar blue;
	uchar alpha;
} RGBA;

RGBA to_shade(float x) {
	x = x / 2 + 0.5;
	uchar l = x * 255;
	
	RGBA c;
	c.red = l;
	c.green = l;
	c.blue = l;
	c.alpha = 255;
	
	return c;
}

__kernel void GEN_shade(
	__global float* noise, // IN : noise matrix
	__global RGBA* color   // OUT : color matrix (pixels)
) {
	ulong i = get_global_id(0);
	color[i] = to_shade(noise[i]);
}


)===="