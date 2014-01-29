#include "misc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"

void TIM_ConfigPins(void);

TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
TIM_OCInitTypeDef			TIM_OCInitStructure;

__IO uint16_t Channel1Pulse = 500;		//  500 * 100 us = 50 ms
__IO uint16_t Channel2Pulse = 1000;		//  100 ms
__IO uint16_t Channel3Pulse = 5000;		//  500 ms
__IO uint16_t Channel4Pulse = 10000;	//    1 s


int main(void)
{
	SystemInit();

	/* TIM1 Configuration */
	TIM_ConfigPins();

	/* Time Base configuration */
	// TIM4_CNT increments every 100 us
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ( (SystemCoreClock / 2) / 10000);
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	// TIM4_CNT overflows every 1000 ms
	TIM_TimeBaseStructure.TIM_Period = 10000;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


	/* Channel 1 to 4 Configuration in Output Compare Toggle mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	/* TIM1 counter enable */
	TIM_Cmd(TIM4, ENABLE);

	/* TIM IT enable */
	TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

	while (1)
	{
	}
}

/**
 * @brief  Configure the TIM4 Pins.
 * @param  None
 * @retval None
 */
void TIM_ConfigPins(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* GPIOD clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* GPIOD Configuration: Channels 1 to 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Connect TIM pins to AF1 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	/* Enable the TIM4 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief  This function handles TIM4 global interrupt request.
 * @param  None
 * @retval None
 */
void TIM4_IRQHandler(void)
{
	uint16_t capture = 0;

	/* TIM4_CH1 toggling */
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
		capture = TIM_GetCapture1(TIM4);
		TIM_SetCompare1(TIM4, capture + Channel1Pulse );
	}

	/* TIM4_CH2 toggling */
	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM4);
		TIM_SetCompare2(TIM4, capture + Channel2Pulse);
	}

	/* TIM4_CH3 toggling */
	if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		capture = TIM_GetCapture3(TIM4);
		TIM_SetCompare3(TIM4, capture + Channel3Pulse);
	}

	/* TIM4_CH4 toggling */
	if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		capture = TIM_GetCapture4(TIM4);
		TIM_SetCompare4(TIM4, capture + Channel4Pulse);
	}

	/* TIM_Update reset all */
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		TIM_SetCompare1(TIM4, Channel1Pulse);
		TIM_SetCompare2(TIM4, Channel2Pulse);
		TIM_SetCompare3(TIM4, Channel3Pulse);
		TIM_SetCompare4(TIM4, Channel4Pulse);
	}


}
