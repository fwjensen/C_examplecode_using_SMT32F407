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
int state = 1;
int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOAEN;

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

	//Button
	GPIOA->MODER |= 0x00;
	GPIOA->OTYPER &= ~(GPIO_OTYPER_IDR_0);
	GPIOA->OSPEEDR |= GPIO_Speed_2MHz;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0_1);

    while(1)
    {
    	//GPIO_SetBits(GPIOD, GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    	//for(i = 0;i < 0x100000;i++){};
    	//GPIO_ResetBits(GPIOD, GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

//    	if(GPIOA->IDR & 0x01 == 0x01){
//    		GPIOD->ODR ^= GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//    		for(i = 0;i < 0x100000;i++){};
//    	}


    	ButtonPress();
    	switch(state) {
    	case 1: LysAv(); break;

    	case 2: LysPaa(); break;

    	case 3: LysBlink(); break;

    	case 4: LysTivoli(); break;

    	}

//    	GPIOD->ODR ^= GPIO_Pin_12;
//    	for(i = 0;i < 0x100000;i++){};
//    	GPIOD->ODR ^= GPIO_Pin_12;
//    	GPIOD->ODR ^= GPIO_Pin_13;
//    	for(i = 0;i < 0x100000;i++){};
//    	GPIOD->ODR ^= GPIO_Pin_13 ;
//    	GPIOD->ODR ^= GPIO_Pin_14;
//    	for(i = 0;i < 0x100000;i++){};
//    	GPIOD->ODR ^= GPIO_Pin_14;
//    	GPIOD->ODR ^= GPIO_Pin_15;
//    	for(i = 0;i < 0x100000;i++){};
//    	GPIOD->ODR ^= GPIO_Pin_15;
    }
}

void ButtonPress(){
	if(GPIOA->IDR & GPIO_Pin_0 == GPIO_Pin_0){
		if(state < 4){
			state++;
		}else{
			state = 1;
		}
	}
}
void LysAv(){
	GPIOD->ODR &= ~(GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	pause(0x100000);
}
void LysPaa(){
	GPIOD->ODR |= GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	pause(0x100000);
}
void LysBlink(){
	GPIOD->ODR ^= GPIO_Pin_12 |  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	pause(0x100000);
}
void LysTivoli(){
	GPIOD->ODR |= GPIO_Pin_12;
	pause(0x10000);
	GPIOD->ODR &= ~(GPIO_Pin_12);
	GPIOD->ODR |= GPIO_Pin_13;
	pause(0x10000);
	GPIOD->ODR &= ~(GPIO_Pin_13);
	GPIOD->ODR |= GPIO_Pin_14;
	pause(0x10000);
	GPIOD->ODR &= ~(GPIO_Pin_14);
	GPIOD->ODR |= GPIO_Pin_15;
	pause(0x10000);
	GPIOD->ODR &= ~(GPIO_Pin_15);
}
void pause(int32_t time){
	for(i = 0;i < time;i++){};
}
