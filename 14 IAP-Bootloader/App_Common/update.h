#ifndef _UPDATE_H_
#define _UPDATE_H_

#define FLASH_SIZE                        0x80000
#define APP_ADDR_IN_FLASH                 0x8003000
#define FLASH_APP_SIZE                    (FLASH_SIZE - (APP_ADDR_IN_FLASH - 0x08000000))  //����app�ռ���ô�С

void UpdateApp(void);

#endif

