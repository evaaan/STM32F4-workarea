/**
  ******************************************************************************
  * @file    DCMI/DCMI_CameraExample/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dcmi_ov9655.h"
#include "dcmi_ov2640.h"
#include  "lcd_log.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup DCMI_CameraExample
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
OV9655_IDTypeDef  OV9655_Camera_ID;
OV2640_IDTypeDef  OV2640_Camera_ID;

__IO uint16_t  uhADCVal = 0;
uint8_t        abuffer[40];
uint16_t dcmi_it;
uint32_t c_high = 0;
uint32_t c_low = 0;
uint16_t on = 1;
uint16_t j;
FlagStatus fstatus = RESET;
ITStatus itstatus = RESET;

uint32_t screenBuffer[120*160*2/4];
uint32_t screenBufferAddr = (uint32_t)(screenBuffer);

extern Camera_TypeDef       Camera;
extern ImageFormat_TypeDef  ImageFormat;
extern __IO uint8_t         ValueMax;
extern const uint8_t *      ImageForematArray[];

/* Private function prototypes -----------------------------------------------*/
//static void ADC_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40xx.s/startup_stm32f427x.s) before to branch to 
       application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  /* Initialize LEDs and push-buttons mounted on STM324xG-EVAL/STM324x7I-EVAL boards */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED3);

	Camera = OV9655_CAMERA;



  /* Configure the Camera module mounted on STM324xG-EVAL/STM324x7I-EVAL boards */
  OV9655_HW_Init(); // Initialize GPIO pins (DCMI, I2C)
	OV9655_Init(BMP_QQVGA); // Initialize DCMI and DMA registers
	OV9655_QQVGAConfig(); // Program OV9655 registers

	

  /* Enable DMA2 stream 1 and DCMI interface then start image capture */
  DMA_Cmd(DMA2_Stream1, ENABLE); 

  DCMI_Cmd(ENABLE); 

	Delay(100);
  DCMI_CaptureCmd(ENABLE); 

	
	// lets try writing to and reading from a register
	/*if (Camera == OV9655_CAMERA) {
		ret_val = OV9655_ReadReg(OV9655_COM10);
	} */
	
  while(1)
  {
    // Toggle LEDs
    STM_EVAL_LEDToggle(LED1);
    STM_EVAL_LEDToggle(LED2);
    STM_EVAL_LEDToggle(LED3);
    STM_EVAL_LEDToggle(LED4);
		/*
		itstatus = DCMI_GetITStatus(dcmi_it);
		if (itstatus == SET) {
			//Delay(100);//catch here
			DCMI_ClearITPendingBit(dcmi_it);
		} */
		Delay(10);
		
		/*
		// capture a single frame
		
		// Set VSYNC low to start frame
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
		
		Delay(1);
			// read 8 pulses of PCLK then set HSYNC high
		for (j = 0; j < 8; j++) {
			// Set HSYNC high while taking data
			GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
			Delay(1);
			GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
			Delay(1);
			
		}
		//if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == Bit_SET)
		
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
		*/
  }
}

/**
  * @brief  Configures the ADC.
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable ADC3 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  /* GPIOF clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 

  /* Configure ADC Channel7 as analog */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* ADC Common Init */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; 
  ADC_CommonInit(&ADC_CommonInitStructure); 

  /* ADC3 Configuration ------------------------------------------------------*/
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC3 Regular Channel Config */
  ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_56Cycles);

  /* Enable ADC3 */
  ADC_Cmd(ADC3, ENABLE);

  /* ADC3 regular Software Start Conv */ 
  ADC_SoftwareStartConv(ADC3);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
