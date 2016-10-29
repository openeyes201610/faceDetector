#ifndef _FACE_DETECTOR_H_
#define _FACE_DETECTOR_H_

#include <vector>
#include <stdint.h>


typedef struct {
    int x, y;
    int width, height;
} Rect;


int load_model(const char *filePath);
void detect_face(uint8_t *img, int width, int height, int stride, std::vector<Rect> &rects);
void release_data();

#endif
