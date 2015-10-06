#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define LEDs = (GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

/*GPIOx_ODR Offset 0x14
 *
 *
 */

//Enabeling RCC - Reset
int i;
int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	//GPIO_InitTypeDef GPIO_InitStruct;

	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	//GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIOD->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;
	GPIOD->OTYPER &= ~(GPIO_OTYPER_OT_12 | GPIO_OTYPER_OT_13 | GPIO_OTYPER_OT_14 | GPIO_OTYPER_OT_15);
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12 | GPIO_OSPEEDER_OSPEEDR13 | GPIO_OSPEEDER_OSPEEDR14 | GPIO_OSPEEDER_OSPEEDR15;
	GPIOD->PUPDR &= ~(GPIO_PUPDR_PUPDR12 | GPIO_PUPDR_PUPDR13 | GPIO_PUPDR_PUPDR14 | GPIO_PUPDR_PUPDR15);

    while(1)
    {
    	LysBlink();
    }
}

void LysBlink()
{
	GPIOD->ODR ^= GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	pause(0x100000);
}
void pause(int32_t time)
{
	for(i = 0;i < time;i++){};
}
