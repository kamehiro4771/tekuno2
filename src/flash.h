/*
 * flash.h
 *
 *  Created on: 2025/03/25
 *      Author: kameyamahiroki
 */

#ifndef FLASH_H_
#define FLASH_H_

unsigned char e2_blank_check(void);
#pragma address e2_FLASH = 0x00100000
#endif /* FLASH_H_ */
