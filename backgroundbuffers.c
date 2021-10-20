
#include "backgroundbuffers.h"

u8 backgroundBuffer1[320 * 240 * 2] __attribute__((aligned(8)));
u8 backgroundBuffer2[320 * 240 * 2] __attribute__((aligned(8)));
u8 backgroundBuffer3[320 * 240 * 2] __attribute__((aligned(8)));
u8* backgroundBuffers[] = { backgroundBuffer1, backgroundBuffer2, backgroundBuffer3 };