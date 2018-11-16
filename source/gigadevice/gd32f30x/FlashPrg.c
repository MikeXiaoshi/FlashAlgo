/* -----------------------------------------------------------------------------
 * Copyright (c) 2015 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software. Permission is granted to anyone to use this 
 * software for any purpose, including commercial applications, and to alter 
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be 
 *    appreciated but is not required. 
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be 
 *    misrepresented as being the original software. 
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *   
 *
 * $Date:        23. April 2015
 * $Revision:    V1.00
 *  
 * Project:      Flash Programming Functions for GigaDevice GD32F30x Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "flashOS.H"                     

typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

typedef unsigned short             int uint16_t;
typedef unsigned                   int uint32_t;

unsigned long    base_adr;

#define REG32(addr)                (*(volatile uint32_t *)(uint32_t)(addr)) 
#define BIT(x)                     ((uint32_t)((uint32_t)0x01U<<(x)))

// Peripheral Memory Map
#define FWDGT_BASE                 0x40003000
#define FMC_BASE                   0x40022000

#define FWDGT                      FWDGT_BASE
#define FMC                        FMC_BASE


//FWDGT
#define FWDGT_CTL                  REG32((FWDGT) + 0x00U)         /*!< FWDGT control register */
#define FWDGT_PSC                  REG32((FWDGT) + 0x04U)         /*!< FWDGT prescaler register */
#define FWDGT_RLD                  REG32((FWDGT) + 0x08U)         /*!< FWDGT reload register */
#define FWDGT_STAT                 REG32((FWDGT) + 0x0CU)         /*!< FWDGT status register */

// FMC
#define FMC_WS                     REG32((FMC) + 0x00U)           /*!< FMC wait state register */
#define FMC_KEY0                   REG32((FMC) + 0x04U)           /*!< FMC unlock key register 0 */
#define FMC_OBKEY                  REG32((FMC) + 0x08U)           /*!< FMC option bytes unlock key register */
#define FMC_STAT0                  REG32((FMC) + 0x0CU)           /*!< FMC status register 0 */
#define FMC_CTL0                   REG32((FMC) + 0x10U)           /*!< FMC control register 0 */
#define FMC_ADDR0                  REG32((FMC) + 0x14U)           /*!< FMC address register 0 */
#define FMC_OBSTAT                 REG32((FMC) + 0x1CU)           /*!< FMC option bytes status register */
#define FMC_WP                     REG32((FMC) + 0x20U)           /*!< FMC erase/program protection register */
#if defined GD32F30X_XD || defined GD32F30X_CL
#define FMC_KEY1                   REG32((FMC) + 0x44U)           /*!< FMC unlock key register 1 */
#define FMC_STAT1                  REG32((FMC) + 0x4CU)           /*!< FMC status register 1 */
#define FMC_CTL1                   REG32((FMC) + 0x50U)           /*!< FMC control register 1 */
#define FMC_ADDR1                  REG32((FMC) + 0x54U)           /*!< FMC address register 1 */
#endif


// FMC Control Register definitions
#define FMC_CTL0_PG                BIT(0)                         /*!< main flash program for bank0 command bit */
#define FMC_CTL0_PER               BIT(1)                         /*!< main flash page erase for bank0 command bit */
#define FMC_CTL0_MER               BIT(2)                         /*!< main flash mass erase for bank0 command bit */
#define FMC_CTL0_OBPG              BIT(4)                         /*!< option bytes program command bit */
#define FMC_CTL0_OBER              BIT(5)                         /*!< option bytes erase command bit */
#define FMC_CTL0_START             BIT(6)                         /*!< send erase command to FMC bit */
#define FMC_CTL0_LK                BIT(7)                         /*!< FMC_CTL0 lock bit */
#define FMC_CTL0_OBWEN             BIT(9)                         /*!< option bytes erase/program enable bit */
#define FMC_CTL0_ERRIE             BIT(10)                        /*!< error interrupt enable bit */
#define FMC_CTL0_ENDIE             BIT(12)                        /*!< end of operation interrupt enable bit */

#define FMC_CTL1_PG                BIT(0)                         /*!< main flash program for bank1 command bit */
#define FMC_CTL1_PER               BIT(1)                         /*!< main flash page erase for bank1 command bit */
#define FMC_CTL1_MER               BIT(2)                         /*!< main flash mass erase for bank1 command bit */
#define FMC_CTL1_START             BIT(6)                         /*!< send erase command to FMC bit */
#define FMC_CTL1_LK                BIT(7)                         /*!< FMC_CTL1 lock bit */
#define FMC_CTL1_ERRIE             BIT(10)                        /*!< error interrupt enable bit */
#define FMC_CTL1_ENDIE             BIT(12)                        /*!< end of operation interrupt enable bit */

// FMC Status Register definitions
#define FMC_STAT0_BUSY             BIT(0)                         /*!< flash busy flag bit */
#define FMC_STAT0_PGERR            BIT(2)                         /*!< flash program error flag bit */
#define FMC_STAT0_WPERR            BIT(4)                         /*!< erase/program protection error flag bit */
#define FMC_STAT0_ENDF             BIT(5)                         /*!< end of operation flag bit */

#define FMC_STAT1_BUSY             BIT(0)                         /*!< flash busy flag bit */
#define FMC_STAT1_PGERR            BIT(2)                         /*!< flash program error flag bit */
#define FMC_STAT1_WPERR            BIT(4)                         /*!< erase/program protection error flag bit */
#define FMC_STAT1_ENDF             BIT(5)                         /*!< end of operation flag bit */

// FMC Keys
#define RDPT_KEY                   0x5AA5
#define UNLOCK_KEY0                ((uint32_t)0x45670123U)        /*!< unlock key 0 */
#define UNLOCK_KEY1                ((uint32_t)0xCDEF89ABU)        /*!< unlock key 1 */

// FMC BANK size
#define BANK1_SIZE                 0x00080000             



#ifdef FMC_OB

int Init(unsigned long adr, unsigned long clk, unsigned long fnc)
{
    FMC_WS  = 0x00000000;                            // Zero Wait State
     
    FMC_KEY0 = UNLOCK_KEY0;                          // Unlock FMC 
    FMC_KEY0 = UNLOCK_KEY1;

    FMC_OBKEY = UNLOCK_KEY0;                         // Unlock Option Bytes
    FMC_OBKEY = UNLOCK_KEY1;
                                                     // Test if FWDGT is running (FWDGT in HW mode)
    if((FMC_OBSTAT & 0x04) == 0x00){   
        FWDGT_CTL  = 0x5555;                         // Enable write access to FWDGT_PSC and FWDGT_RLD     
        FWDGT_PSC  = 0x06;                           // Set prescaler to 256  
        FWDGT_RLD = 4095;                            // Set reload value to 4095
    }

    return(0);
}
#endif

#ifdef FMC_PE

int Init(unsigned long adr, unsigned long clk, unsigned long fnc)
{
    base_adr = adr & ~(BANK1_SIZE - 1);              // Align to Size Boundary
    FMC_WS  = 0x00000000;                            // Zero Wait State
    FMC_KEY0  = UNLOCK_KEY0;                         // Unlock FMC 
    FMC_KEY0  = UNLOCK_KEY1;

#if defined GD32F30x_XD || defined GD32F30x_CL
    FMC_KEY1  = UNLOCK_KEY0;                         // Unlock FMC Bank2
    FMC_KEY1  = UNLOCK_KEY1;
#endif

                                                     // Test if FWDGT is running (FWDGT in HW mode)
    if((FMC_OBSTAT & 0x04) == 0x00){
        FWDGT_CTL  = 0x5555;                         // Enable write access to FWDGT_PSC and FWDGT_RLD     
        FWDGT_PSC  = 0x06;                           // Set prescaler to 256  
        FWDGT_RLD = 4095;                            // Set reload value to 4095
    }

    return(0);
} 
#endif



#ifdef FMC_OB
int UnInit(unsigned long fnc)
{
    FMC_CTL0 |=  FMC_CTL0_LK;
    FMC_CTL0 &= ~FMC_CTL0_OBWEN;                      // Lock FMC & Option Bytes
    return(0);
}
#endif

#ifdef FMC_PE
int UnInit(unsigned long fnc) 
{
    FMC_CTL0  |=  FMC_CTL0_LK;                        // Lock FMC
#if defined GD32F30X_XD || defined GD32F30X_CL
    FMC_CTL1  |=  FMC_CTL1_LK;                        // Lock FMC
#endif
    return(0);
} 
#endif



#ifdef FMC_OB
int EraseChip(void)
{
    FMC_CTL0 |=  FMC_CTL0_OBER;                       // Option Byte Erase Enabled 
    FMC_CTL0 |=  FMC_CTL0_START;                      // Start Erase

    while(FMC_STAT0 & FMC_STAT0_BUSY){
        FWDGT_CTL = 0xAAAA;                           // Reload FWDGT
    }

    FMC_CTL0 &= ~FMC_CTL0_OBER;                       // Option Byte Erase Disabled 
                                                      // Unprotect FMC
    FMC_CTL0 |=  FMC_CTL0_OBPG;                       // Option Byte Programming Enabled

    M16(0x1FFFF800) = RDPT_KEY;                       // Program Half Word: RDPRT Key
    while(FMC_STAT0 & FMC_STAT0_BUSY){
        FWDGT_CTL = 0xAAAA;                           // Reload FWDGT
    }

    FMC_CTL0 &= ~FMC_CTL0_OBPG;                       // Option Byte Programming Disabled

                                                      // Check for Errors
    if(FMC_STAT0 & (FMC_STAT0_PGERR | FMC_STAT0_WPERR)){
        FMC_STAT0 |= FMC_STAT0_PGERR | FMC_STAT0_WPERR;
        return(1);                                    // Failed
    }  
    return(0);                                        // Done
}
#endif

#ifdef FMC_PE
int EraseChip(void)
{
    FMC_CTL0  |=  FMC_CTL0_MER;                       // Mass Erase Enabled
    FMC_CTL0  |=  FMC_CTL0_START;                     // Start Erase

    while(FMC_STAT0  & FMC_STAT0_BUSY){
        FWDGT_CTL = 0xAAAA;                           // Reload FWDGT
    }

    FMC_CTL0  &= ~FMC_CTL0_MER;                       // Mass Erase Disabled

#if defined GD32F30X_XD || defined GD32F30X_CL        //Erase Bank2

    FMC_CTL1  |=  FMC_CTL1_MER;                       // Mass Erase Enabled
    FMC_CTL1  |=  FMC_CTL1_START;                     // Start Erase

    while (FMC_STAT1  & FMC_STAT1_BUSY){
        FWDGT_CTL = 0xAAAA;                           // Reload FWDGT
    }

    FMC_CTL1  &= ~FMC_CTL1_MER;                       // Mass Erase Disabled
#endif

    return(0);                                        // Done
}

#endif

#ifdef FMC_OB

int EraseSector(unsigned long adr)
{
    FMC_CTL0 |=  FMC_CTL0_OBER;                       // Option Byte Erase Enabled 
    FMC_CTL0 |=  FMC_CTL0_START;                      // Start Erase

    while(FMC_STAT0 & FMC_STAT0_BUSY){
        FWDGT_CTL = 0xAAAA;                           // Reload FWDGT
    }

    FMC_CTL0 &= ~FMC_CTL0_OBER;                       // Option Byte Erase Disabled 

    return (0);                                       // Done
}
#endif

#ifdef FMC_PE
int EraseSector(unsigned long adr)
{
#if defined GD32F30X_XD || defined GD32F30X_CL
    if(adr < (base_adr + BANK1_SIZE)){                 // Flash bank 2
#endif
        FMC_CTL0  |=  FMC_CTL0_PER;                    // Page Erase Enabled 
        FMC_ADDR0 =  adr;                              // Page Address
        FMC_CTL0  |=  FMC_CTL0_START;                  // Start Erase

        while(FMC_STAT0  & FMC_STAT0_BUSY){
            FWDGT_CTL = 0xAAAA;                        // Reload FWDGT
        }

        FMC_CTL0  &= ~FMC_CTL0_PER;                    // Page Erase Disabled 

        return (0);                                    // Done
#if defined GD32F30X_XD || defined GD32F30X_CL
    }else{
        FMC_CTL1  |=  FMC_CTL1_PER;                    // Page Erase Enabled 
        FMC_ADDR1 =  adr;                              // Page Address
        FMC_CTL1  |=  FMC_CTL1_START;                  // Start Erase

        while(FMC_STAT1  & FMC_STAT1_BUSY){
            FWDGT_CTL = 0xAAAA;                        // Reload FWDGT
        }

        FMC_CTL1  &= ~FMC_CTL1_PER;                    // Page Erase Disabled 

        return(0);                                     // Done
    }
#endif
}
#endif



#ifdef FMC_OB
int BlankCheck(unsigned long adr, unsigned long sz, unsigned char pat)
{
    return(1);                                          // Always Force Erase
}

#endif

#ifdef FMC_OB
int ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf)
{
    sz = (sz +1) & ~1;                                  // Adjust size for Half Words
    
    while(sz){
        FMC_CTL0 |=  FMC_CTL0_OBPG;                     // Option Byte Programming Enabled

        M16(adr) = *((unsigned short *)buf);            // Program Half Word
        while(FMC_STAT0 & FMC_STAT0_BUSY){
            FWDGT_CTL = 0xAAAA;                         // Reload FWDGT
        }

        FMC_CTL0 &= ~FMC_CTL0_OBPG;                     // Options Byte Programming Disabled

                                                        // Check for Errors
        if(FMC_STAT0 & (FMC_STAT0_PGERR | FMC_STAT0_WPERR)){
            FMC_STAT0 |= FMC_STAT0_PGERR | FMC_STAT0_WPERR;
            return(1);                                  // Failed
        }
                                                        // Go to next Half Word
        adr += 2;
        buf += 2;
        sz  -= 2;     
    }

    return(0);                                         // Done
}

#endif

#ifdef FMC_PE
int ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf) 
{
    sz = (sz + 3) & ~3;                             // Adjust size for  Words
#if defined GD32F30X_XD || defined GD32F30X_CL
    if(adr < (base_adr + BANK1_SIZE)){              // Flash bank 2
#endif   
        while(sz){
            FMC_CTL0  |=  FMC_CTL0_PG;              // Programming Enabled

            M32(adr) = *((unsigned long *)buf);     // Program Word
            while(FMC_STAT0  & FMC_STAT0_BUSY);

            FMC_CTL0  &= ~FMC_CTL0_PG;              // Programming Disabled

                                                    // Check for Errors
            if(FMC_STAT0 & (FMC_STAT0_PGERR | FMC_STAT0_WPERR)){
                FMC_STAT0 |= FMC_STAT0_PGERR | FMC_STAT0_WPERR;
                return(1);                          // Failed
            }
                                                    // Go to next  Word
            adr += 4;
            buf += 4;
            sz  -= 4;
        }

#if defined GD32F30X_XD || defined GD32F30X_CL
    }else{
        while(sz){
            FMC_CTL1  |=  FMC_CTL1_PG;               // Programming Enabled

            M32(adr) = *((unsigned long *)buf);      // Program Word
            while(FMC_STAT1  & FMC_STAT1_BUSY);

            FMC_CTL1  &= ~FMC_CTL1_PG;               // Programming Disabled

                                                     // Check for Errors
            if(FMC_STAT1 & (FMC_STAT1_PGERR | FMC_STAT1_WPERR)){
                FMC_STAT1 |= FMC_STAT1_PGERR | FMC_STAT1_WPERR;
                return(1);                          // Failed
            }
                                                     // Go to next  Word
            adr += 4;
            buf += 4;
            sz  -= 4;
        }
    }
#endif
    return(0);                                      // Done
}
#endif

