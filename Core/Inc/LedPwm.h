/**
 ******************************************************************************
 *  @file            LedPwm.h
 *  @addtogroup      LedPwm
 *  @brief           Modulo de gerencia de duty cycle dos LEDs.
 *  @author          Guilherme Juvencio
 *  @brief           Interface publica do modulo de controle PWM dos LEDs.
 *                   Armazena o duty cycle individual de cada LED, controla
 *                   qual deles esta ativo e atualiza apenas o canal
 *                   selecionado, preservando os demais. Nao acessa hardware
 *                   diretamente (delega ao BSP).
 ******************************************************************************
 */

#ifndef dLEDPWM_H
#define dLEDPWM_H


/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include "Bsp.h"

/*---------------------------------------------------------------------------*/
/* Defines Publicos                                                          */
/*---------------------------------------------------------------------------*/

/// @brief Numero de canais de LED controlados pelo modulo
#define dLEDPWM_CHANNEL_COUNT       (3U)


/*---------------------------------------------------------------------------*/
/* Prototipos das Funcoes Publicas                                           */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Inicializa o modulo zerando o duty cycle de todos os LEDs
 *         e definindo o LED 1 como ativo.
 * @note   Deve ser chamada apos Bsp_Init.
 * @retval Nenhum.
 */
void LedPwm_Init(void);

/**
 * @brief  Altera o LED ativo para receber atualizacoes de duty cycle.
 *         O LED que estava ativo congela no seu ultimo valor.
 * @param  led Canal do LED a ser ativado (eLED_CHANNEL_1, _2 ou _3).
 * @note   Nao altera os valores de duty cycle armazenados.
 * @retval Nenhum.
 */
void LedPwm_SetActiveLed(eLedChannel_t led);

/**
 * @brief  Retorna qual LED esta atualmente selecionado.
 * @retval eLedChannel_t Canal do LED ativo.
 */
eLedChannel_t LedPwm_GetActiveLed(void);

/**
 * @brief  Atualiza o duty cycle do LED ativo e aplica ao hardware via BSP.
 * @param  percent Valor percentual do duty cycle (0 a 100).
 * @note   Apenas o LED ativo e afetado. Os demais permanecem inalterados.
 * @retval Nenhum.
 */
void LedPwm_UpdateActiveDuty(uint8_t percent);

/**
 * @brief  Retorna o duty cycle armazenado de um LED especifico.
 * @param  led Canal do LED a consultar (eLED_CHANNEL_1, _2 ou _3).
 * @retval uint8_t Valor percentual armazenado (0 a 100).
 */
uint8_t LedPwm_GetDuty(eLedChannel_t led);

#endif /* dLEDPWM_H */
