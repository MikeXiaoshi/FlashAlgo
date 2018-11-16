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
 * Project:      Flash Device Description for GigaDevice GD32F30x Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "FlashOS.H"        // FlashOS Structures

#ifdef FMC_PE

#ifdef GD32F30X_HD
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "GD32F30x High-density FMC ",         // Device Name 
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00080000,                 // Device Size in Bytes (512kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0800, 0x000000,           // Sector Size  2kB (256Sectors)
   SECTOR_END
};
#endif

#ifdef GD32F30X_XD
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "GD32F30x Extra-density FMC ",         // Device Name 
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00300000,                 // Device Size in Bytes (3072kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0800, 0x000000,           // Sector Size  2kB (256Sectors)
   0x1000, 0x080000,           // Sector Size  4kB (640Sectors)
   SECTOR_END
};

#endif


#ifdef GD32F30X_CL
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "GD32F30x Connectivity line FMC ",        // Device Name 
   ONCHIP,                     // Device Type
   0x08000000,                 // Device Start Address
   0x00100000,                 // Device Size in Bytes (1024kB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   3000,                       // Erase Sector Timeout 3000 mSec

// Specify Size and Address of Sectors
   0x0800, 0x000000,           // Sector Size  2kB (256Sectors)
   0x1000, 0x080000,           // Sector Size  4kB (640Sectors)
   SECTOR_END
};
#endif

#endif // FMC_PE



#ifdef FMC_OB

struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "GD32F30x FMC OB",          // Device Name
   ONCHIP,                     // Device Type
   0x1FFFF800,                 // Device Start Address
   0x00000010,                 // Device Size in Bytes (16)
   16,                         // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   3000,                       // Program Page Timeout 3 Sec
   3000,                       // Erase Sector Timeout 3 Sec

 //Specify Size and Address of Sectors
   0x0010, 0x000000,           // Sector Size 16B
   SECTOR_END
};

#endif  // FMC_OB
