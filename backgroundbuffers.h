
#ifndef BACKGROUNDBUFFERS_H
#define BACKGROUNDBUFFERS_H

#include <nusys.h>

#define NUMBER_OF_BACKGROUND_BUFFERS 3

extern u8 backgroundBuffer1[320 * 240 * 2] __attribute__((aligned(8)));
extern u8 backgroundBuffer2[320 * 240 * 2] __attribute__((aligned(8)));
extern u8 backgroundBuffer3[320 * 240 * 2] __attribute__((aligned(8)));
extern u8* backgroundBuffers[];


#endif