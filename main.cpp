#include <iostream>
#include <fstream>
#include <string>
#include "lodepng.h"
#include <vector>

#include "Image_filtering.h"

int main() {
	std::string filename = "Lenna_work.png";
	std::vector<unsigned char> image; //RGBA format
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filename);
	if (error) {
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		//system("pause");
		return 0;
	}

	float **mask;
	unsigned mask_size;
	get_mask(mask, mask_size, edge_detection_v3);
	std::vector<unsigned char> image_new = applied_mask(mask, mask_size, image, width, height);

	filename = "result.png";
	error = lodepng::encode(filename, image_new, width, height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	//system("pause");
	return 0;
}
