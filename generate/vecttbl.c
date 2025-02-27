/************************************************************************
*
* Device     : RX/RX600/RX621,RX62N
*
* File Name  : vecttbl.c
*
* Abstract   : Initialize of Vector Table.
*
* History    : 1.00 (2010-03-05)  [Hardware Manual Revision : 0.20]
*            : 1.10 (2011-02-21)  [Hardware Manual Revision : 0.50]
*            : 1.11 (2011-06-20)  [Hardware Manual Revision : 1.00]
*            : 1.20 (2013-02-18)  [Hardware Manual Revision : 1.30]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2013 (2010,2011) Renesas Electronics Corporation. and
* Renesas Solutions Corp. All rights reserved.
*
************************************************************************/

#include "vect.h"

typedef void (*fp) (void);

#pragma section C FIXEDVECT

void (*const Fixed_Vectors[])(void) = {
//;0xffffff80  
    Dummy,
//;0xffffff84  Reserved
    Dummy,    
//;0xffffff88  Reserved
    Dummy,
//;0xffffff8C  Reserved
    Dummy,
//;0xffffff90  Reserved
    Dummy,    
//;0xffffff94  Reserved
    Dummy,    
//;0xffffff98  Reserved
    Dummy,   
//;0xffffff9C  Reserved
    Dummy,    
//;0xffffffA0  Reserved
// ID codes (Default)
   (fp)0xFFFFFFFF,
 //;0xffffffA4  Reserved
   (fp)0xFFFFFFFF,
//;0xffffffA8  Reserved
   (fp)0xFFFFFFFF,
//;0xffffffAC  Reserved
   (fp)0xFFFFFFFF,
//;0xffffffB0  Reserved
    Dummy,
//;0xffffffB4  Reserved
    Dummy,
//;0xffffffB8  Reserved
    Dummy,
//;0xffffffBC  Reserved
    Dummy,
//;0xffffffC0  Reserved
    Dummy,
//;0xffffffC4  Reserved
    Dummy,
//;0xffffffC8  Reserved
    Dummy,
//;0xffffffCC  Reserved
    Dummy,
//;0xffffffd0  Exception(Supervisor Instruction)
    Excep_SuperVisorInst,
//;0xffffffd4  Exception(Access Instruction)
    Excep_AccessInst,
//;0xffffffd8  Reserved
    Dummy,
//;0xffffffdc  Exception(Undefined Instruction)
    Excep_UndefinedInst,
//;0xffffffe0  Reserved
    Dummy,
//;0xffffffe4  Exception(Floating Point)
    Excep_FloatingPoint,
//;0xffffffe8  Reserved
    Dummy,
//;0xffffffec  Reserved
    Dummy,
//;0xfffffff0  Reserved
    Dummy,
//;0xfffffff4  Reserved
    Dummy,
//;0xfffffff8  NMI
    NonMaskableInterrupt,
//;0xfffffffc  RESET
//;<<VECTOR DATA START (POWER ON RESET)>>
//;Power On Reset PC
    /*(void*)*/ PowerON_Reset_PC                                                                                                                 
//;<<VECTOR DATA END (POWER ON RESET)>>
};


