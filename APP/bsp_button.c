#include "bsp_button.h"


static void NVIC_Configuration(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 配置NVIC为优先级组1 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* 配置中断源：按键1 */
	NVIC_InitStructure.NVIC_IRQChannel = KEYR_INT_EXTI_IRQ;
	/* 配置抢占优先级 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* 配置子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* 使能中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* 配置中断源：按键2，其他使用上面相关配置 */
	NVIC_InitStructure.NVIC_IRQChannel = KEYD_INT_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = KEYL_INT_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = KEYUP_INT_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStructure);
}


static void button_setup(void) {
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//配置中断优先级
	NVIC_Configuration();

	quickly_gpioinit(KEYR_PORT, KEYR_PIN, KEYR_SPEED, KEYR_MODE, KEYR_GPIO_CLK);
	GPIO_EXTILineConfig(KEYR_INT_EXTI_PORTSOURCE, KEYR_INT_EXTI_PINSOURCE);
	EXTI_InitStruct.EXTI_Line = KEYR_INT_EXTI_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	//把配置好的成员写到寄存器
	EXTI_Init(&EXTI_InitStruct);


	quickly_gpioinit(KEYD_PORT, KEYD_PIN, KEYD_SPEED, KEYD_MODE, KEYD_GPIO_CLK);
	// 初始化EXTI
	GPIO_EXTILineConfig(KEYD_INT_EXTI_PORTSOURCE, KEYD_INT_EXTI_PINSOURCE);
	EXTI_InitStruct.EXTI_Line = KEYD_INT_EXTI_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	//把配置好的成员写到寄存器
	EXTI_Init(&EXTI_InitStruct);
	//配置中断优先级


	quickly_gpioinit(KEYL_PORT, KEYL_PIN, KEYL_SPEED, KEYL_MODE, KEYL_GPIO_CLK);
	// 初始化EXTI
	GPIO_EXTILineConfig(KEYL_INT_EXTI_PORTSOURCE, KEYL_INT_EXTI_PINSOURCE);
	EXTI_InitStruct.EXTI_Line = KEYL_INT_EXTI_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	//把配置好的成员写到寄存器
	EXTI_Init(&EXTI_InitStruct);
	//配置中断优先级


	quickly_gpioinit(KEYUP_PORT, KEYUP_PIN, KEYUP_SPEED, KEYUP_MODE, KEYUP_GPIO_CLK);
	// 初始化EXTI
	GPIO_EXTILineConfig(KEYUP_INT_EXTI_PORTSOURCE, KEYUP_INT_EXTI_PINSOURCE);
	EXTI_InitStruct.EXTI_Line = KEYUP_INT_EXTI_LINE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	//把配置好的成员写到寄存器
	EXTI_Init(&EXTI_InitStruct);
	//配置中断优先级

}

#define BUTTON_NUMBER 4

#define BTN_UP_IDX (0)
#define BTN_DOWN_IDX (1)
#define BTN_RIGHT_IDX (2)
#define BTN_LEFT_IDX (3)

#define KEY_DOWN_READ() (GPIO_ReadInputDataBit(KEYD_PORT,KEYD_PIN))
#define KEY_UP_READ() (GPIO_ReadInputDataBit(KEYUP_PORT,KEYUP_PIN))
#define KEY_LEFT_READ() (GPIO_ReadInputDataBit(KEYL_PORT,KEYL_PIN))
#define KEY_RIGHT_READ() (GPIO_ReadInputDataBit(KEYR_PORT,KEYR_PIN))
uint32_t btn_state_down(void) {
	return KEY_DOWN_READ();
}
uint32_t btn_state_up(void) {
	return KEY_UP_READ();
}
uint32_t btn_state_right(void) {
	return KEY_RIGHT_READ();
}
uint32_t btn_state_left(void) {
	return KEY_LEFT_READ();
}


uint32_t (*const button_get_state[BUTTON_NUMBER])(void) = {
	btn_state_up,
	btn_state_down,
	btn_state_left,
	btn_state_right
};

// 等待所有按键弹起，在此之前不产生按键事件
static bool button_wait_release = false;
void btn_wait_once(void) {
	button_wait_release = true;
}
static uint8_t button_current_valid = 0x00;
static uint8_t button_old_down = 0x00;
static uint8_t button_current_down = 0x00;
static uint16_t button_press_time[BUTTON_NUMBER] = { 0 };
static uint16_t button_press_time_up_and_down = 0;

static uint8_t button_main_short_count = 0;
static uint8_t button_main_hit_timeout = 0;
void btn_hit_reset(void) {
	button_main_short_count = 0;
	button_main_hit_timeout = 0;
}


const uint16_t button_down_event[BUTTON_NUMBER] = {
	UEVT_BTN_UP_DOWN,
	UEVT_BTN_DOWN_DOWN,
	UEVT_BTN_RIGHT_DOWN,
	UEVT_BTN_LEFT_DOWN
};
const uint16_t button_long_event[BUTTON_NUMBER] = {
	UEVT_BTN_UP_LONG,
	UEVT_BTN_DOWN_LONG,
	UEVT_BTN_RIGHT_LONG_WORK,
	UEVT_BTN_LEFT_LONG_WORK
};
const uint16_t button_repeat_event[BUTTON_NUMBER] = {
	UEVT_BTN_UP_REPEAT,
	UEVT_BTN_DOWN_REPEAT,
	UEVT_BTN_RIGHT_REPEAT,
	UEVT_BTN_LEFT_REPEAT
};
const uint16_t button_release_event[BUTTON_NUMBER] = {
	UEVT_BTN_UP_RELEASE,
	UEVT_BTN_DOWN_RELEASE,
	UEVT_BTN_RIGHT_RELEASE,
	UEVT_BTN_LEFT_RELEASE
};

volatile uint8_t btn_int_valid = 0xFF;
volatile uint8_t btn_int_debounce[BUTTON_NUMBER] = { 0 };
void KEYR_IRQHandler(void) {
	EXTI_ClearITPendingBit(EXTI_Line4);

	if(GPIO_ReadInputDataBit(KEYR_PORT, KEYR_PIN) != SET) {
		if(btn_int_valid & (1 << BTN_RIGHT_IDX)) {
			uevt_bc_e(UEVT_BTN_RIGHT_DOWN);
			//printf("UEVT_BTN_RIGHT_DOWN");
			btn_int_valid &= 0xFF ^ (1 << BTN_RIGHT_IDX);
		} else {
			btn_int_debounce[BTN_RIGHT_IDX] = 2;
		}
	}
}



void KEYD_IRQHandler(void) {
	//if(EXTI_GetITStatus(EXTI_Line4)!=RESET){

	EXTI_ClearITPendingBit(EXTI_Line3);
	if(GPIO_ReadInputDataBit(KEYD_PORT, KEYD_PIN) != SET) {
		if(btn_int_valid & (1 << BTN_DOWN_IDX)) {
			uevt_bc_e(UEVT_BTN_DOWN_DOWN);
			//printf("UEVT_BTN_DOWN_DOWN");
			btn_int_valid &= 0xFF ^ (1 << BTN_DOWN_IDX);
		} else {
			btn_int_debounce[BTN_DOWN_IDX] = 2;
		}
	}
}


void KEYL_IRQHandler(void) {
	EXTI_ClearITPendingBit(EXTI_Line2);
	if(GPIO_ReadInputDataBit(KEYL_PORT, KEYL_PIN) != SET) {
		if(btn_int_valid & (1 << BTN_LEFT_IDX)) {
			uevt_bc_e(UEVT_BTN_LEFT_DOWN);
			//printf("UEVT_BTN_LEFT_DOWN");
			btn_int_valid &= 0xFF ^ (1 << BTN_LEFT_IDX);
		} else {
			btn_int_debounce[BTN_LEFT_IDX] = 2;
		}
	}
}

void KEYUP_IRQHandler(void) {
	EXTI_ClearITPendingBit(EXTI_Line0);
	if(GPIO_ReadInputDataBit(KEYUP_PORT, KEYUP_PIN) != SET) {
		if(btn_int_valid & (1 << BTN_UP_IDX)) {
			uevt_bc_e(UEVT_BTN_UP_DOWN);
			//printf("UEVT_BTN_UP_DOWN");
			btn_int_valid &= 0xFF ^ (1 << BTN_UP_IDX);
		} else {
			btn_int_debounce[BTN_UP_IDX] = 2;
		}
	}
}


#define BTN_LONG_TICK (40)
#define BTN_REPEAT_TICK (12)
void btn_on_uevt_handler(uevt_t* evt) {
	uint8_t i = 0;
	static uint8_t btn_int_enable_delay = 0;
	static uint8_t btn_64hz_disable_delay = 0;
	switch(evt->evt_id) {
		case UEVT_APP_BOOT:
			button_setup();
			break;
		case UEVT_BTN_DOWN_DOWN:
			// set_lptim_req_bit(BUTTON_LPTIM_BIT);
			button_current_down |= (0x01 << BTN_DOWN_IDX);
			// if(button_current_down == ((0x01 << BTN_DOWN_IDX) | (0x01 << BTN_RIGHT_IDX)) && button_press_time[BTN_RIGHT_IDX] < BTN_LONG_TICK) {
			// 	button_current_valid = button_current_down;
			// 	btn_wait_once();
			// 	// uevt_bc_e(UEVT_BTN_DOWNMAIN_DOWN);
			// } else if(button_current_down == ((0x01 << BTN_DOWN_IDX) | (0x01 << BTN_UP_IDX)) && button_press_time[BTN_UP_IDX] < BTN_LONG_TICK) {
			// 	button_current_valid = (0x01 << BTN_DOWN_IDX) | (0x01 << BTN_UP_IDX);
			// 	button_press_time_up_and_down = 0;
			// } else if(button_current_valid == 0) {
			// 	button_current_valid = (0x01 << BTN_DOWN_IDX);
			// 	button_press_time[BTN_DOWN_IDX] = 0;
			// }
			// // LOG_RAW("current down=%02X valid=%02X\n", button_current_down, button_current_valid);
			// btn_hit_reset();
			break;
		case UEVT_BTN_UP_DOWN:
			// set_lptim_req_bit(BUTTON_LPTIM_BIT);
			button_current_down |= (0x01 << BTN_UP_IDX);
			// if(button_current_down == ((0x01 << BTN_UP_IDX) | (0x01 << BTN_RIGHT_IDX)) && button_press_time[BTN_RIGHT_IDX] < BTN_LONG_TICK) {
			// 	button_current_valid = button_current_down;
			// 	btn_wait_once();
			// 	// uevt_bc_e(UEVT_BTN_UPMAIN_DOWN);
			// } else if(button_current_down == ((0x01 << BTN_UP_IDX) | (0x01 << BTN_DOWN_IDX)) && button_press_time[BTN_DOWN_IDX] < BTN_LONG_TICK) {
			// 	button_current_valid = (0x01 << BTN_UP_IDX) | (0x01 << BTN_DOWN_IDX);
			// 	button_press_time_up_and_down = 0;
			// } else if(button_current_valid == 0) {
			// 	button_current_valid = (0x01 << BTN_UP_IDX);
			// 	button_press_time[BTN_UP_IDX] = 0;
			// }
			// // LOG_RAW("current down=%02X valid=%02X\n", button_current_down, button_current_valid);
			// btn_hit_reset();
			break;
		case UEVT_BTN_RIGHT_DOWN:
			// set_lptim_req_bit(BUTTON_LPTIM_BIT);
			button_current_down |= (0x01 << BTN_RIGHT_IDX);
			// if((button_current_down & ((0x01 << BTN_DOWN_IDX) | (0x01 << BTN_RIGHT_IDX)) == ((0x01 << BTN_DOWN_IDX) | (0x01 << BTN_RIGHT_IDX)))
			//    && button_press_time[BTN_DOWN_IDX] < BTN_LONG_TICK) {
			// 	button_current_valid = button_current_down;
			// 	button_main_short_count = 0;
			// 	button_main_hit_timeout = 0;
			// 	btn_wait_once();
			// 	uevt_bc_e(UEVT_BTN_DOWNMAIN_DOWN);
			// 	break;
			// } else if((button_current_down & ((0x01 << BTN_UP_IDX) | (0x01 << BTN_RIGHT_IDX)) == ((0x01 << BTN_UP_IDX) | (0x01 << BTN_RIGHT_IDX)))
			// 		  && button_press_time[BTN_UP_IDX] < BTN_LONG_TICK) {
			// 	button_current_valid = button_current_down;
			// 	button_main_short_count = 0;
			// 	button_main_hit_timeout = 0;
			// 	btn_wait_once();
			// 	uevt_bc_e(UEVT_BTN_UPMAIN_DOWN);
			// 	break;
			// } else {
			// 	button_current_valid = (0x01 << BTN_RIGHT_IDX);
			// }
			// button_press_time[BTN_RIGHT_IDX] = 0;
			// if(button_main_short_count == 0) {
			// 	button_main_short_count = 1;
			// }
			// if(button_main_hit_timeout > 0) {
			// 	button_main_short_count += 1;
			// 	if(button_main_short_count == 5) {
			// 		uevt_bc_e(UEVT_BTN_RIGHT_HIT5);
			// 		button_main_short_count = 0;
			// 	}
			// }
			break;
		case UEVT_BTN_LEFT_DOWN:
			button_current_down |= (0x01 << BTN_LEFT_IDX);
			break;
		case UEVT_RTC_1MS:
			// if(!is_charge) {
			for(i = 0; i < BUTTON_NUMBER; i++) {
				if(!(button_get_state[i]())) {
					button_current_down &= ~(0x01 << i);  //清零
				}
			}
			// 	if(button_main_hit_timeout > 0 && (button_current_valid & 0x04) == 0) {
			// 		button_main_hit_timeout -= 1;
			// 		if(button_main_hit_timeout == 0) {
			// 			if(button_main_short_count > 0) {
			// 				if(button_main_short_count <= 4) {
			// 					uevt_bc_e(UEVT_BTN_RIGHT_HIT0 + button_main_short_count);
			// 				}
			// 			}
			// 			button_main_short_count = 0;
			// 		}
			// 	}


			// 	if(button_current_valid > 0) {
			// 		// BUTTON MAIN 事件单独处理
			// 		if(button_current_valid & (1 << BTN_RIGHT_IDX)) {
			// 			button_press_time[BTN_RIGHT_IDX] += 1;
			// 			if(button_press_time[BTN_RIGHT_IDX] == 16) {
			// 				button_main_short_count = 0;
			// 				if(!button_wait_release) {
			// 					uevt_bc_e(UEVT_BTN_RIGHT_LONG_WORK);
			// 				}
			// 			}
			// 			if(button_press_time[BTN_RIGHT_IDX] >= BTN_LONG_TICK + BTN_REPEAT_TICK) {
			// 				// uevt_bc_e(button_repeat_event[i]);
			// 				button_press_time[BTN_RIGHT_IDX] = BTN_LONG_TICK + 1;
			// 			}
			// 			if((button_current_down & (1 << BTN_RIGHT_IDX)) == 0) {
			// 				button_current_valid &= (0xFF ^ (1 << BTN_RIGHT_IDX));
			// 				button_press_time[BTN_RIGHT_IDX] = 0;
			// 				if(!button_wait_release) {
			// 					button_main_hit_timeout = 16;
			// 					uevt_bc_e(UEVT_BTN_RIGHT_RELEASE);
			// 				}
			// 			}
			// 		}
			// 		// // 此处处理BUTTON UP与DOWN事件
			// 		for(uint8_t i = 0; i < BUTTON_NUMBER - 1; i++) {
			// 			if(button_current_valid & (0x1 << i)) {
			// 				button_press_time[i] += 1;
			// 				if(button_press_time[i] == BTN_LONG_TICK) {
			// 					if(!button_wait_release && (button_current_valid == (0x1 << i))) {
			// 						uevt_bc_e(button_long_event[i]);
			// 					}
			// 				}
			// 				if(button_press_time[i] >= BTN_LONG_TICK + BTN_REPEAT_TICK) {
			// 					if(!button_wait_release && (button_current_valid == (0x1 << i))) {
			// 						uevt_bc_e(button_repeat_event[i]);
			// 					}
			// 					button_press_time[i] = BTN_LONG_TICK + 1;
			// 				}
			// 				if(((button_current_down & (0x01 << i))) == 0 && (button_current_valid & (0x01 << i))) {
			// 					button_current_valid &= (0xFF ^ (0x01 << i));
			// 					if(!button_wait_release) {
			// 						if(button_press_time[i] >= 2) {
			// 							uevt_bc_e(button_release_event[i]);
			// 						}
			// 					}
			// 					button_press_time[i] = 0;
			// 				}
			// 			}
			// 		}
			// 		// // 此处处理BUTTON UP与DOWN同时触发事件
			// 		// if(button_current_valid == ((0x1 << BTN_DOWN_IDX) | (0x1 << BTN_UP_IDX))) {
			// 		// 	button_press_time_up_and_down += 1;
			// 		// 	if(button_press_time_up_and_down == BTN_LONG_TICK) {
			// 		// 		if(!button_wait_release) {
			// 		// 			uevt_bc_e(UEVT_BTN_UPDOWN_LONG);
			// 		// 		}
			// 		// 	}
			// 		// 	if(button_press_time_up_and_down >= BTN_LONG_TICK + BTN_REPEAT_TICK) {
			// 		// 		button_press_time_up_and_down = BTN_LONG_TICK + 1;
			// 		// 	}
			// 		// }
			// 	}
			for(i = 0; i < BUTTON_NUMBER; i++) {
				if(!(btn_int_valid & (1 << i)) && !(button_current_down & (1 << i))) {
					if(btn_int_debounce[i] > 0) {
						btn_int_debounce[i] -= 1;
						if(btn_int_debounce[i] == 0) {
							btn_int_valid |= (1 << i);
							btn_64hz_disable_delay = 32;
						}
					} else {
						btn_int_debounce[i] = 2;
					}
				}
			}
			// 	if(btn_64hz_disable_delay > 0) {
			// 		btn_64hz_disable_delay -= 1;
			// 		if(btn_64hz_disable_delay == 0) {
			// 			reset_lptim_req_bit(BUTTON_LPTIM_BIT);
			// 		}
			// 	}
			// 	if(button_current_down == 0) {
			// 		button_wait_release = false;
			// 	}
			// } else {
			// 	button_wait_release = true;
			// }
			break;
	}
}



void button_init(void) {
	user_event_handler_regist(btn_on_uevt_handler);
}
