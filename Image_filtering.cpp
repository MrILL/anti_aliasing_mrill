#include <vector>
#include <iostream>
#include <math.h>

#include "Image_filtering.h"

using std::vector;

template<typename T>
static unsigned char to_uchar(T data) {
	return static_cast<unsigned char>((data > 255) ? 255 : (data < 0) ? 0 : data);
}

template<typename T, size_t size_row, size_t size_col>
static void copy_2d_array(T **&out, unsigned &size, const T(&in)[size_row][size_col]) {
	size = size_row;
	out = new T*[size];
	for (size_t i = 0; i < size; i++) {
		out[i] = new T[size_col];
		for (size_t j = 0; j < size; j++)
			out[i][j] = in[i][j];
	}
}

void get_mask(float **&out, unsigned &out_size, kernel type) {
	switch (type) {
	case edge_detection_v1:
		copy_2d_array(out, out_size, mask_edge_detection_v1);
		break;
	case edge_detection_v2:
		copy_2d_array(out, out_size, mask_edge_detection_v2);
		break;
	case edge_detection_v3:
		copy_2d_array(out, out_size, mask_edge_detection_v3);
		break;
	case sharpen:
		copy_2d_array(out, out_size, mask_sharpen);
		break;
	case box_blur:
		copy_2d_array(out, out_size, mask_box_blur);
		break;
	case gaussian_blur_s3:
		copy_2d_array(out, out_size, mask_gaussian_blur_s3);
		break;
	case gaussian_blur_s5:
		copy_2d_array(out, out_size, mask_gaussian_blur_s5);
		break;
	default:
		std::cout << "wrong kernel type\n";
		return;
	}

	//out = mask_edge_detection_2;
}

static void convolution_pixel(float **mask, unsigned mask_size,
					   std::vector<unsigned char> &out, std::vector<unsigned char> &in, unsigned width, unsigned height, 
					   unsigned x, unsigned y) {
	float convolution_R = 0;
	float convolution_G = 0;
	float convolution_B = 0;
	unsigned offset = mask_size / 2; /*offset from center square*/
	for (unsigned i = 0; i < mask_size; i++) {
		for (unsigned j = 0; j < mask_size; j++) {
			convolution_R += mask[i][j] * in[(y - offset + j)*width * 4 + (x - offset + i) * 4 + 0];
			convolution_G += mask[i][j] * in[(y - offset + j)*width * 4 + (x - offset + i) * 4 + 1];
			convolution_B += mask[i][j] * in[(y - offset + j)*width * 4 + (x - offset + i) * 4 + 2];
		}
	}
	out[y*width * 4 + x * 4 + 0] = to_uchar(convolution_R);
	out[y*width * 4 + x * 4 + 1] = to_uchar(convolution_G);
	out[y*width * 4 + x * 4 + 2] = to_uchar(convolution_B);
	out[y*width * 4 + x * 4 + 3] = in[y*width * 4 + x * 4 + 3]; /*alpha chanel without difference*/
}

vector<unsigned char> applied_mask(float **mask, unsigned mask_size, 
								   vector<unsigned char> image, unsigned width, unsigned height) {
	vector<unsigned char> new_image;
	new_image.resize(width*height*4);

	unsigned offset = mask_size / 2;
	
	
	for (unsigned color = 0; color < 4; color++) {//fill border of image
		for (unsigned indent = 0; indent < offset; indent++) {
			for (unsigned y = 0; y < height; y++) {
				// 0<x<indent - left border
				new_image[y*width*4 + indent*4 + color] = image[y*width*4 + indent*4 + color];
				// width-1-indent<x<width-1 - right border
				new_image[y*width*4 + (width-1-indent)*4 + color] = image[y*width*4 + (width-1-indent)*4 + color];
			}
			for (unsigned x = 0; x < width; x++) {
				// 0<y<indent - top border
				new_image[indent*width*4 + x*4 + color] = image[indent*width*4 + x*4 + color];
				// height-1-indent<y<height-1 - bottom border
				new_image[(height-1-indent)*width*4 + x*4 + color] = image[(height-1-indent)*width*4 + x*4 + color];
			}
		}
	}

	for (unsigned i = offset; i < width- offset; i++) {
		for (unsigned j = offset; j < height- offset; j++) {
			convolution_pixel(mask, mask_size, new_image, image, width, height, i, j);
		}
	}
	return new_image;
}
