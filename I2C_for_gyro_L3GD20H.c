#include "GPIO.h"
#include "I2C.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"

volatile unsigned short recieve[1024];
volatile int i = 1, x = 0;

void setup(I2C_TypeDef* I2Cx)
{
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB));
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2Cx,0x6B<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2Cx, 0x00);
	while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE));

	I2C_SendData(I2Cx,0x28);
	while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE));
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2Cx, ENABLE);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF)); // stop bit flag
	for(x = 0;x < 0xFFFFF;x++); //Delay
}

short read8(I2C_TypeDef* I2Cx){
	uint8_t msb=0, lsb=0;
	unsigned char dummy1;
	unsigned short data;

	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB));
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2Cx,0x6B<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2Cx,0x28);
	while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE));
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB));

	I2C_Send7bitAddress(I2Cx, 0x6B<<1, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	msb = I2C_ReceiveData(I2Cx);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);
	I2C_AcknowledgeConfig(I2Cx, DISABLE);

	lsb = I2C_ReceiveData(I2Cx);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

	//dummy1 = I2C_ReceiveData(I2Cx);
	//while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

	I2C_GenerateSTOP(I2Cx, ENABLE);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));


	data  = msb<<8;
	data |= lsb;

	return data;
}

void write8(I2C_TypeDef* I2Cx,uint8_t reg, uint8_t addr)
{
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2Cx,ENABLE);
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB));
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2Cx,0x6B<<1,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2Cx,reg);
	while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE));
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2Cx,addr);
	while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE));
	while(!I2C_CheckEvent(I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2Cx,ENABLE);
	while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_STOPF));
}

int main(void)
{
	I2C_Initialize(I2C2,I2C_Ack_Enable,I2C_AcknowledgedAddress_7bit, 100000, I2C_DutyCycle_2, I2C_Mode_I2C, 0x00);

	setup(I2C2);

	write8(I2C2,0x20,0x0F);

	unsigned short temp = 0;
	while(1)
	{
		temp = read8(I2C2);
		if(recieve[i-1] != temp){
			recieve[i++] = temp;
			if(i == 1024){
				i = 0;
			}
		}
	}
}
