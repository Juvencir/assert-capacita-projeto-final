/**
 ******************************************************************************
 *  @file            Button.h
 *  @addtogroup      Button
 *  @brief           Modulo de gerencia do botao de usuario (USER_Btn).
 *  @author          Guilherme Juvencio
 *  @brief           Interface publica do modulo do botao.
 *                   Implementa maquina de estados toggle com debounce
 *                   nao-bloqueante via EXTI13 e TIM7. Exibe a flag de
 *                   congelamento para o resto do sistema.
 ******************************************************************************
 */

#ifndef dBUTTON_H
#define dBUTTON_H


/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include <stdbool.h>

/*---------------------------------------------------------------------------*/
/* Defines Publicos                                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Prototipos das Funcoes Publicas                                           */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Inicializa o estado do botao e habilita a interrupcao EXTI13
 *         no NVIC.
 * @note   Deve ser chamada apos MX_GPIO_Init e Bsp_Init.
 * @retval Nenhum.
 */
void Button_Init(void);

/**
 * @brief  Retorna o estado atual de congelamento do sistema.
 * @note   Alterna a cada pressionamento valido do botao (toggle).
 * @retval true  Se o sistema estiver congelado (STATE: OFF).
 * @retval false Se o sistema estiver em funcionamento normal (STATE: ON).
 */
bool Button_IsFrozen(void);

/**
 * @brief  Processa a maquina de estados do botao.
 *         Verifica a flag de debounce do TIM7 e, ao detectar o fim
 *         dos 30 ms, alterna o estado de congelamento e re-arma a EXTI.
 * @note   Deve ser chamada a cada iteracao do laco principal.
 * @retval Nenhum.
 */
void Button_Process(void);

#endif /* dBUTTON_H */
