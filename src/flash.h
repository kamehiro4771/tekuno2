/*
 * flash.h
 *
 *  Created on: 2025/03/25
 *      Author: kameyamahiroki
 */

#ifndef FLASH_H_
#define FLASH_H_
unsigned char e2_blank_check(unsigned short block_adress);
#pragma address e2_FLASH = 0x00100000
#pragma address FCU_RAM = 0x007f8000
#pragma address FCU_FIRM_WARE = 0xfeffe000
#define BLOCK_NUM (16)
unsigned char fcu_initialize(void);
unsigned char e2data_erase(unsigned short erase_address);
unsigned char e2_writing(unsigned short offset,void *write_data,unsigned int word_count);
#endif /* FLASH_H_ */
