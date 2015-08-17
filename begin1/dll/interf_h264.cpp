#include "stdafx.h"
#include "interf_h264.h"


void h264_decod_setup(AVCodecContext **cout, AVFrame **pictureout)
{
    AVCodec *codec = &h264_decoder;
	AVCodecContext *c;
	AVFrame *picture;

    avcodec_init();
	c = avcodec_alloc_context();
    picture = avcodec_alloc_frame();

    if(codec->capabilities & CODEC_CAP_TRUNCATED)
		c->flags |= CODEC_FLAG_TRUNCATED;

    if (avcodec_open(c, codec) < 0)
	{
        fprintf(stderr, "could not open codec\n");
        exit(1);
	}
	else
	{
		H264Context *h = (H264Context*)c->priv_data;
		MpegEncContext *s = &h->s;
		s->dsp.idct_permutation_type = 1;
		dsputil_init(&s->dsp, c);
	}

	*cout = c;
	*pictureout = picture;

}


int decoder_h264_to_yuv(AVCodecContext *c, AVFrame *picture,
						unsigned char *h264frame, int h264len,
						unsigned char *yuvframe)
{
	int i, size, got_picture, len, yuvTemplen;
	unsigned char *inbuf_ptr;
	char *yuvTemp;

	yuvTemp = (char*)calloc(60000, sizeof(char));
	if (yuvTemp == NULL)
	{
		perror("calloc error£º");
		exit(0);
	}

	yuvTemplen = 0;

	inbuf_ptr = h264frame;
	size = h264len;

	len = avcodec_decode_video(c, picture, &got_picture,
		inbuf_ptr, size);

	if (len < 0) 
	{
		fprintf(stderr, "Error while decoding frame \n");
		exit(1);
	}
	printf("size =%d; len =%d; got_picture=%d\n", size, len, got_picture);

	if (got_picture) 
	{

		for(i=0; i<c->height; i++)
			memcpy(yuvTemp + i*c->width, picture->data[0] + i*picture->linesize[0], c->width);
		yuvTemplen += c->width*c->height;

		for(i=0; i<c->height/2; i++)
			memcpy(yuvTemp + yuvTemplen + i*c->width/2, picture->data[1] + i*picture->linesize[1], c->width/2);
		yuvTemplen += c->width*c->height/4;

		for(i=0; i<c->height/2; i++)
			memcpy(yuvTemp + yuvTemplen + i*c->width/2, picture->data[2] + i*picture->linesize[2], c->width/2);
		yuvTemplen += c->width*c->height/4;

		memcpy(yuvframe, yuvTemp, yuvTemplen);

	}

	free(yuvTemp);
	return got_picture;
}

