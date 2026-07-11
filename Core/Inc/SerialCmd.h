/**
 ******************************************************************************
 *  @file            SerialCmd.h
 *  @addtogroup      SerialCmd
 *  @brief           Modulo de recepcao e interpretacao de comandos seriais.
 *  @author          Guilherme Juvencio
 *  @brief           Interface publica do parser de comandos UART.
 *                   Acumula caracteres recebidos via USART3 de forma
 *                   nao-bloqueante, identifica os comandos LED1, LED2 e LED3
 *                   e expoe o ultimo comando valido para a aplicacao.
 ******************************************************************************
 */

#ifndef dSERIALCMD_H
#define dSERIALCMD_H


/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include "Bsp.h"

/*---------------------------------------------------------------------------*/
/* Defines Publicos                                                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Prototipos das Funcoes Publicas                                           */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Inicializa o buffer de recepcao e o estado do parser.
 * @note   Deve ser chamada apos Bsp_Init.
 * @retval Nenhum.
 */
void SerialCmd_Init(void);

/**
 * @brief  Processa os caracteres disponiveis no buffer de RX da USART3.
 *         Le caractere por caractere de forma nao-bloqueante, acumula
 *         ate encontrar um terminador de linha e entao interpreta
 *         o comando.
 * @note   Deve ser chamada a cada iteracao do laco principal.
 * @retval Nenhum.
 */
void SerialCmd_Process(void);

/**
 * @brief  Indica se um novo comando valido foi recebido desde a ultima
 *         leitura.
 * @note   A flag e consumida internamente apos a chamada de
 *         SerialCmd_GetCommand.
 * @retval true  Se ha um comando valido pendente.
 * @retval false Caso contrario.
 */
bool SerialCmd_IsCommandPending(void);

/**
 * @brief  Retorna o ultimo comando de LED valido recebido.
 * @note   So e valido quando SerialCmd_IsCommandPending retornou true.
 *         A chamada consome a flag de comando pendente.
 * @retval eLedChannel_t Canal do LED correspondente ao comando
 *                       (eLED_CHANNEL_1, _2 ou _3).
 */
eLedChannel_t SerialCmd_GetCommand(void);

#endif /* dSERIALCMD_H */
