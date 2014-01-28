
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

GPIO_InitTypeDef  GPIO_InitStructure;
uint32_t tick;

void Delay_ms(uint32_t time_ms);

int main(void)
{

	SystemInit();

	SysTick_Config(SystemCoreClock / 1000);

	/* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	while(1)
	{

		/* PD12 to be toggled */
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);

		/* Insert delay */
		Delay_ms(100);

		/* PD13 to be toggled */
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);

		/* Insert delay */
		Delay_ms(100);

		/* PD14 to be toggled */
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);

		/* Insert delay */
		Delay_ms(100);

		/* PD15 to be toggled */
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);

		/* Insert delay */
		Delay_ms(100);

	}

}

void Delay_ms(uint32_t time_ms)
{
	uint32_t delayTicks = tick + time_ms;

	while ( delayTicks != tick );
}


void SysTick_Handler(void)
{
	tick++;
}
