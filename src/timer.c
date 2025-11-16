/*
 * timer.c
 *
 *  Created on: 2024/01/06
 *      Author: kameyamahiroki
 */
#include "iodefine.h"
#include "main.h"

/*
 * ワークエリア定義
 */


/*
 *
 */


/********************************************************************/
/*DA出力用タイマ設定								 				*/
/*void  da_speaker_open(void)										*/
/********************************************************************/
void da_speaker_open(void)
{
	SYSTEM.MSTPCRA.BIT.MSTPA9	= 0;//マルチファンクションタイマパルスユニット0モジュールストップ解除
	MTU1.TCR.BIT.TPSC			= 0;//48MHz
	MTU1.TCR.BIT.CCLR			= 1;//
	MTU1.TIER.BIT.TGIEA			= 1;//TGIA割り込み許可
	MTU1.TGRA					= 180;
	//割り込みコントローラの設定
	IR(MTU1,TGIA1)				= 0;
	IEN(MTU1,TGIA1)				= 1;
	IPR(MTU1,TGIA1)				= 1;
}

/****************************************************************************/
/*一定時間待機																*/
/*void cmt0_wait(unsigned short cnt)										*/
/*	引数：unsigned short cnt 												*/
/*		unsigned char cks 分周設定：00:8分周 01:32分周 10:128分周 11:512分周*/
/****************************************************************************/
void cmt2_wait(unsigned long cnt,unsigned char cks)
{
	MSTP(CMT2)			= 0;	//モジュールストップ解除
	CMT.CMSTR1.BIT.STR2 = 0;	//タイマ停止
	CMT2.CMCR.BIT.CMIE	= 1;	//割り込み許可
	CMT2.CMCR.BIT.CKS	= cks;	//クロックの分周設定
	CMT2.CMCNT			= 0;
	CMT2.CMCOR			= cnt;
	IR(CMT2,CMI2)		= 0;
	IPR(CMT2,CMI2)		= 1;
	CMT.CMSTR1.BIT.STR2 = 1;	//カウントダウン動作開始
	while(IR(CMT2,CMI2) == 0);//割り込みが入るまで待機
	CMT.CMSTR1.BIT.STR2 = 0;	//カウントダウン動作停止
	IR(CMT2,CMI2) 		= 0;
	MSTP(CMT2)			= 1;	//モジュールストップ
}

void second_wait(unsigned long sec)
{
	unsigned long cnt,wait_num;
	wait_num = sec * 10;
	for (cnt = 0; cnt < wait_num; cnt++) {
		cmt2_wait(9375, CKS512);
	}
}

/********************************************************************/
/*乱数生成用タイマCMT1の設定										*/
/*void cmt1_initiralize(void)										*/
/********************************************************************/
void cmt1_initiralize(void)
{
	MSTP(CMT1)			= 0;//モジュールストップ状態解除
	CMT1.CMCR.BIT.CKS	= 1;//
	CMT1.CMCR.BIT.CMIE	= 0;//割り込み禁止
	CMT.CMSTR0.BIT.STR1 = 1;//カウントダウン動作開始
}

/********************************************************************/
/*乱数値取得															*/
/*unsigned short random_number_acquisition(void)					*/
/*	戻り値：unsigned char 乱数											*/
/********************************************************************/
unsigned short random_number_acquisition(void)
{
	return  CMT1.CMCNT;
}

void enable(void)
{
	__setpsw_i();//割り込み許可
}

void disable(void)
{
	__clrpsw_i();//割り込み禁止
}
