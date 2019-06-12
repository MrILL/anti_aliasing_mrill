#pragma once
#include <vector>
#include "filter_mask.h"

void get_mask(float **&out_mask, unsigned &mask_size, kernel type);

std::vector<unsigned char> applied_mask(
	float **mask, unsigned mask_size, 
	std::vector<unsigned char> image, unsigned width, unsigned height);
