/*
 * MSMPEG4 backend for ffmpeg encoder and decoder
 * copyright (c) 2007 Aurelien Jacobs <aurel@gnuage.org>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file msmpeg4.h
 */
#ifdef __cplusplus
extern "C" {
#endif


#ifndef FFMPEG_MSMPEG4_H
#define FFMPEG_MSMPEG4_H

//#include "config.h"
#include "avcodec.h"
#include "dsputil.h"
#include "mpegvideo.h"

#define INTER_INTRA_VLC_BITS 3
#define MB_NON_INTRA_VLC_BITS 9
#define MB_INTRA_VLC_BITS 9

extern VLC ff_mb_non_intra_vlc[4];
extern VLC ff_inter_intra_vlc;

void ff_msmpeg4_code012(PutBitContext *pb, int n);
void ff_msmpeg4_encode_block(MpegEncContext * s, DCTELEM * block, int n);
void ff_msmpeg4_handle_slices(MpegEncContext *s);
void ff_msmpeg4_encode_motion(MpegEncContext * s, int mx, int my);
int ff_msmpeg4_coded_block_pred(MpegEncContext * s, int n,
                                uint8_t **coded_block_ptr);
int ff_msmpeg4_decode_motion(MpegEncContext * s, int *mx_ptr, int *my_ptr);
int ff_msmpeg4_decode_block(MpegEncContext * s, DCTELEM * block,
                            int n, int coded, const uint8_t *scan_table);
int ff_wmv2_decode_mb(MpegEncContext *s, DCTELEM block[6][64]);

#define ENABLE_MSMPEG4_DECODER (ENABLE_MSMPEG4V1_DECODER || \
                                ENABLE_MSMPEG4V2_DECODER || \
                                ENABLE_MSMPEG4V3_DECODER || \
                                ENABLE_WMV2_DECODER)
#define ENABLE_MSMPEG4_ENCODER (ENABLE_MSMPEG4V1_ENCODER || \
                                ENABLE_MSMPEG4V2_ENCODER || \
                                ENABLE_MSMPEG4V3_ENCODER || \
                                ENABLE_WMV2_ENCODER)
#define ENABLE_MSMPEG4 (ENABLE_MSMPEG4_DECODER || ENABLE_MSMPEG4_ENCODER)
#define ENABLE_WMV2 (ENABLE_WMV2_DECODER || ENABLE_WMV2_ENCODER)
#define ENABLE_WMV_DECODER (ENABLE_WMV1_DECODER || ENABLE_WMV2_DECODER)
#define ENABLE_WMV_ENCODER (ENABLE_WMV1_ENCODER || ENABLE_WMV2_ENCODER)

#endif /* FFMPEG_MSMPEG4_H */

#ifdef __cplusplus
}
#endif