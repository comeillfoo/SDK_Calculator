/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "kb.h"
#include "sdk_uart.h"
#include "pca9538.h"
#include "oled.h"
#include "fonts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef int operator( const int, const int );

int add( const int arg1, const int arg2 ) {
	return arg1 + arg2;
}

int mul( const int arg1, const int arg2 ) {
	return arg1 * arg2;
}

int sub( const int arg1, const int arg2 ) {
	return arg1 - arg2;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ROW1 0x1E
#define ROW2 0x3D
#define ROW3 0x7B
#define ROW4 0xF7
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
enum BTNS {
	ONE = 0,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	ZERO,
	OPCODE,
	EQUAL,
	NONE
};

const static enum BTNS btns[][3] = {
		{ ONE, TWO, THREE     },
		{ FOUR, FIVE, SIX     },
		{ SEVEN, EIGHT, NINE  },
		{ OPCODE, ZERO, EQUAL }
};

const static char btns2chars[] = {
		[ ONE ] = '1',
		[ TWO ] = '2',
		[ THREE ] = '3',
		[ FOUR ] = '4',
		[ FIVE ] = '5',
		[ SIX ] = '6',
		[ SEVEN ] = '7',
		[ EIGHT ] = '8',
		[ NINE ] = '9',
		[ ZERO ] = '0',
		[ OPCODE ] = '+',
		[ EQUAL ] = '='
};

enum OPCODES {
		OP_ADD,
		OP_SUB,
		OP_MUL
};

const static const char op2char[] = {
		[ OP_ADD ] = '+',
		[ OP_SUB ] = '-',
		[ OP_MUL ] = 'x'
};

const static char btns2ints[] = {
		[ ONE ] = 1,
		[ TWO ] = 2,
		[ THREE ] = 3,
		[ FOUR ] = 4,
		[ FIVE ] = 5,
		[ SIX ] = 6,
		[ SEVEN ] = 7,
		[ EIGHT ] = 8,
		[ NINE ] = 9,
		[ ZERO ] = 0,
};

const static operator* operators[] = {
		[ OP_ADD ] = add,
		[ OP_SUB ] = sub,
		[ OP_MUL ] = mul
};

const static uint8_t delay = 10;
const static uint8_t rows[] = { ROW1, ROW2, ROW3, ROW4 };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void KB_Test( void );
void OLED_KB( uint8_t OLED_Keys[]);
void oled_Reset( void );

enum BTNS KB_GetBtn() {
	for ( size_t i = 0; i < 4; ++i ) {
		// убираем дребезг
		uint8_t OldKey = Check_Row( rows[i] );
		HAL_Delay( delay );
		uint8_t Key = Check_Row( rows[i] );
		Key = OldKey & Key & 0b111;
		if ( Key > 0 ) {
			uint8_t left, centre, right;
			left   = ( Key >> 2 ) & 1;
			centre = ( Key >> 1 ) & 1;
			right  = ( Key >> 0 ) & 1;
			if ( left )
				return btns[ i ][ 0 ];
			else if ( centre )
				return btns[ i ][ 1 ];
			else if ( right )
				return btns[ i ][ 2 ];
		}
	}
	return NONE;
}

int get3DigitNumber() {
	int arg = 0;
	enum BTNS pressed = NONE;
  int numLength = 0;
  do {
	  pressed = KB_GetBtn();
	  if ( pressed == NONE )
		  continue;
	  if ( pressed != EQUAL && pressed != OPCODE ) {
		  oled_WriteChar( btns2chars[ pressed ], Font_16x26, White );
		  oled_UpdateScreen();
		  arg = arg * 10 + btns2ints[ pressed ];
		  numLength++;
		  HAL_Delay( delay );
	  }

	  if ( numLength > 0 && pressed == EQUAL )
		  return arg;
  } while ( numLength < 3 );
  return arg;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  oled_Init();

  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */
	  	// KB_Test();
	  	  oled_WriteString( "Enter first:", Font_7x10, White );
	  	  oled_UpdateScreen();
	  	  HAL_Delay( delay );
	  	  oled_SetCursor( 0, 11 );
	  	  int arg1 = get3DigitNumber();
	  	  oled_Reset();
	  	  oled_WriteString( "Enter second:", Font_7x10, White );
	  	  oled_UpdateScreen();
	  	  HAL_Delay( delay );
	  	  oled_SetCursor( 0, 11 );
	  	  int arg2 = get3DigitNumber();
	  	  oled_Reset();
	  	  oled_WriteString( "Switch operator:", Font_7x10, White );
	  	  oled_UpdateScreen();
	  	  HAL_Delay( delay );
	  	  oled_SetCursor( 0, 11 );
	  	  enum BTNS pressed = NONE;
	  	  enum OPCODES op = OP_MUL;
	  	  do {
	  		  pressed = KB_GetBtn();
	  		  if ( pressed == OPCODE ) {
	  			  op = ( op + 1 ) % 3;
	  			  oled_Reset();
	  			  oled_WriteString( "Switch operator:", Font_7x10, White );
	  			  oled_UpdateScreen();
	  			  HAL_Delay( delay );
	  			  oled_SetCursor( 55, 22 );
	  			  oled_WriteChar( op2char[ op ], Font_16x26, White );
	  			  oled_UpdateScreen();
	  		  }
	  	  } while ( pressed != EQUAL );
	  	  oled_Reset();
	  	  oled_WriteString( "Result:", Font_7x10, White );
	  	  oled_UpdateScreen();
	  	  HAL_Delay( delay );
	  	  oled_SetCursor( 0, 11 );
	  	  int result = operators[ op ]( arg1, arg2 );
	  	  char buffer[ 7 ];
	  	  itoa( result, buffer, 10 );
	  	  oled_WriteString( buffer, Font_16x26, White );
	  	  oled_UpdateScreen();
	  	  while( KB_GetBtn() != EQUAL );
	  	  oled_Reset();
	    }
	    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void KB_Test( void ) {
	UART_Transmit( (uint8_t*)"KB test start\n" );
	uint8_t R = 0, C = 0, L = 0, Row[4] = {0xF7, 0x7B, 0x3D, 0x1E}, Key, OldKey, OLED_Keys[12] = {0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30};
	oled_Reset();
	oled_WriteString("From bottom to top", Font_7x10, White);
	OLED_KB(OLED_Keys);
	oled_UpdateScreen();
	for ( int i = 0; i < 4; i++ ) {
		while( !( R && C && L ) ) {
			OldKey = Key;
			Key = Check_Row( Row[i] );
			if ( Key == 0x01 && Key != OldKey) {
				UART_Transmit( (uint8_t*)"Right pressed\n" );
				R = 1;
				OLED_Keys[2+3*i] = 0x31;
				OLED_KB(OLED_Keys);
			} else if ( Key == 0x02 && Key != OldKey) {
				UART_Transmit( (uint8_t*)"Center pressed\n" );
				C = 1;
				OLED_Keys[1+3*i] = 0x31;
				OLED_KB(OLED_Keys);
			} else if ( Key == 0x04 && Key != OldKey) {
				UART_Transmit( (uint8_t*)"Left pressed\n" );
				L = 1;
				OLED_Keys[3*i] = 0x31;
				OLED_KB(OLED_Keys);
			}
		}
		R = C = L = 0;
		HAL_Delay(25);
	}
	UART_Transmit( (uint8_t*)"KB test complete\n");
}
void OLED_KB( uint8_t OLED_Keys[12]) {
	for (int i = 3; i >= 0; i--) {
		oled_SetCursor(56, 5+(4-i)*10);
		for (int j = 0; j < 3; j++) {
			oled_WriteChar(OLED_Keys[j+3*i], Font_7x10, White);
		}
	}
	oled_UpdateScreen();
}
void oled_Reset( void ) {
	oled_Fill(Black);
	oled_SetCursor(0, 0);
	oled_UpdateScreen();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
