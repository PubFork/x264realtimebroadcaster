#include "stdafx.h"
#include "VideoDecDisplayThread.h"

package v_dec_package;

unsigned WINAPI Depackand264DecodeThread(void* pvParam)
{
// ************************************************************************************************** //
// 运行环境初始化
// 

	YUVRcvRecordFlag = FALSE;
	//接收由UDP来的包//DTP#RTP#h264
	//						  #NAL#264data
	Video_H264		h264decodTemp;
    Video_YUV420	yuvoutbuf;
	int lostNum = 0;
	int frame = 0, got_yuv = 0;

	AVCodecContext *c = NULL;
	AVFrame *picture = NULL;

	memset(&h264decodTemp, 0, sizeof(h264decodTemp));
	memset(&yuvoutbuf, 0, sizeof(yuvoutbuf));

	h264_decod_setup(&c,&picture);

//
// 初始化结束
// ************************************************************************************************** //
    printf("Running:      Depackand264DecodeThread\n");
	while( 1 ) //VideoDecFlag
	{
		static BOOL pRcvfromVideoSocketQueueEmpty;
		critical_section5.Lock();
		pRcvfromVideoSocketQueueEmpty = pSocketRcvVideoDecQueue.empty();
		critical_section5.Unlock();

		if(!pRcvfromVideoSocketQueueEmpty)	//接收队列非空则取数据进行拆包、解码工作
		{	
			critical_section5.Lock();
			h264decodTemp = pSocketRcvVideoDecQueue.front();	//从队列取出一帧
			critical_section5.Unlock();

// ************************************************************************************************** //

	// *********************************************************************************************** //
                   
			





						int forbidden_bit		= h264decodTemp.slicebuf[4] & 0x80; // 1 bit = 0
						int nal_reference_idc	= h264decodTemp.slicebuf[4] & 0x60; // 2 bit = 11
						int nal_unit_type_s		= h264decodTemp.slicebuf[4] & 0x1f; // 5 bit = SPS-7(00111);PPS-8(01000);IDR-5(00101);1(00001)

						printf("Decoding NAL_TYPE is : %d .. %x\n",nal_unit_type_s,  h264decodTemp.slicebuf[4]);

	// *********************************************************************************************** //
			while (h264decodTemp.slicelen > 0) 
			{

				got_yuv = decoder_h264_to_yuv(c, picture,
					h264decodTemp.slicebuf, h264decodTemp.slicelen,
					yuvoutbuf.YUVbuffer);





				if (got_yuv)
				{
					frame++;
					printf("decoding frame %d\n", frame);
					yuvoutbuf.sq = h264decodTemp.sq;
					yuvoutbuf.ts = h264decodTemp.ts;
					yuvoutbuf.width = c->width;
					yuvoutbuf.height = c->height;


                  /*  FILE *fp;

					fp=fopen("C:\\264decoded.yuv","ab+");

					fwrite(yuvoutbuf.YUVbuffer,1,yuvoutbuf.width*yuvoutbuf.height,fp);

					fclose(fp);*/


			// *********************************************************************************************** //

					if( VideoRecordFlag )
					{	
						printf("Video Record Thread Start!\n");
						YUVRcvRecordFlag = TRUE;
						TempLenYUVD = 16 + yuvoutbuf.width * yuvoutbuf.height * 3/2;
						memcpy(pTempYUVD, &yuvoutbuf, TempLenYUVD);
						EventRecodYUVD.SetEvent();
					}

			// *********************************************************************************************** //
					static int dwYUVForVideoOutQueueSize;
					critical_section6.Lock();
					dwYUVForVideoOutQueueSize = pVideoDecPlayQueue.size();
					critical_section6.Unlock();

					if( dwYUVForVideoOutQueueSize < 4 )     //缓冲队列长度为10帧,10*40ms=400ms
					{
						critical_section6.Lock();
						pVideoDecPlayQueue.push( yuvoutbuf );       //解码完毕的YUV数据送入队列
						critical_section6.Unlock();
					}
					else
					{
						critical_section6.Lock();
						for(int queueCount = 0; queueCount<4; queueCount++)  //删除的帧数，影响流畅程度，根据实际测试情况选择
						{
							pVideoDecPlayQueue.pop();
							lostNum++;
						}
						critical_section6.Unlock();
					}
		// *********************************************************************************************** //
				}
				else h264decodTemp.slicelen = 0;
	// *********************************************************************************************** //
			}
			EventYUVReceived.SetEvent();

			critical_section5.Lock();
			pSocketRcvVideoDecQueue.pop();			//销毁已经拆包、解码处理的帧
			critical_section5.Unlock();
// 
// ************************************************************************************************** //
		}
		Sleep(1);
	}
    avcodec_close(c);
    av_free(c);
    av_free(picture);

	printf("Thread OVER : Depackand264DecodeThread\n");
	//EventVideoDecEnd.SetEvent();
	return 0;
}





unsigned WINAPI VideoPlayThread(void* pvParam)
{
// ************************************************************************************************** //
// 运行环境初始化
// 
	BOOL firstframe = TRUE;
	int sframe = 0;

    Video_YUV420	yuvplaytemp;
	Video_YUV420	yuvplaybuf;
	memset(&yuvplaybuf, 0, sizeof(yuvplaybuf));
//
// 初始化结束
// ************************************************************************************************** //
	printf("Running:      VideoPlayThread\n");
	while( 1 )
	{
		if ( VideoPlayFlag )
		{

			static BOOL pRcvfromVideodecodQueueEmpty;
			critical_section6.Lock();
			pRcvfromVideodecodQueueEmpty = pVideoDecPlayQueue.empty();
			critical_section6.Unlock();

			if(!pRcvfromVideodecodQueueEmpty)
			{
				critical_section6.Lock();
				yuvplaytemp = pVideoDecPlayQueue.front();
				critical_section6.Unlock();

				printf("Audio ts :%d - Video ts is :%d\n",SQTScontrol.ts,yuvplaytemp.ts);

				if (SynchronousFlag)
				{

					if (firstframe)
					{
						memcpy(&yuvplaybuf, &yuvplaytemp, sizeof(Video_YUV420));
						sframe++;
						printf("playing first yuv frame %3d\n", sframe);
						YUVdisplayer(yuvplaybuf.width, yuvplaybuf.height, yuvplaybuf.YUVbuffer);
						firstframe = FALSE;

						critical_section6.Lock();
						pVideoDecPlayQueue.pop();			//销毁已经拆包、解码处理的帧
						critical_section6.Unlock();
					}


					if ( yuvplaytemp.ts < SQTScontrol.ts - TIME_STEP )
					{
						sframe++;
						critical_section6.Lock();
						pVideoDecPlayQueue.pop();			//销毁已经拆包、解码处理的帧
						critical_section6.Unlock();
					}
					else if ( yuvplaytemp.ts > SQTScontrol.ts + TIME_STEP )
					{
						printf("rp playing yuv frame %3d\n", sframe);
						YUVdisplayer(yuvplaybuf.width, yuvplaybuf.height, yuvplaybuf.YUVbuffer);
					}
					else
					{
						memcpy(&yuvplaybuf, &yuvplaytemp, sizeof(Video_YUV420));
						sframe++;
						printf("nomal playing yuv frame %3d\n", sframe);
						YUVdisplayer(yuvplaybuf.width, yuvplaybuf.height, yuvplaybuf.YUVbuffer);

						critical_section6.Lock();
						pVideoDecPlayQueue.pop();			//销毁已经拆包、解码处理的帧
						critical_section6.Unlock();
					}

				}
				else
				{
					memcpy(&yuvplaybuf, &yuvplaytemp, sizeof(Video_YUV420));
					sframe++;
					printf("playing yuv frame %3d\n", sframe);
					YUVdisplayer(yuvplaybuf.width, yuvplaybuf.height, yuvplaybuf.YUVbuffer);

					critical_section6.Lock();
					pVideoDecPlayQueue.pop();			//销毁已经拆包、解码处理的帧
					critical_section6.Unlock();
				}

			}

		}

		else
		{
			//printf("Video Play Waiting\n");
			sframe = 0;
		}

		Sleep(1);

	}

	printf("Thread OVER : VideoPlayThread\n");
	//EventVideoPlayEnd.SetEvent();
	return 0;

}