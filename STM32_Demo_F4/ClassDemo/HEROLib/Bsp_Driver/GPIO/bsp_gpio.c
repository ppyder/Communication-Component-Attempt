/**
 ******************************************************************************
 * @file    bsp_gpio.c
 * @brief   板级外设：gpio用作输入输出功能的驱动代码，包含其初始化和对象注册功能.
 ******************************************************************************
 *
 * Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018. All rights reserved.
 *
 * @version 1.0 示例版本
 * @author  杨亦凡
 * @contact 17863107058(手机)   942041771(qq)
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
 * @brief	    确定一个八位二进制数中最高非零位的位置（最低位标号为0，从右到左依次递增，最高位为7）
 * @param[in]	Number - 待检测的四位二进制数
 * @retval	    位置标号(uint8_t)
 */
static uint8_t CheckHighest1In8Bits(uint8_t Number)
{
	int8_t cnt = 0;
	uint8_t result = 0;
	
	for(cnt = (8 - 1); cnt >= 0; cnt--)
	{
		if((Number >> cnt) == 1)
		{
			result = cnt;			//从高位往低位，捕获第一个不是零的位
			break;
		}
	}
	
	return result;
}

/**
 * @brief	    将引脚位表达转换为引脚数字编号
 * @param[in]	PinBit	待转换的参量
 * @retval	    位置标号(uint8_t)
 * @note	    ①内部调用
 *			    ②官方为位表达, 非数字编号（数字编号方便阅读） @ref GPIO_pins_define
 */
uint32_t PinBit2PinNum(uint32_t PinBit)
{
	uint32_t result = 0;
	
	PinBit &= GPIO_PIN_MASK;	//取后十六位
	
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
 * @brief	    将引脚数字编号转换为引脚位表达
 * @param[in]	PinNum	待转换的参量
 * @retval	    引脚位表达(uint32_t)
 * @note	    ①内部调用
 *			    ②官方为位表达, 非数字编号（数字编号方便阅读） @ref GPIO_pins_define
 */
uint32_t PinNum2PinBit(uint32_t PinNum)
{
	return (0x01 << PinNum);
}

/**
 * @brief	    开启IO对应的外设时钟
 * @param[in]   GPIOx	指定GPIO端口	(GPIOx, x可以是A -> F)
 * @retval	    None
 */
static void ENABLE_GPIO_CLK(GPIO_TypeDef *GPIOx)
{
	switch((uint32_t)GPIOx)
	{
		case (uint32_t)GPIOA:
						if(__HAL_RCC_GPIOA_IS_CLK_DISABLED())	//若该时钟还未被开启
							__HAL_RCC_GPIOA_CLK_ENABLE();  
                   	    break;
						
		case (uint32_t)GPIOB:
						if(__HAL_RCC_GPIOB_IS_CLK_DISABLED())	//若该时钟还未被开启
							__HAL_RCC_GPIOB_CLK_ENABLE();  
                   	    break;
						
		case (uint32_t)GPIOC:
						if(__HAL_RCC_GPIOC_IS_CLK_DISABLED())	//若该时钟还未被开启
							__HAL_RCC_GPIOC_CLK_ENABLE();  
                   	    break;	
						
		case (uint32_t)GPIOD:
						if(__HAL_RCC_GPIOD_IS_CLK_DISABLED())	//若该时钟还未被开启
							__HAL_RCC_GPIOD_CLK_ENABLE();  
                   	    break;
						
		case (uint32_t)GPIOE:
						if(__HAL_RCC_GPIOE_IS_CLK_DISABLED())	//若该时钟还未被开启
							__HAL_RCC_GPIOE_CLK_ENABLE();  
                   	    break;
						
		case (uint32_t)GPIOF:
						if(__HAL_RCC_GPIOF_IS_CLK_DISABLED())	//若该时钟还未被开启
							__HAL_RCC_GPIOF_CLK_ENABLE();  
                   	    break; 
						
		case (uint32_t)GPIOG:
						if(__HAL_RCC_GPIOG_IS_CLK_DISABLED())	//若该时钟还未被开启
							__HAL_RCC_GPIOG_CLK_ENABLE();  
                   	    break;
						
		default: break;
	}
}

/**
 * @brief	    配置GPIO为普通输入功能
 * @param[in]   GPIOx		指定GPIO端口			(GPIOx, x可以是A -> F)
 * @param[in]   GPIO_Pin    指定GPIO引脚号(位表达)	@ref GPIO_pins_define
 * @param[in]   InputMode	指定输入模式		    @ref GPIO_mode_define
 * @param[in]   Pull		指定上下拉				@ref GPIO_pull_define
 * @retval	    None
 */
void Input_GPIO_Init(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t InputMode, uint32_t Pull)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//开启对应IO的时钟
	ENABLE_GPIO_CLK(GPIOx);
	
	//指定引脚号
    GPIO_InitStruct.Pin = GPIO_Pin;
	
	//指定模式
	GPIO_InitStruct.Mode = InputMode;
	
	//指定上下拉
	GPIO_InitStruct.Pull = Pull;
	
	//按照指定参数初始化对应IO
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
 * @brief	    配置GPIO为普通输出功能
 * @param[in]   GPIOx		指定GPIO端口		    (GPIOx, x可以是A -> F)
 * @param[in]   GPIO_Pin    指定GPIO引脚号(位表达)	@ref GPIO_pins_define
 * @param[in]   OutputMode	指定输出模式		    @ref GPIO_mode_define
 * @param[in]   Speed		指定输出速率		    @ref GPIO_speed_define
 * @param[in]   Initstate	指定初始输出电平	    @ref GPIO_PinState
 * @retval	    None
 */
void Output_GPIO_Init(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t OutputMode, uint32_t Speed, GPIO_PinState Initstate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//开启对应IO的时钟
	ENABLE_GPIO_CLK(GPIOx);
	
	//指定引脚号
    GPIO_InitStruct.Pin = GPIO_Pin;
	
	//指定输出模式
	GPIO_InitStruct.Mode = OutputMode;
	
	//指定输出速率
	GPIO_InitStruct.Speed = Speed;

	//按照指定参数初始化对应IO
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	
	//指定IO初始状态
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, Initstate);
}

/**
 * @brief	    注册一个作为普通输入功能的IO
 * @param[out]  pDevice		指向一个设备结构体的指针
 * @param[in]   ID			该设备的编号（句柄）
 * @param[in]   Name		对该设备的文字性描述
 * @param[in]   GPIOx		指定GPIO端口			    (GPIOx, x可以是A -> F)
 * @param[in]   GPIO_Pin    指定GPIO引脚号(位表达)	@ref GPIO_pins_define
 * @param[in]   OutputMode	指定输出模式			    @ref GPIO_mode_define
 * @param[in]   Speed		指定输出速率			    @ref GPIO_speed_define
 * @param[in]   Initstate	指定初始输出电平		    @ref GPIO_PinState
 * @retval	    None
 */
void Register_InputIO(IO_In* pDevice, uint32_t ID, char* Name,
						GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t InputMode, uint32_t Pull)
{
	//初始化对应外设
	Input_GPIO_Init(GPIOx, GPIO_Pin, InputMode, Pull);

	if(pDevice == NULL)		//内存安全保护
		return;
	
	//初始化数据结构体
	pDevice -> Basic.ID = ID;
	pDevice -> Basic.Name = Name;
	
	pDevice -> filter.History = 0;	//清空滤波器
	pDevice -> filter.State = false;
	
	pDevice ->GPIOx = GPIOx;
	pDevice ->GPIO_Pin_Num = PinBit2PinNum(GPIO_Pin);	//保存引脚编号
	
	pDevice -> Operator = &BIT_ADDR((uint32_t)GPIOx + ODR_Addr_Offset, pDevice ->GPIO_Pin_Num);
};

/**
 * @brief	    注册一个作为普通输出功能的IO
 * @param[out]  pDevice		指向一个设备结构体的指针
 * @param[in]   ID			该设备的编号（句柄）
 * @param[in]   Name		对该设备的文字性描述
 * @param[in]   GPIOx		指定GPIO端口			    (GPIOx, x可以是A -> F)
 * @param[in]   GPIO_Pin    指定GPIO引脚号(位表达)	@ref GPIO_pins_define
 * @param[in]   InputMode	指定输入模式			    @ref GPIO_mode_define
 * @param[in]   Pull		指定上下拉				@ref GPIO_pull_define
 * @retval	    None
 */
void Register_OutputIO(IO_Out* pDevice, uint32_t ID, char* Name,
						GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t OutputMode, uint32_t Speed, GPIO_PinState Initstate)
{
	//初始化对应外设
	Output_GPIO_Init(GPIOx, GPIO_Pin, OutputMode, Speed, Initstate);

	if(pDevice == NULL)		//内存安全保护
		return;
	
	//初始化数据结构体
	pDevice -> Basic.ID = ID;
	pDevice -> Basic.Name = Name;

	pDevice ->GPIOx = GPIOx;
	pDevice ->GPIO_Pin_Num = PinBit2PinNum(GPIO_Pin);	//保存引脚编号
	
	pDevice -> Operator = &BIT_ADDR((uint32_t)GPIOx + ODR_Addr_Offset, pDevice ->GPIO_Pin_Num);
};

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/
