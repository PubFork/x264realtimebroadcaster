#include "stdafx.h"
#include "AMR.h"

/*
* Function Name:
*		AMR
*
* Description: 
*		构造函数。内存初始化；编码速率设置
*		类初始化语法：
*		AMR m_AMR(MR475); //创建一个4.75速率的AMR对象
*
* Parameter(s):
*       m_mode           : 用户设置速率
*
* Return:
*		无
*/
AMR::AMR(enum Mode My_Mode)
{
	dtx=0;
	enstate = Encoder_Interface_init(dtx);
	destate = Decoder_Interface_init();
	///////////////////////////////////////////////2012.5.4//////////////////////////////////////////////////
    //My_Mode=MR475;
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	LTE_Mode = My_Mode;
}

AMR::~AMR(void)
{
	Encoder_Interface_exit(enstate);
	Decoder_Interface_exit(destate);
}

/*
* Function Name:
*		AMR_Encode
*
* Description: 
*		完成AMR编码功能
*
* Parameter(s):
*		PCMFrame                 : PCM帧            
*		AMRFrame                 : AMR帧
*
* Return:
*		无
*/
void AMR::AMR_Encode(short PCMFrame[160],unsigned char AMRFrame[32])
{
	Encoder_Interface_Encode(enstate, LTE_Mode, PCMFrame, AMRFrame,0);
}

/*
* Function Name:
*		AMR_Decode
*
* Description: 
*		完成AMR解码功能
*
* Parameter(s):
*		AMRFrame                 : AMR帧
*		PCMFrame                 : PCM帧            
*
* Return:
*		无
*/
void AMR::AMR_Decode(unsigned char AMRFrame[32],short PCMFrame[160])
{
	Decoder_Interface_Decode(destate, AMRFrame, PCMFrame, 0);
}