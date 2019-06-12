#pragma once

typedef enum {
	edge_detection_v1,
	edge_detection_v2,
	edge_detection_v3,
	sharpen,
	box_blur,
	gaussian_blur_s3,
	gaussian_blur_s5
} kernel;

extern const float mask_edge_detection_v1[3][3];
extern const float mask_edge_detection_v2[3][3];
extern const float mask_edge_detection_v3[3][3];

extern const float mask_sharpen[3][3];

extern const float mask_box_blur[3][3];

extern const float mask_gaussian_blur_s3[3][3];

extern const float mask_gaussian_blur_s5[5][5];
