#ifndef __NORFLASH_DRV_H_
#define __NORFLASH_DRV_H_

void NorflashDrvInit(void);
void ReadNorflashID(uint8_t *mid, uint16_t *did);
void NorflashDrvTest(void);



#endif
