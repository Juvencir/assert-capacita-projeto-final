/**
 ******************************************************************************
 *  @file            LedPwm.c
 *  @addtogroup      LedPwm
 *  @brief           Modulo de gerencia de duty cycle dos LEDs.
 *  @author          Guilherme Juvencio
 *  @brief           Implementacao do modulo de controle PWM dos LEDs.
 *                   Armazena o duty cycle individual de cada LED, controla
 *                   qual deles esta ativo e atualiza apenas o canal
 *                   selecionado, preservando os demais. Delega a escrita
 *                   no hardware ao BSP.
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include "LedPwm.h"

/*---------------------------------------------------------------------------*/
/* Defines Locais                                                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Tipos Locais                                                              */
/*---------------------------------------------------------------------------*/

/// @brief Estrutura que agrupa as variaveis de estado do modulo LedPwm
typedef struct
{
    uint8_t       dutyCycle[dLEDPWM_CHANNEL_COUNT]; ///< Duty cycle individual (0 a 100)
    eLedChannel_t activeLed;                         ///< LED atualmente selecionado
} LedPwm_t;

/// @brief Instancia unica do estado do LedPwm
static LedPwm_t ledPwm;


/*---------------------------------------------------------------------------*/
/* Funcoes Locais                                                            */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Converte o canal logico (eLedChannel_t) para indice do array
 *         interno (0 a 2).
 * @param  led Canal do LED.
 * @retval uint8_t Indice 0, 1 ou 2 correspondente ao canal.
 */
static uint8_t LedPwm_ChannelToIndex(eLedChannel_t led)
{
    uint8_t index;

    switch (led)
    {
    case eLED_CHANNEL_1:
        index = 0U;
        break;
    case eLED_CHANNEL_2:
        index = 1U;
        break;
    case eLED_CHANNEL_3:
        index = 2U;
        break;
    default:
        index = 0U;
        break;
    }

    return index;
}


/*---------------------------------------------------------------------------*/
/* Funcoes Publicas                                                          */
/*---------------------------------------------------------------------------*/

void LedPwm_Init(void)
{
    uint8_t i;

    for (i = 0U; i < dLEDPWM_CHANNEL_COUNT; i++)
    {
        ledPwm.dutyCycle[i] = 0U;
    }

    ledPwm.activeLed = eLED_CHANNEL_1;
}

void LedPwm_SetActiveLed(eLedChannel_t led)
{
    if ((led >= eLED_CHANNEL_1) && (led <= eLED_CHANNEL_3))
    {
        ledPwm.activeLed = led;
    }
}

eLedChannel_t LedPwm_GetActiveLed(void)
{
    return ledPwm.activeLed;
}

void LedPwm_UpdateActiveDuty(uint8_t percent)
{
    uint8_t index;

    index = LedPwm_ChannelToIndex(ledPwm.activeLed);

    ledPwm.dutyCycle[index] = percent;

    Bsp_Pwm_SetDutyCycle(ledPwm.activeLed, percent);
}

uint8_t LedPwm_GetDuty(eLedChannel_t led)
{
    uint8_t index;

    index = LedPwm_ChannelToIndex(led);

    return ledPwm.dutyCycle[index];
}
