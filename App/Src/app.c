
#include "app.h"
#include "main.h"


#if OS_FREERTOS
# include "cmsis_os.h"
# include "st25r.h"

# include "data_protocol.h"
# include "eeprom.h"
# include "sender.h"

# include "led.h"

# if FTR_GUI
#  include "gui.h"
# endif
#endif

#if BOARD(CONTROLLER, 0)
#include "FatFs.h"
#include "diskio.h"
#include "ff_gen_drv.h"
#include "ff.h"
#include "ffconf.h"
#include "integer.h"
#include "sd_diskio.h"
#endif 

#if OS_BAREMETAL

#elif OS_FREERTOS
extern uint32_t period;
uint32_t period = 20;

#define NUM_WORDS ((416 * (240 / 8)) / 4)
uint32_t words[NUM_WORDS];

void Controller_hearbeatTask(void *args)
{
#if FTR_GUI
	GUI_init();
#endif

#if FTR_LED
	LED_enableHz(LED_DEBUG_B, 1);
#endif

	while(1)
	{
		osDelay(500);
	}

#if FTR_DATASENDER
	SenderDataSpec senderData;


	senderData.startBit = 0;
	senderData.data = words;
	senderData.numWords = NUM_WORDS;

	for (uint32_t i = 0; i < NUM_WORDS; i++)
	{
		words[i] = i;
	}

#endif

	static uint32_t counter = 0;

	for (;;)
	{
#if FTR_DATASENDER
		extern osMessageQueueId_t dataSenderQueueHandle;
		osMessageQueuePut(dataSenderQueueHandle, &senderData, 0, 10);
#endif

		osDelay(period);
	}
}
#endif

#if BOARD(CONTROLLER, 0)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	ST25R_EXTICallback(GPIO_Pin);
}

FATFS fs;  //file system object
FIL file;  //file itself
FRESULT fr; //Fatfs return
FRESULT res; //result variable
FILINFO fno;
uint8_t 
uint8_t card_buffer[128]; //buffer to store file data change size? 
//card size buffer^^
UINT br;  // bytes read
DIR dir; 
char all_names_buffer [20][32]; //rows, names
char file_name_buffer [32];


static void sd_menu(){

	//SELECT SD MENU IN GUI.C
	//display a screen that scrolls through different file names 
	//fetch file name 
	int index = 0;
	int store_max_index = 0;
	res = f_opendir(&dir, "/");
	if (res != FR_OK){
		return;
	}
	while (1) {
		res = f_readdir(&dir, &fno);  // Read a directory item
		if (res != FR_OK || fno.fname[0] == 0)  // No more files
			break;
		all_names_buffer[index] = fno.fname;
		index = index+1;
	}

	store_max_index = index; 
	int selectFile = -1;

	//controls here:
	//if GPIO pin up then
	//change = -1 or +1 
	// if (!(index + change)%store_max_index) (if it equals 0 then)
	//then  index = store_max index
	//else index = index+change
	//display file anme all_names_buffer[index] 
	//if selected
		file_name_buffer = all_names_buffer[index];//return value here
}

static void sdcard_demo()
{
	//SD card needs SDIO or SPI

	//direct to sdcard_demo()
	char image_name 
	fr = f_mount(&fs, "", 1);

	if (fr != FR_OK){
		//PRINT ON CONTROLLER SCREEN SD NOT READ
		return;
	}

	sd_menu();
	
	fr = f_open(&file, file_name_buffer, FA_READ);
	if (fr != FR_OK){
		//print fail or flashred lLED?
		f_mount(NULL, "", 1);  // Unmount SD
        return;
	}


	fr = f_read(&file, buffer, sizeof(buffer)-1, &br);
	if(fr==FR_OK){
		//print file data? say file name and to tap card
		//upload to e-ink!
	}


	f_closedir(&dir);

	// Trap
	//while (1);
}
#endif

#if BOARD(CONTROLLER, 1)
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	Button_EXTIRisingCallback(GPIO_Pin);
	ST25R_EXTICallback(GPIO_Pin);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	Button_EXTIRisingCallback(GPIO_Pin);
}
#endif
