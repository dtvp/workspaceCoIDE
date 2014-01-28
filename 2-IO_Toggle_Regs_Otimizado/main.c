#include "stm32f4xx.h"

void Delay(__IO uint32_t nCount);

int main(void)
{
	uint32_t ledIndex;

	SystemInit();

	/* GPIOD Periph clock enable */
	RCC->AHB1ENR |= 0x8;

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIOD->MODER &= ~(0x55000000);
	GPIOD->MODER |=   0x55000000;

	while(1)
	{

		for ( ledIndex = 0x1000; ledIndex != 0x10000; ledIndex = ledIndex << 1 )
		{
			/* Pin to be toggled */
			if ( GPIOD->ODR & ledIndex )
				GPIOD->BSRRH = ledIndex;
			else
				GPIOD->BSRRL = ledIndex;

			/* Insert delay */
			Delay(0x3FFFFF);
		}

	}

}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
