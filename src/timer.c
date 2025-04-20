/*
 * timer.c
 *
 *  Created on: 2024/01/06
 *      Author: kameyamahiroki
 */
#include "iodefine.h"
#include "main.h"
#define MAX_FUNC_NUM (5)
#define MAX_AREA_NUM (5)
/*
 * ワークエリア定義
 */
unsigned long *area_array[MAX_AREA_NUM];//カウントエリア登録変数
typedef void (*func_ptr)(void);//登録関数へのポインタ
unsigned long func_interval_array[MAX_FUNC_NUM];//関数呼び出しの間隔配列
func_ptr func_array[MAX_FUNC_NUM];//登録配列へのポインタ配列
unsigned char function_cnt;//登録した関数の数
unsigned char area_cnt;//登録したタイマエリアの数
unsigned long timer_cnt_array[MAX_FUNC_NUM];//タイマーカウント
/*
 *
 */
/********************************************************************/
/*システムタイマー設定								 			*/
/*void  mtu0_initialize(void)										*/
/********************************************************************/
void  mtu0_initialize(void)
{
 	SYSTEM.MSTPCRA.BIT.MSTPA9	= 0;		//マルチファンクションタイマパルスユニット（ユニット０）のモジュールストップ解除
	MTU0.TCR.BIT.TPSC			= 1;		//PCLKの４分周でカウント
	MTU0.TCR.BIT.CCLR			= 1;		//TGRAのコンペアマッチでTCNTクリア
	MTU0.TIER.BIT.TGIEA			= 1;		//TGIA割り込み許可
	MTU0.TGRA					= 12000;	//1msでコンペアマッチ
	//割り込みコントローラの設定
	IEN(MTU0,TGIA0)				= 1;
	IPR(MTU0,TGIA0)				= 1;
	IR(MTU0,TGIA0)				= 0;
	MTUA.TSTR.BIT.CST0			= 1;		//MTU0.TCNTのカウントスタート
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
/*		unsigned char cks 分周設定：00:8分周 01:32分周 10:128分周 11:512分周		*/
/****************************************************************************/
void cmt2_wait(unsigned long cnt,unsigned char cks)
{
	MSTP(CMT2)			= 0;//モジュールストップ解除
	CMT.CMSTR1.BIT.STR2 = 0;//タイマ停止
	CMT2.CMCR.BIT.CMIE	= 1;//割り込み許可
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
void count_time(void)
{
	unsigned char i = 0;
	__clrpsw_i();//割り込み禁止
	for(i = 0;i < MAX_FUNC_NUM;i++){
		if(func_array[i] == NULL){
			/*nop*/
		}else{
			timer_cnt_array[i]++;
			if(func_interval_array[i] == timer_cnt_array[i]){//カウントが呼び出しカウントと一致した
				func_array[i]();//登録された関数呼び出し
				timer_cnt_array[i] = 0;//タイマーカウントクリア
			}
		}
	}
	for(i = 0;i < area_cnt;i++){
		if(*(area_array[i]) != 0)
			(*(area_array[i]))--;//カウントエリアをダウンカウント
	}
	__setpsw_i();//割り込み許可
}

/********************************************************************************/
/*タイマカウントで呼び出される関数を登録する									*/
/*unsigned char interval_function_set(unsigned long interval,void func(void))	*/
/*	引数：unsigned long interval 	呼び出す周期(ms)							*/
/*		：void func(unsigned char)  登録するコールバック関数ポインタ			*/
/*	戻り値：SUCCESS又はERROR													*/
/********************************************************************************/
unsigned char interval_function_set(unsigned long interval,void func(void))
{
	unsigned char ret,i;
	__clrpsw_i();											//割り込み禁止
	if(function_cnt < MAX_FUNC_NUM){
		for(i = 0;i < MAX_FUNC_NUM;i++){
			if(func_array[i] == func){
				__setpsw_i();											//割り込み許可
				return SUCCESS;								//関数が既に登録されている
			}
		}
		func_interval_array[function_cnt]  	= interval;
		func_array[function_cnt++]			= func;
		ret									= SUCCESS;
	}else
		ret									= ERROR;		//登録数オーバー
	__setpsw_i();											//割り込み許可
	return ret;
}

/****************************************************************/
/* タイマカウントエリア登録										*/
/*unsigned char timer_area_registration(unsigned long* area)	*/
/*	引数：unsigned long* areaダウンカウントするエリア			*/
/*	戻り値：SUCCESS登録完了　ERROR：既に登録済み				*/
/****************************************************************/
unsigned char timer_area_registration(unsigned long* area)
{
	unsigned char ret,i;
	__clrpsw_i();											//割り込み禁止
	for(i = 0;i < area_cnt;i++){
		if(area_array[i] == area)							//既に登録済みのエリア
			break;
	}
	if(i == area_cnt){
		area_array[area_cnt++] = area;
		ret = SUCCESS;
	}else
		ret = ERROR;
	__setpsw_i();//割り込み許可
	return ret;
}
/********************************************************************/
/*周期起動関数削除													*/
/*void count_timer_dell(void func(unsigned char))					*/
/*	引数：削除する関数のポインタ									*/
/********************************************************************/
void count_timer_dell(void func(void))
{
	unsigned char i;
	__clrpsw_i();//割り込み禁止
	for(i = 0;i < MAX_FUNC_NUM;i++){
		if(func == func_array[i]){
			function_cnt--;
//			g_count_time[i]		= 0;
			func_array[i]		= NULL;
		}
	}
	__setpsw_i();//割り込み許可
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
