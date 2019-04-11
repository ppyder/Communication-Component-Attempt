/**
 ******************************************************************************
 * @file    bsp_gpio.c
 * @brief   �弶���裺gpio��������������ܵ��������룬�������ʼ���Ͷ���ע�Ṧ��.
 ******************************************************************************
 *
 * Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018. All rights reserved.
 *
 * @version 1.0 ʾ���汾
 * @author  ���ෲ
 * @contact 17863107058(�ֻ�)   942041771(qq)
 * @date    2018/10/10
 *
 ******************************************************************************
 */
 
/* Includes ------------------------------------------------------------------*/
#include "bsp_gpio.h"

/* Private function prototypes -----------------------------------------------*/
static uint8_t CheckHighest1In8Bits(uint8_t Number);
static void ENABLE_GPIO_CLK(GPIO_TypeDef *GPIOx);
uint32_t PinBit2PinNum(uint32_t PinBit);
uint32_t PinNum2PinBit(uint32_t PinNum);

/**
 * @brief	    ȷ��һ����λ������������߷���λ��λ�ã����λ���Ϊ0�����ҵ������ε��������λΪ7��
 * @param[in]	Number - ��������λ��������
 * @retval	    λ�ñ��(uint8_t)
 */
static uint8_t CheckHighest1In8Bits(uint8_t Number)
{
	int8_t cnt = 0;
	uint8_t result = 0;
	
	for(cnt = (8 - 1); cnt >= 0; cnt--)
	{
		if((Number >> cnt) == 1)
		{
			result = cnt;			//�Ӹ�λ����λ�������һ���������λ
			break;
		}
	}
	
	return result;
}

/**
 * @brief	    ������λ���ת��Ϊ�������ֱ��
 * @param[in]	PinBit	��ת���Ĳ���
 * @retval	    λ�ñ��(uint8_t)
 * @note	    ���ڲ�����
 *			    �ڹٷ�Ϊλ���, �����ֱ�ţ����ֱ�ŷ����Ķ��� @ref GPIO_pins_define
 */
uint32_t PinBit2PinNum(uint32_t PinBit)
{
	uint32_t result = 0;
	
	PinBit &= GPIO_PIN_MASK;	//ȡ��ʮ��λ
	
	if((PinBit >> 8) != 0)
	{
		result = CheckHighest1In8Bits((uint8_t)(PinBit >> 8)) + 8;
	}
	else
	{
		result = CheckHighest1In8Bits((uint8_t)(PinBit & 0xFF));
	}
	
	return result;
}

/**
 * @brief	    ���������ֱ��ת��Ϊ����λ���
 * @param[in]	PinNum	��ת���Ĳ���
 * @retval	    ����λ���(uint32_t)
 * @note	    ���ڲ�����
 *			    �ڹٷ�Ϊλ���, �����ֱ�ţ����ֱ�ŷ����Ķ��� @ref GPIO_pins_define
 */
uint32_t PinNum2PinBit(uint32_t PinNum)
{
	return (0x01 << PinNum);
}

/**
 * @brief	    ����IO��Ӧ������ʱ��
 * @param[in]   GPIOx	ָ��GPIO�˿�	(GPIOx, x������A -> F)
 * @retval	    None
 */
static void ENABLE_GPIO_CLK(GPIO_TypeDef *GPIOx)
{
	switch((uint32_t)GPIOx)
	{
		case (uint32_t)GPIOA:
						if(__HAL_RCC_GPIOA_IS_CLK_DISABLED())	//����ʱ�ӻ�δ������
							__HAL_RCC_GPIOA_CLK_ENABLE();  
                   	    break;
						
		case (uint32_t)GPIOB:
						if(__HAL_RCC_GPIOB_IS_CLK_DISABLED())	//����ʱ�ӻ�δ������
							__HAL_RCC_GPIOB_CLK_ENABLE();  
                   	    break;
						
		case (uint32_t)GPIOC:
						if(__HAL_RCC_GPIOC_IS_CLK_DISABLED())	//����ʱ�ӻ�δ������
							__HAL_RCC_GPIOC_CLK_ENABLE();  
                   	    break;	
						
		case (uint32_t)GPIOD:
						if(__HAL_RCC_GPIOD_IS_CLK_DISABLED())	//����ʱ�ӻ�δ������
							__HAL_RCC_GPIOD_CLK_ENABLE();  
                   	    break;
						
		case (uint32_t)GPIOE:
						if(__HAL_RCC_GPIOE_IS_CLK_DISABLED())	//����ʱ�ӻ�δ������
							__HAL_RCC_GPIOE_CLK_ENABLE();  
                   	    break;
						
		case (uint32_t)GPIOF:
						if(__HAL_RCC_GPIOF_IS_CLK_DISABLED())	//����ʱ�ӻ�δ������
							__HAL_RCC_GPIOF_CLK_ENABLE();  
                   	    break; 
						
		case (uint32_t)GPIOG:
						if(__HAL_RCC_GPIOG_IS_CLK_DISABLED())	//����ʱ�ӻ�δ������
							__HAL_RCC_GPIOG_CLK_ENABLE();  
                   	    break;
						
		default: break;
	}
}

/**
 * @brief	    ����GPIOΪ��ͨ���빦��
 * @param[in]   GPIOx		ָ��GPIO�˿�			(GPIOx, x������A -> F)
 * @param[in]   GPIO_Pin    ָ��GPIO���ź�(λ���)	@ref GPIO_pins_define
 * @param[in]   InputMode	ָ������ģʽ		    @ref GPIO_mode_define
 * @param[in]   Pull		ָ��������				@ref GPIO_pull_define
 * @retval	    None
 */
void Input_GPIO_Init(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t InputMode, uint32_t Pull)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//������ӦIO��ʱ��
	ENABLE_GPIO_CLK(GPIOx);
	
	//ָ�����ź�
    GPIO_InitStruct.Pin = GPIO_Pin;
	
	//ָ��ģʽ
	GPIO_InitStruct.Mode = InputMode;
	
	//ָ��������
	GPIO_InitStruct.Pull = Pull;
	
	//����ָ��������ʼ����ӦIO
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
 * @brief	    ����GPIOΪ��ͨ�������
 * @param[in]   GPIOx		ָ��GPIO�˿�		    (GPIOx, x������A -> F)
 * @param[in]   GPIO_Pin    ָ��GPIO���ź�(λ���)	@ref GPIO_pins_define
 * @param[in]   OutputMode	ָ�����ģʽ		    @ref GPIO_mode_define
 * @param[in]   Speed		ָ���������		    @ref GPIO_speed_define
 * @param[in]   Initstate	ָ����ʼ�����ƽ	    @ref GPIO_PinState
 * @retval	    None
 */
void Output_GPIO_Init(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t OutputMode, uint32_t Speed, GPIO_PinState Initstate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//������ӦIO��ʱ��
	ENABLE_GPIO_CLK(GPIOx);
	
	//ָ�����ź�
    GPIO_InitStruct.Pin = GPIO_Pin;
	
	//ָ�����ģʽ
	GPIO_InitStruct.Mode = OutputMode;
	
	//ָ���������
	GPIO_InitStruct.Speed = Speed;

	//����ָ��������ʼ����ӦIO
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	
	//ָ��IO��ʼ״̬
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, Initstate);
}

/**
 * @brief	    ע��һ����Ϊ��ͨ���빦�ܵ�IO
 * @param[out]  pDevice		ָ��һ���豸�ṹ���ָ��
 * @param[in]   ID			���豸�ı�ţ������
 * @param[in]   Name		�Ը��豸������������
 * @param[in]   GPIOx		ָ��GPIO�˿�			    (GPIOx, x������A -> F)
 * @param[in]   GPIO_Pin    ָ��GPIO���ź�(λ���)	@ref GPIO_pins_define
 * @param[in]   OutputMode	ָ�����ģʽ			    @ref GPIO_mode_define
 * @param[in]   Speed		ָ���������			    @ref GPIO_speed_define
 * @param[in]   Initstate	ָ����ʼ�����ƽ		    @ref GPIO_PinState
 * @retval	    None
 */
void Register_InputIO(IO_In* pDevice, uint32_t ID, char* Name,
						GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t InputMode, uint32_t Pull)
{
	//��ʼ����Ӧ����
	Input_GPIO_Init(GPIOx, GPIO_Pin, InputMode, Pull);

	if(pDevice == NULL)		//�ڴ氲ȫ����
		return;
	
	//��ʼ�����ݽṹ��
	pDevice -> Basic.ID = ID;
	pDevice -> Basic.Name = Name;
	
	pDevice -> filter.History = 0;	//����˲���
	pDevice -> filter.State = false;
	
	pDevice ->GPIOx = GPIOx;
	pDevice ->GPIO_Pin_Num = PinBit2PinNum(GPIO_Pin);	//�������ű��
	
	pDevice -> Operator = &BIT_ADDR((uint32_t)GPIOx + ODR_Addr_Offset, pDevice ->GPIO_Pin_Num);
};

/**
 * @brief	    ע��һ����Ϊ��ͨ������ܵ�IO
 * @param[out]  pDevice		ָ��һ���豸�ṹ���ָ��
 * @param[in]   ID			���豸�ı�ţ������
 * @param[in]   Name		�Ը��豸������������
 * @param[in]   GPIOx		ָ��GPIO�˿�			    (GPIOx, x������A -> F)
 * @param[in]   GPIO_Pin    ָ��GPIO���ź�(λ���)	@ref GPIO_pins_define
 * @param[in]   InputMode	ָ������ģʽ			    @ref GPIO_mode_define
 * @param[in]   Pull		ָ��������				@ref GPIO_pull_define
 * @retval	    None
 */
void Register_OutputIO(IO_Out* pDevice, uint32_t ID, char* Name,
						GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t OutputMode, uint32_t Speed, GPIO_PinState Initstate)
{
	//��ʼ����Ӧ����
	Output_GPIO_Init(GPIOx, GPIO_Pin, OutputMode, Speed, Initstate);

	if(pDevice == NULL)		//�ڴ氲ȫ����
		return;
	
	//��ʼ�����ݽṹ��
	pDevice -> Basic.ID = ID;
	pDevice -> Basic.Name = Name;

	pDevice ->GPIOx = GPIOx;
	pDevice ->GPIO_Pin_Num = PinBit2PinNum(GPIO_Pin);	//�������ű��
	
	pDevice -> Operator = &BIT_ADDR((uint32_t)GPIOx + ODR_Addr_Offset, pDevice ->GPIO_Pin_Num);
};

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/
