/*
 * flash.h
 *
 *  Created on: 2025/03/25
 *      Author: kameyamahiroki
 */

#ifndef FLASH_H_
#define FLASH_H_
unsigned short e2_blank_check(void);
#pragma address e2_FLASH = 0x00100000
#pragma address FCU_RAM = 0x007f8000
#pragma address FCU_FIRM_WARE = 0xfeffe000
unsigned char fcu_initialize(void);
#endif /* FLASH_H_ */
