#include "stm32f4xx.h"
//#include <stm32f4xx_gpio.h>
//#include <stm32f4xx_rcc.h>

//GPIO_InitTypeDef  GPIO_InitStructure;

void Delay(__IO uint32_t nCount);

int main(void)
{

	SystemInit();

	/* GPIOD Periph clock enable */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC->AHB1ENR |= 0x8;

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIOD->MODER &= ~(0x55000000);
	GPIOD->MODER |=   0x55000000;

	while(1)
	{

		/* PD12 to be toggled */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		if ( GPIOD->ODR & 0x00001000 )
			GPIOD->BSRRH = 0x1000;
		else
			GPIOD->BSRRL = 0x1000;

		/* Insert delay */
		Delay(0x3FFFFF);

		/* PD13 to be toggled */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		if ( GPIOD->ODR & 0x00002000 )
			GPIOD->BSRRH = 0x2000;
		else
			GPIOD->BSRRL = 0x2000;

		/* Insert delay */
		Delay(0x3FFFFF);

		/* PD14 to be toggled */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		if ( GPIOD->ODR & 0x00004000 )
			GPIOD->BSRRH = 0x4000;
		else
			GPIOD->BSRRL = 0x4000;

		/* Insert delay */
		Delay(0x3FFFFF);

		/* PD15 to be toggled */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		if ( GPIOD->ODR & 0x00008000 )
			GPIOD->BSRRH = 0x8000;
		else
			GPIOD->BSRRL = 0x8000;

		/* Insert delay */
		Delay(0x3FFFFF);

	}

}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
