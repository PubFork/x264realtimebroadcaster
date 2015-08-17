/*
* Copyright (c) 2010, Communicaition Research Centre, HIT. All rights reserved.
* 
* FILENAME��AMR.h
* DESCRIPTION:AMR��(Class)��ʵ��AMR���롢���빦�ܡ�
* 
* HISTORY:
* Author      Time              Memo
* �γ���    2010-03-05        �����ļ���д��ṹ����
*
* �γ���    2010-03-06        ʵ�ֳ�Ա�������빤�̽�ϵ���ͨ��
*/
#ifndef _AMR_H_
#define _AMR_H_

#include "./AMR_dec_enc/interf_enc.h"		//3GPP AMR�����׼C�������ӿ�
#include "./AMR_dec_enc/interf_dec.h"		//3GPP AMR�����׼C�������ӿ�

class AMR
{
public:
/*
 * Reserve and init. memory�����ñ�������
 */
	AMR(enum Mode My_Mode);

/*
 * Exit and free memory
 */
	~AMR(void);

/*
 * AMR����
 */
	void AMR_Encode(short PCMFrame[160],unsigned char AMRFrame[32]);

/*
 * AMR����
 */
	void AMR_Decode(unsigned char AMRFrame[32],short PCMFrame[160]);

private:
	int dtx;
	enum Mode LTE_Mode;
	void * enstate;
	void * destate;
};

#endif;