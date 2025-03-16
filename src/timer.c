/*
 * timer.c
 *
 *  Created on: 2024/01/06
 *      Author: kameyamahiroki
 */
#include "iodefine.h"
#include "main.h"
#define MAX_TIMER_NUM (5)
/*
 * ワークエリア定義
 */
unsigned long *g_count_time[MAX_TIMER_NUM];//カウントエリア登録変数
typedef void (*func_ptr)(void);//登録関数へのポインタ
func_ptr func_array[MAX_TIMER_NUM];//登録配列へのポインタ配列
unsigned char g_timer_cnt;//登録したタイマ数
/*
 *
 */
/********************************************************************/
/*システムタイマー設定								 			*/
/*void  mtu0_initialize(void)										*/
/********************************************************************/
void  mtu0_initialize(void)
{
 	SYSTEM.MSTPCRA.BIT.MSTPA9	= 0;

	MTU0.TCR.BIT.TPSC			= 1;//PCLKの４分周12MHzでカウント
	MTU0.TCR.BIT.CCLR			= 1;//TGRAのコンペアマッチでTCNTクリア
	MTU0.TIER.BIT.TGIEA			= 1;//TGIA
	MTU0.TGRA					= 12000;//1msでコンペアマッチ
	//割り込みコントローラの設定
	IEN(MTU0,TGIA0)				= 1;
	IPR(MTU0,TGIA0)				= 2;
	IR(MTU0,TGIA0)				= 0;
	MTUA.TSTR.BIT.CST0			= 1;//MTU0.TCNTのカウントスタート
}

/********************************************************************/
/*DA出力用タイマ設定								 	*/
/*void  mtu1_initialize(void)										*/
/********************************************************************/
void mtu1_initialize(void)
{
	SYSTEM.MSTPCRA.BIT.MSTPA9	= 0;//?}???`?t?@???N?V?????p???X???j?b?g?i???j?b?g?O?j?X?g?b?v??????
	//?}???`?t?@???N?V?????^?C?}?p???X???j?b?g?O?`?????l??1????
	MTU1.TCR.BIT.TPSC			= 0;//48MHz
	MTU1.TCR.BIT.CCLR			= 1;//?R???y?A?}?b?`A??N???A
	MTU1.TIER.BIT.TGIEA			= 1;//TGIA?????????
	MTU1.TGRA					= 180;
	//割り込みコントローラの設定
	IEN(MTU1,TGIA1)				= 1;
	IPR(MTU1,TGIA1)				= 1;
	IR(MTU1,TGIA1)				= 0;
}

/****************************************************************************/
/*一定時間待機																*/
/*void cmt0_wait(unsigned short cnt)										*/
/*	引数：unsigned short cnt 												*/
/*		unsigned char cks 分周設定：00:8分周 01:32分周 10:128分周 11:512分周*/
/****************************************************************************/
//Excep_UndefinedInst割り込みに入って動かなくなる
//コンボ表示をして攻撃音鳴らすまではいく、モンスター名は表示されない
//宝石を消さない時は止まらない
void cmt2_wait(unsigned long cnt,unsigned char cks)
{
	MSTP(CMT2)			= 0;//モジュールストップ解除
	CMT.CMSTR1.BIT.STR2 = 0;//タイマ停止
	CMT2.CMCR.BIT.CMIE	= 1;//?R???y?A?}?b?`?????????
	CMT2.CMCR.BIT.CKS	= cks;//クロックの分周設定
	CMT2.CMCNT			= 0;
	CMT2.CMCOR			= cnt;
	IR(CMT2,CMI2)		= 0;
	IPR(CMT2,CMI2)		= 1;
	CMT.CMSTR1.BIT.STR2 = 1;//カウントダウン動作開始
	while(IR(CMT2,CMI2) == 0);
	CMT.CMSTR1.BIT.STR2 = 0;//カウントダウン動作停止
	IR(CMT2,CMI2) 		= 0;
	MSTP(CMT2)			= 1;//モジュールストップ
}
/********************************************************************/
/*マルチファンクションタイマのコンペアマッチA割り込みで呼ばれる		*/
/*void count_time(void)												*/
/********************************************************************/
//関数のポインタを使って
void count_time(void)
{
	unsigned char i;
	IEN(MTU0,TGIA0)				= 0;//割り込み禁止
	for(i = 0;i < g_timer_cnt;i++){
		(*(g_count_time[i]))--;//カウントエリアをダウンカウント
		if((*(g_count_time[i])) == 0)
			func_array[i]();
	}
	IEN(MTU0,TGIA0)				= 1;//割り込み禁止解除
}

/********************************************************************/
/*カウントするエリアとコールバック関数を登録する										*/
/*void count_timer_set(unsigned long *timer)						*/
/*	引数：unsigned long *timer 登録するエリア						*/
/*		：void func(unsigned char)  登録するコールバック関数ポインタ*/
/*	戻り値：SUCCESS又はERROR										*/
/********************************************************************/
unsigned char count_timer_set(unsigned long *timer,void func(void))
{
	unsigned char ret;
	IEN(MTU0,TGIA0)				= 0;//割り込み禁止
	if(g_timer_cnt < MAX_TIMER_NUM){
		g_count_time[g_timer_cnt]  	= timer;
		func_array[g_timer_cnt++]	= func;
		ret							= SUCCESS;
	}else
		ret							= ERROR;
	IEN(MTU0,TGIA0)				= 1;//割り込み禁止解除
	return ret;
}


/********************************************************************/
/*周期起動関数解除												*/
/*void count_timer_dell(void func(unsigned char))										*/
/********************************************************************/
void count_timer_dell(void func(void))
{
	unsigned char i;
	IEN(MTU0,TGIA0)				= 0;//割り込み禁止
	for(i = 0;i < MAX_TIMER_NUM;i++){
		if(func == func_array[i]){
			g_count_time[i]	= NULL;
//			g_count_time[i]		= 0;
			func_array[i]		= NULL;
			g_timer_cnt--;
		}
	}
	IEN(MTU0,TGIA0)				= 1;//割り込み禁止解除
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
