#include <vector>
#include <iostream>
#include <math.h>

#include "Image_filtering.h"

using std::cout;//
using std::endl;//
using std::vector;

template<typename T>
unsigned char to_uchar(T data) {
	return static_cast<unsigned char>((data > 255) ? 255 : (data < 0) ? 0 : data);
}

template<typename T, size_t size_row, size_t size_col>
void copy_2d_array(T **&out, unsigned &size, const T(&in)[size_row][size_col]) {
	size = size_row;
	out = new T*[size];
	for (size_t i = 0; i < size; i++) {
		out[i] = new T[size_col];
		for (size_t j = 0; j < size; j++)
			out[i][j] = in[i][j];
	}
}

void output_sqr(vector<unsigned char> image, unsigned width, unsigned height, unsigned lenght) {
	for (unsigned i = 0; i < lenght; i++) {
		for (unsigned j = 0; j < lenght; j++) {
			cout << (int)image[i*width * 4 + j * 4] << ' ' << (int)image[i*width * 4 + j * 4 + 1] << ' ' << (int)image[i*width * 4 + j * 4 + 2] << ' ' << (int)image[i*width * 4 + j * 4 + 3] << '|';
		}
		cout << endl;
	}
}

void get_mask(float **&out, unsigned &out_size, enum kernel type) {
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

void get_mask(float(&out)[3][3]) {
	get_mask(out, edge_detection_v1);
}//


void convolution_pixel(float **mask, unsigned mask_size, vector<unsigned char> &out, vector<unsigned char> &in, unsigned width, unsigned height, unsigned x, unsigned y) {
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

vector<unsigned char> applied_mask(float **mask, unsigned mask_size, vector<unsigned char> image, unsigned width, unsigned height) {
	vector<unsigned char> new_image;
	new_image.resize(width*height*4);
	
	for (unsigned y = 0; y < height; y++) { // x=const
		//x=0
		new_image[y*width * 4 + 0] = image[y*width * 4 + 0];
		new_image[y*width * 4 + 1] = image[y*width * 4 + 1];
		new_image[y*width * 4 + 2] = image[y*width * 4 + 2];
		new_image[y*width * 4 + 3] = image[y*width * 4 + 3];
		//x=width-1
		new_image[y*width * 4 + (width - 1) * 4 + 0] = image[y*width * 4 + (width - 1) * 4 + 0];
		new_image[y*width * 4 + (width - 1) * 4 + 1] = image[y*width * 4 + (width - 1) * 4 + 1];
		new_image[y*width * 4 + (width - 1) * 4 + 2] = image[y*width * 4 + (width - 1) * 4 + 2];
		new_image[y*width * 4 + (width - 1) * 4 + 3] = image[y*width * 4 + (width - 1) * 4 + 3];
	}
	for (unsigned x = 0; x < width; x++) { //y=const
		//y=0
		new_image[x * 4 + 0] = image[x * 4 + 0];
		new_image[x * 4 + 1] = image[x * 4 + 1];
		new_image[x * 4 + 2] = image[x * 4 + 2];
		new_image[x * 4 + 3] = image[x * 4 + 3];
		//y=height-1
		new_image[(height - 1)*width * 4 + x * 4 + 0] = image[(height - 1)*width * 4 + x * 4 + 0];
		new_image[(height - 1)*width * 4 + x * 4 + 1] = image[(height - 1)*width * 4 + x * 4 + 1];
		new_image[(height - 1)*width * 4 + x * 4 + 2] = image[(height - 1)*width * 4 + x * 4 + 2];
		new_image[(height - 1)*width * 4 + x * 4 + 3] = image[(height - 1)*width * 4 + x * 4 + 3];
	}

	unsigned offset = mask_size / 2;
	for (unsigned i = offset; i < width- offset; i++) {
		for (unsigned j = offset; j < height- offset; j++) {
			convolution_pixel(mask, mask_size, new_image, image, width, height, i, j);
		}
	}
	return new_image;
}
