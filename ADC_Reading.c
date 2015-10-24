#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_tim.h"

static __IO uint32_t speed = 0x100000;
static __IO uint16_t AD_value;
int i;

void LysTivoli(void);
void pause(int32_t time);

GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;

void InitializeTimer(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 40000;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 500;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void InitializeLEDs(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIODEN, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void InitializeADC_GPIOC(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void InitializeADC(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//ADC Initialisation
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ADC1 configuration */
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; //?
	ADC_InitStructure.ADC_ExternalTrigConvEdge= ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfConversion = 1;

	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_3Cycles);
}

void ADC1_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
	}
}

int main(void)
{
	InitializeLEDs();
	InitializeTimer();
	InitializeADC_GPIOC();
	InitializeADC();


	while(1)
	{
		ADC_SoftwareStartConv(ADC1);
		AD_value = ADC_GetConversionValue(ADC1); //Read ADC value
		if(AD_value > 200)
		{
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		}

//		pause(0x100000);

		ADC1_IRQHandler();
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		pause(0x100000);
	}
}
void pause(int32_t time)
{
	for(i = 0;i < time;i++){};
}
