
#ifndef _interf_h264_H_
#define _interf_h264_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <memory.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "./h264_dec/h264.h"
#include "./h264_dec/avcodec.h"



extern AVCodec h264_decoder;

void h264_decod_setup(AVCodecContext **cout, AVFrame **pictureout);

int decoder_h264_to_yuv(AVCodecContext *c, AVFrame *picture,
						unsigned char *h264frame, int h264len,
						unsigned char *yuvframe);

#ifdef __cplusplus
}
#endif

#endif;
