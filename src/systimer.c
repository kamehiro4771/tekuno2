/*
 * systimer.c
 *
 *  Created on: 2025/11/16
 *      Author: kameyamahiroki
 */

#include "iodefine.h"
#include <stddef.h>
#define MAX_FUNC_NUM (15)
#define MAX_AREA_NUM (15)

/*
 * ワークエリア定義
 */
unsigned long *area_array[MAX_AREA_NUM];		//カウントエリア登録変数
unsigned long func_interval_array[MAX_FUNC_NUM];//関数呼び出しの間隔配列
T_FUNC_PTR func_array[MAX_FUNC_NUM];			//関数へのポインタ配列
unsigned char function_cnt;						//登録した関数の数
unsigned char area_cnt;							//登録したタイマエリアの数
unsigned long timer_cnt_array[MAX_FUNC_NUM];	//タイマーカウント

/********************************************************************/
/*システムタイマー設定								 			*/
/*void  system_timer_open(void)										*/
/********************************************************************/
void  system_timer_open(void)
{
 	SYSTEM.MSTPCRA.BIT.MSTPA9	= 0;		//マルチファンクションタイマパルスユニット（ユニット０）のモジュールストップ解除
	MTU0.TCR.BIT.TPSC			= 1;		//PCLKの４分周でカウント
	MTU0.TCR.BIT.CCLR			= 1;		//TGRAのコンペアマッチでTCNTクリア
	MTU0.TIER.BIT.TGIEA			= 1;		//TGIA割り込み許可
	MTU0.TGRA					= 12000;	//1msでコンペアマッチ
	//割り込みコントローラの設定
	IR(MTU0,TGIA0)				= 0;
	IEN(MTU0,TGIA0)				= 1;
	IPR(MTU0,TGIA0)				= 1;
	MTUA.TSTR.BIT.CST0			= 1;		//MTU0.TCNTのカウントスタート
}

/********************************************************************/
/*マルチファンクションタイマのコンペアマッチA割り込みで呼ばれる		*/
/*void count_time(void)												*/
/********************************************************************/
void count_time(void)
{
	unsigned char i = 0;
	disable();//割り込み禁止
	for(i = 0;i < MAX_FUNC_NUM;i++){
		if(func_array[i] == NULL){
			/*nop*/
		}else{
			timer_cnt_array[i]++;
			if(func_interval_array[i] == timer_cnt_array[i]){	//カウントが呼び出し感覚と一致した
				func_array[i]();								//登録された関数呼び出し
				timer_cnt_array[i] = 0;							//タイマーカウントクリア
			}
		}
	}
	for(i = 0;i < area_cnt;i++){
		if(*(area_array[i]) != 0)
			(*(area_array[i]))--;								//カウントエリアをダウンカウント
	}
	enable();												//割り込み許可
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
	disable();																	//割り込み禁止
	if(function_cnt >= MAX_FUNC_NUM)
		ret											= ERROR;						//登録数オーバー
	else{
		//下のforループMAX_FUNC_NUMである必要があるのか
		//会社のシステムタイマと比較
		for(i = 0;i < MAX_FUNC_NUM;i++){
			if(func_array[i] == func){
				ret									= SUCCESS;						//関数が既に登録されている
			}else if(func_array[i] == NULL){
				func_interval_array[i]  			= interval;
				timer_cnt_array[i] 					= 0;
				func_array[i]						= func;
				function_cnt++;
				ret									= SUCCESS;
				break;
			}
		}
	}
	enable();																	//割り込み許可
	return ret;
}

/****************************************************************/
/* タイマカウントエリア登録												*/
/*unsigned char timer_area_registration(unsigned long* area)	*/
/*	引数：unsigned long* areaダウンカウントするエリア							*/
/*	戻り値：SUCCESS登録完了　ERROR：既に登録済み							*/
/****************************************************************/
unsigned char timer_area_registration(unsigned long* area)
{
	unsigned char ret,i;
	disable();											//割り込み禁止
	if(area_cnt >= MAX_AREA_NUM )
		ret						= ERROR;
	else{
		for(i = 0;i < area_cnt;i++){
			if(area_array[i] == area){							//既に登録済みのエリア
				break;
		}
		if(i == area_cnt){
			area_array[area_cnt++] = area;
			ret = SUCCESS;
		}
	}
	enable();//割り込み許可
	return ret;
}

/********************************************************************/
/*周期起動関数削除													*/
/*void interval_function_del(void func(unsigned char))					*/
/*	引数：削除する関数のポインタ									*/
/********************************************************************/
void interval_function_del(void func(void))
{
	unsigned char i;
	disable();//割り込み禁止
	for(i = 0;i < MAX_FUNC_NUM;i++){
		if(func == func_array[i]){
			function_cnt--;
//			g_count_time[i]		= 0;
			func_array[i]		= NULL;
		}
	}
	enable();//割り込み許可
}
