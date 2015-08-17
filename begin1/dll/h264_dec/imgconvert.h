#ifdef __cplusplus
extern "C" {
#endif


#ifndef FFMPEG_IMGCONVERT_H
#define FFMPEG_IMGCONVERT_H

#include "avcodec.h"

int ff_fill_linesize(AVPicture *picture, int pix_fmt, int width);

int ff_fill_pointer(AVPicture *picture, uint8_t *ptr, int pix_fmt, int height);

int ff_get_plane_bytewidth(enum PixelFormat pix_fmt, int width, int plane);

#endif

#ifdef __cplusplus
}
#endif