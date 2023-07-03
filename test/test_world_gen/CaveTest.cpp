//
// Created by robcholz on 7/2/23.
//
#include "gtest/gtest.h"
#include "util/math/noise/PerlinNoise.hpp"
#include "util/ppm.hpp"

TEST(CaveTest,BasicAssertion){
	// Define the size of the image
	unsigned int width = 1000, height = 800;
	ppm image(width, height);
	math::PerlinNoise pn;
	unsigned int kk = 0;
	for (unsigned int i = 0; i < height; ++i) {     // y
		for (unsigned int j =  0; j < width; ++j) {  // x
			double x = (double) j / ((double) width);
			double y = (double) i / ((double) height);

			// Typical Perlin noise
			double n = pn.noise(10 * x, 10 * y, 0.8);

			// Wood like structure
			n = 2 * pn.noise(x * 5, y * 5, 0.55);
			//n = n - floor(n);
			auto color_channel = (uint8_t) floor(255 * n);
			if (color_channel > 250) { // red, fill air
				image.r[kk] = 255;
				image.g[kk] = 0;
				image.b[kk] = 0;
			} else {
				image.r[kk] = 50;
				image.g[kk] = 205;
				image.b[kk] = 50;
			} // reserve

			kk++;
		}
	}
	// Save the image in a binary PPM file
	image.write("../test/test_world_gen/figure_8_R.ppm");
}