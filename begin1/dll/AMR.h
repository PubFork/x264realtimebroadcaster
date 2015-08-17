/*
* Copyright (c) 2010, Communicaition Research Centre, HIT. All rights reserved.
* 
* FILENAME：AMR.h
* DESCRIPTION:AMR类(Class)，实现AMR编码、解码功能。
* 
* HISTORY:
* Author      Time              Memo
* 何朝阳    2010-03-05        创建文件，写类结构代码
*
* 何朝阳    2010-03-06        实现成员函数，与工程结合调试通过
*/
#ifndef _AMR_H_
#define _AMR_H_

#include "./AMR_dec_enc/interf_enc.h"		//3GPP AMR编码标准C程序函数接口
#include "./AMR_dec_enc/interf_dec.h"		//3GPP AMR解码标准C程序函数接口

class AMR
{
public:
/*
 * Reserve and init. memory；设置编码速率
 */
	AMR(enum Mode My_Mode);

/*
 * Exit and free memory
 */
	~AMR(void);

/*
 * AMR编码
 */
	void AMR_Encode(short PCMFrame[160],unsigned char AMRFrame[32]);

/*
 * AMR解码
 */
	void AMR_Decode(unsigned char AMRFrame[32],short PCMFrame[160]);

private:
	int dtx;
	enum Mode LTE_Mode;
	void * enstate;
	void * destate;
};

#endif;