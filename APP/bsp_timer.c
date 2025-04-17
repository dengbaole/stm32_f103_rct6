#include "bsp_timer.h"


void tim6_init(uint16_t per, uint16_t psc) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);             //使能时钟

	TIM_TimeBaseInitStructure.TIM_Period = per;                      //预分频器
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); //¿ªƴ¶¨ʱƷ֐¶ύ
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	//中断配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;//中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨµÀʹĜ
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM6, ENABLE); //ʹĜ¶¨ʱƷ
}


// 初始化 TIM7
void tim7_init(uint16_t per, uint16_t psc) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); // 使能 TIM7 时钟

	TIM_TimeBaseInitStructure.TIM_Period = per; // 自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc; // 预分频器
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); // 使能更新中断
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update); // 清除中断挂起位

	// 中断配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn; // 中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM7, ENABLE); // 启动 TIM7
}



/*******************************************************************************
* 函 数 名         : tim3_ch2_pwm_init
* 函数功能		   : TIM3通道2 PWM初始化函数
* 输    入         : per:重装载值
					 psc:分频系数
* 输    出         : 无
*******************************************************************************/
void tim3_ch2_pwm_init(uint16_t per, uint16_t psc) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //改变指定管脚的映射

	TIM_TimeBaseInitStructure.TIM_Period = per; //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); //输出比较通道2初始化

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能TIMx在 CCR2 上的预装载寄存器
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能预装载寄存器

	TIM_Cmd(TIM3, ENABLE); //使能定时器
}




//36MHz/36 = 1000 000   1us     1ms*100 = 100 = 100us
void TIM6_IRQHandler(void) {
	static u8 t_100us = 0;
	if(TIM_GetITStatus(TIM6, TIM_IT_Update)) {
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		// todo 定时器100us
		if(t_100us == 0) {
			if(g_sort_pwm > 0) {
				LED_G(ON);
			}
			if(r_sort_pwm > 0) {
				LED_R(ON);
			}
		}
		if(t_100us >= g_sort_pwm) {
			LED_G(OFF);
		}
		if(t_100us >= r_sort_pwm) {
			LED_R(OFF);
		}
		t_100us++;
		if(t_100us >= 100) {

			t_100us = 0;
		}

	}
}

// 定时器 7 中断服务例程
void TIM7_IRQHandler(void) {
	static uint16_t counter;
	// 检查 TIM7 更新中断标志位
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
		// 清除中断标志位
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

		counter++;
		uevt_bc_e(UEVT_RTC_10MS);
		if((counter % 10) == 0) {
			uevt_bc_e(UEVT_RTC_100MS);
		}
	}
}


/// @brief 1ms定时器
/// @param
void SysTick_Handler(void) {
	static uint16_t counter;
	uevt_bc_e(UEVT_RTC_1MS);
	counter++;
	if((counter % 10) == 0) {
		uevt_bc_e(UEVT_RTC_10MS);
	}
	if((counter % 100) == 0) {
		uevt_bc_e(UEVT_RTC_100MS);
	}
}
