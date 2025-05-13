#include "even.h"
//#include "bsp_spi_flash.h"
//#include "bsp_spi_lcd.h"
#include "stdio.h"
#include "platform.h"

#include "pic.h"


//处理打印测试
void main_handler(uevt_t* evt) {
	char time_string[9]; // HH:MM:SS 格式需要 9 个字符（包括结束符）

	static bool started = false;
	static uint32_t tick_1MS = 0;
	static uint32_t tick_10MS = 0;
	static uint8_t h = 0;
	switch(evt->evt_id) {
		case UEVT_APP_BOOT:
			lcd_gpio_init();
			flash_gpio_init();
			spi2_init();
			SPI_FLASH_TYPE = flash_reas_id(); //读取FLASH ID.
			lcd_init();//LCD初始化
			led_init();//LED初始化
			lcd_clear(0, 0, LCD_W, LCD_H, WHITE);
			// LCD_ShowPicture(20, 45, 120, 29, gImage_pic1);
			// LCD_ShowPicture2(0, 0,  &boot_00015_bmp);
			// LCD_ShowPicture_test(0, 0,  0);
			// LCD_ShowString(10,0,"stm32f103_rct6!",WHITE,BLACK,16,0);
			
			LCD_BL_ON();//打开背光
			break;
		case UEVT_RTC_10MS:
			if(started) {
				//LOG_HEAD("[%08d]:\n", tick++);
			}
			h++;
			if(h % 3 == 0) {
				// LCD_ShowPicture2(0, 0,  &boot_00025_bmp);
				// LCD_ShowPicture2(0, 0,  fonts_10_12_num_array[h / 10 % 10]);
				
			}
			// lcd_clear(0, 0, LCD_W, LCD_H, BLACK);
			// LCD_ShowPicture(20, 45, 120, 29, gImage_pic1);
			//    LCD_ShowPicture2(h%20, 45,  &boot_00000_bmp);
			// if(pwm_n <200){
			// 	pwm_n++;
			// }else{
			// 	pwm_n = 0;
			// }
			// setledg_pwm(spwmWave[pwm_n]);
			break;
		case UEVT_RTC_1MS:
			// tick_1MS++;
			// if(tick_1MS % 1000 == 500) {
			// 	LED_R(OFF);
			// } else if(tick_1MS % 1000 == 0) {
			// 	LED_R(ON);
			// }
			break;

		case UEVT_APP_START:
			started = true;
			break;
		default:
			break;
	}
}




void ws2812_handler(uevt_t* evt) {
	static bool started = false;
	static uint8_t work_led_h = 0;
	static uint8_t work_led_v = 100;
	static uint8_t b_led = 0;
	static uint32_t tick_1MS = 0;
	uint8_t i = 0;
	switch(evt->evt_id) {
		case UEVT_RTC_100MS:

			break;
		case UEVT_RTC_1MS:
			// b_led++;
			// if(b_led % 4 == 0) {
			// 	work_led_h++;
			// 	for(i = 0; i < WS2812_DEPTH_N; i++) {
			// 		ws2812_set_color(i, hsv2rgb(work_led_h + i * 4, 255, work_led_v));
			// 	}

			// 	ws2812_color_update();
			// }
			break;
		case UEVT_APP_BOOT:
			// //初�?�化
			// SPI1_Init();
			// dma_config();
			// printf("dma_config!\r\n");
			break;
		default:
			break;
	}
}



static void dma_init(void) {
	user_event_handler_regist(ws2812_handler);
}


uint8_t buf[30];
const uint8_t text_buf[] = "www.prechin.net";
void flash_handler(uevt_t* evt) {
	static bool started = false;
	static uint32_t tick_1MS = 0;
	switch(evt->evt_id) {
		case UEVT_RTC_100MS:
			break;
		case UEVT_RTC_1MS:
			break;
		case UEVT_APP_BOOT:
			// en25qxx_init();
			// while(en25qxx_readid() != EN25Q128) {		//检测不到EN25QXX
			// 	printf("EN25Q128 Check Failed!\r\n");
			// }
			// printf("EN25Q128 Check Success!!!!\r\n");
			// uint16_t i = sizeof(text_buf);
			// en25qxx_write((u8*)text_buf, 0, i);
			// printf("发送的数据�?%s\r\n", text_buf);
			// en25qxx_read(buf, 0, i);
			// printf("接收的数�?�?%s\r\n", buf);
			break;
	}
}

static void spi_flash_init(void) {
	user_event_handler_regist(flash_handler);
}

void spi_lcd_handler(uevt_t* evt) {
	static uint16_t i = 0;
	switch(evt->evt_id) {
		case UEVT_APP_BOOT:
			// Lcd_Init();
			// Lcd_Clear(BLACK); //清屏
			// my_showimage(0, 0, &qq_bmp);
			break;
		case UEVT_RTC_1MS:
			break;
		case UEVT_RTC_100MS:
			// if(i++ ) {
			// 	my_showimage(0, 0, fonts_22_28_num_array[i % 10]);
			// }
			break;
	}
}


static void display_init(void) {
	user_event_handler_regist(spi_lcd_handler);
}



void eeprom_handler(uevt_t* evt) {
	static uint16_t i = 0;
	switch(evt->evt_id) {
		case UEVT_APP_BOOT:
			// 初�?�化
			// si2c_master_init(); /* 配置GPIO */
			// eeprom_test();
			break;
		case UEVT_RTC_1MS:
			break;
		case UEVT_RTC_100MS:

			break;
	}


}


static void eeprom_init(void) {
	user_event_handler_regist(eeprom_handler);
}

// void others_handler(uevt_t* evt) {
// 	static uint8_t time_string[20];
// 	static uint16_t tick2_10ms = 0;
// 	switch(evt->evt_id) {
// 		case UEVT_APP_BOOT:
// 			// 初�?�化
// 			BEEP_Init();
// 			break;
// 		case UEVT_RTC_1MS:
// 			break;
// 		case UEVT_RTC_10MS:
// 			tick2_10ms++;
// 			// if(beep_on_times > 0) {
// 			// 	if(tick2_10ms % 100 == 0){
// 			// 		beep_on();
// 			// 	}
// 			// 	if(tick2_10ms % 100 == 50){
// 			// 		beep_on_times--;
// 			// 		beep_off();
// 			// 	}

// 			// }
// 			sprintf(time_string, "%02d", tick2_10ms);
// 			LCD_ShowString(100, 300, tftlcd_data.width, tftlcd_data.height, 24, time_string);
// 			// beep_on();
// 			break;
// 		case UEVT_RTC_100MS:

// 			break;
// 	}
// }


void others_handler(uevt_t* evt) {
	char time_string[9]; // HH:MM:SS 格式需要 9 个字符（包括结束符）

	static bool started = false;
	static uint32_t tick_1MS = 0;
	static uint32_t tick_10MS = 0;
	static uint8_t pwm_n = 0;
	switch(evt->evt_id) {
		case UEVT_APP_BOOT:
			// TFTlcd_init();			//LCD初�?�化

			// LCD_ShowString(10, 30, tftlcd_data.width, tftlcd_data.height, 16, "Hello World!");
			// LCD_ShowString(10, 50, tftlcd_data.width, tftlcd_data.height, 24, "Hello World!");
			// LCD_ShowFontHZ(10, 80, "普中科技");
			// LCD_ShowString(10, 120, tftlcd_data.width, tftlcd_data.height, 24, "www.prechin.cn");

			break;
		case UEVT_RTC_100MS:
			if(started) {
				//LOG_HEAD("[%08d]:\n", tick++);
			}
			// if(pwm_n <200){
			// 	pwm_n++;
			// }else{
			// 	pwm_n = 0;
			// }
			// setledg_pwm(spwmWave[pwm_n]);
			break;
		case UEVT_RTC_1MS:
			// tick_1MS++;
			// if(tick_1MS % 1000 == 500) {
			// 	LED_R(OFF);
			// } else if(tick_1MS % 1000 == 0) {
			// 	LED_R(ON);
			// }
			break;
		case UEVT_RTC_10MS:
			tick_10MS++;
			// if(tick_10MS % 100 == 0 && beep_on_times > 0) {
			// 	beep_on_times--;
			// 	beep_on();
			// }
			// if(tick_10MS % 100 == 50) {
			// 	beep_off();
			// }
			break;
		case UEVT_APP_START:
			started = true;
			break;
		default:
			break;
	}
}

static void other_init(void) {
	user_event_handler_regist(others_handler);
}

void module_init(void) {
	user_event_handler_regist(main_handler);
	// other_init();
	// eeprom_init();
	// display_init();
	// dma_init();
	// spi_flash_init();
	// button_init();
	// adc_init();
	// pwm_init();
	// user_event_handler_regist(test_handler);
}
