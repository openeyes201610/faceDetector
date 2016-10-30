#ifndef _FACE_DETECTOR_H_
#define _FACE_DETECTOR_H_

#include <vector>
#include <stdint.h>


typedef struct {
    int x, y;
    int width, height;
} Rect;


/****************************************
 * load model
 * filePath: ../face_detector_model.bin
 ****************************************/
int load_model(const char *filePath);

/***************************************
 * set detect parameter
 * minScale: factor of min face 
 * maxScale: factor of max face 
 * layers: pyramid layers 
 * offset: offset factor
 ***************************************/
void set_detect_params(float minScale, float maxScale, int layers, float offset);

/****************************************
 * detect face on image
 * img: gray image data, gray
 * width: image width
 * height: image height
 * stride: image memory width
 * rects: detect result
 ****************************************/
void detect_face(uint8_t *img, int width, int height, int stride, std::vector<Rect> &rects);


/****************************************
 * release data, after use the library
 ****************************************/
void release_data();

#endif
