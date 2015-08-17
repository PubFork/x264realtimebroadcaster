#include "stdafx.h"
#include "AMR.h"

/*
* Function Name:
*		AMR
*
* Description: 
*		���캯�����ڴ��ʼ����������������
*		���ʼ���﷨��
*		AMR m_AMR(MR475); //����һ��4.75���ʵ�AMR����
*
* Parameter(s):
*       m_mode           : �û���������
*
* Return:
*		��
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
*		���AMR���빦��
*
* Parameter(s):
*		PCMFrame                 : PCM֡            
*		AMRFrame                 : AMR֡
*
* Return:
*		��
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
*		���AMR���빦��
*
* Parameter(s):
*		AMRFrame                 : AMR֡
*		PCMFrame                 : PCM֡            
*
* Return:
*		��
*/
void AMR::AMR_Decode(unsigned char AMRFrame[32],short PCMFrame[160])
{
	Decoder_Interface_Decode(destate, AMRFrame, PCMFrame, 0);
}