#pragma once
#include <vector>
using namespace std;

enum kernel{ 
	edge_detection_v1, 
	edge_detection_v2, 
	edge_detection_v3, 
	sharpen, 
	box_blur, 
	gaussian_blur_s3, 
	gaussian_blur_s5
};

void get_mask(float **&out, unsigned &out_size, enum kernel type);

vector<unsigned char> applied_mask(
	float **mask, unsigned mask_size, 
	vector<unsigned char> image, unsigned width, unsigned height);

const float mask_edge_detection_v1[3][3] = {
	{0,  1, 0},
	{1, -4, 1},
	{0,  1, 0}
};

const float mask_edge_detection_v2[3][3] = {
	{ 1, 0 ,-1},
	{ 0, 0,  0},
	{-1, 0,  1}
};

const float mask_edge_detection_v3[3][3] = {
	{-1, -1, -1},
	{-1,  8, -1},
	{-1, -1, -1}
};

const float mask_sharpen[3][3] = {
	{ 0, -1,  0},
	{-1,  5, -1},
	{ 0, -1,  0}
};

const float mask_box_blur[3][3] = {
	{1.f/9,  1.f/9, 1.f/9},
	{1.f/9,  1.f/9, 1.f/9},
	{1.f/9,  1.f/9, 1.f/9}
};

const float mask_gaussian_blur_s3[3][3] = {
	{1.f/16, 1.f/8, 1.f/16},
	{1.f/8,  1.f/4, 1.f/8 },
	{1.f/16, 1.f/8, 1.f/16}
};

const float mask_gaussian_blur_s5[5][5] ={
	{1.f/256,  4.f/256,  6.f/256,  4.f/256, 1.f/256},
	{4.f/256, 16.f/256, 24.f/256, 16.f/256, 4.f/256},
	{6.f/256, 24.f/256, 36.f/256, 24.f/256, 6.f/256},
	{4.f/256, 16.f/256, 24.f/256, 16.f/256, 4.f/256},
	{1.f/256,  4.f/256,  6.f/256,  4.f/256, 1.f/256}
};
