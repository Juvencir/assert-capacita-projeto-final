/**
 ******************************************************************************
 *  @file            Bsp.c
 *  @addtogroup      Bsp
 *  @brief           Camada de abstracao de hardware para o projeto.
 *  @author          Guilherme Juvencio
 *  @brief           Implementacao da camada de abstracao de hardware.
 *                   Encapsula chamadas a HAL da ST para ADC, PWM, Timers e
 *                   USART3.
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include "Bsp.h"
#include "adc.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

/*---------------------------------------------------------------------------*/
/* Defines Locais                                                            */
/*---------------------------------------------------------------------------*/

/// @brief Periodo do TIM3 para PWM (0 a 99 passos de duty cycle)
#define dBSP_TIM3_PERIOD_TICKS      (99U)

/// @brief Timeout para HAL_UART_Receive em ticks de SysTick (1 ms)
#define dBSP_UART_RX_TIMEOUT        (1U)


/*---------------------------------------------------------------------------*/
/* Tipos Locais                                                              */
/*---------------------------------------------------------------------------*/

/// @brief Estrutura que agrupa as variaveis de estado do modulo BSP
typedef struct
{
    volatile bool tim6SampleFlag;    ///< Flag de estouro do TIM6 (5 ms)
    volatile bool tim7DebounceFlag;  ///< Flag de estouro do TIM7 (30 ms)
} Bsp_t;

/// @brief Instancia unica do estado do BSP
static Bsp_t bsp;


/*---------------------------------------------------------------------------*/
/* Funcoes Locais                                                            */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Converte um valor percentual (0 a 100) para o valor de comparacao
 *         do PWM (0 a dBSP_TIM3_PERIOD_TICKS).
 * @param  percent Valor percentual de duty cycle (0 a 100).
 * @retval uint32_t Valor de comparacao para o registo CCR do TIM3.
 */
static uint32_t Bsp_Pwm_PercentToCompare(uint8_t percent)
{
    uint32_t saturatedPercent;

    if (percent > 100)
    {
        saturatedPercent = 100;
    }
    else
    {
        saturatedPercent = percent;
    }

    return (saturatedPercent * (uint32_t)dBSP_TIM3_PERIOD_TICKS)
           / 100;
}


/*---------------------------------------------------------------------------*/
/* Callbacks da HAL                                                          */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Callback de periodo expirado dos timers.
 *         Distingue entre TIM6 (amostragem) e TIM7 (debounce) e levanta
 *         as respectivas flags para processamento no laco principal.
 * @param  htim Ponteiro para o handle do timer que gerou a interrupcao.
 * @note   Executada em contexto de ISR. Deve ser rapida.
 * @retval Nenhum.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        bsp.tim6SampleFlag = true;
    }
    else if (htim->Instance == TIM7)
    {
        bsp.tim7DebounceFlag = true;
    }
}

/*---------------------------------------------------------------------------*/
/* Funcoes Publicas                                                          */
/*---------------------------------------------------------------------------*/

void Bsp_Init(void)
{
    HAL_TIM_Base_Start_IT(&htim6);

    Bsp_Pwm_StartAll();
}

bool Bsp_Tim6_IsSampleFlag(void)
{
    return bsp.tim6SampleFlag;
}

void Bsp_Tim6_ClearSampleFlag(void)
{
    bsp.tim6SampleFlag = false;
}

void Bsp_Tim7_StartOnePulse(void)
{
    HAL_TIM_Base_Start_IT(&htim7);
}

bool Bsp_Tim7_IsDebounceFlag(void)
{
    return bsp.tim7DebounceFlag;
}

void Bsp_Tim7_ClearDebounceFlag(void)
{
    bsp.tim7DebounceFlag = false;
}

uint16_t Bsp_Adc_Read(void)
{
    HAL_StatusTypeDef status;

    HAL_ADC_Start(&hadc1);

    status = HAL_ADC_PollForConversion(&hadc1, dBSP_ADC_TIMEOUT_MS);

    if (status == HAL_OK)
    {
        return (uint16_t)HAL_ADC_GetValue(&hadc1);
    }

    HAL_ADC_Stop(&hadc1);
    return 0U;
}

void Bsp_Pwm_StartAll(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

void Bsp_Pwm_SetDutyCycle(eLedChannel_t channel, uint8_t percent)
{
    uint32_t compareValue;
    uint32_t halChannel;

    switch (channel)
    {
    case eLED_CHANNEL_1:
        halChannel = TIM_CHANNEL_1;
        break;
    case eLED_CHANNEL_2:
        halChannel = TIM_CHANNEL_2;
        break;
    case eLED_CHANNEL_3:
        halChannel = TIM_CHANNEL_3;
        break;
    default:
        return;
    }

    compareValue = Bsp_Pwm_PercentToCompare(percent);
    __HAL_TIM_SET_COMPARE(&htim3, halChannel, compareValue);
}

void Bsp_Uart_TransmitChar(uint8_t ch)
{
    HAL_UART_Transmit(&huart3, &ch, 1U, HAL_MAX_DELAY);
}

bool Bsp_Uart_IsRxReady(void)
{
    return (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE) != RESET);
}

uint8_t Bsp_Uart_ReadChar(void)
{
    uint8_t ch = 0U;

    HAL_UART_Receive(&huart3, &ch, 1U, dBSP_UART_RX_TIMEOUT);

    return ch;
}