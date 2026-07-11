/**
 ******************************************************************************
 *  @file            Bsp.c
 *  @addtogroup      Bsp
 *  @brief           Camada de abstracao de hardware para o projeto.
 *  @author          Guilherme Juvencio
 *  @brief           Interface publica da camada de abstracao de hardware.
 *                   Encapsula chamadas a HAL da ST para ADC, PWM, Timers e
 *                   USART3.
 ******************************************************************************
 */

#ifndef dBSP_H
#define dBSP_H


/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>

/*---------------------------------------------------------------------------*/
/* Defines Publicos                                                          */
/*---------------------------------------------------------------------------*/

/// @brief Timeout para conversao ADC em milissegundos
#define dBSP_ADC_TIMEOUT_MS       (10U)

/*---------------------------------------------------------------------------*/
/* Enumeracoes Publicas                                                      */
/*---------------------------------------------------------------------------*/

/// @brief Canais de LED disponiveis para controle PWM no TIM3
typedef enum
{
    eLED_CHANNEL_1 = 1,     ///< LED 1 - TIM3 CH1 (PA6)
    eLED_CHANNEL_2 = 2,     ///< LED 2 - TIM3 CH2 (PC7)
    eLED_CHANNEL_3 = 3      ///< LED 3 - TIM3 CH3 (PC8)
} eLedChannel_t;

/*---------------------------------------------------------------------------*/
/* Prototipos das Funcoes Publicas                                           */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Inicializa os servicos da camada BSP.
 *         Deve ser chamada apos a inicializacao dos perifericos (MX_*_Init).
 *         Inicia o TIM6 em modo de interrupcao para gerar flags periodicas
 *         de amostragem e inicia todos os canais PWM do TIM3.
 * @retval Nenhum.
 */
void Bsp_Init(void);

/**
 * @brief  Verifica se a flag de amostragem do TIM6 foi levantada.
 * @note   A flag e setada na ISR de overflow do TIM6 a cada 5 ms.
 * @retval true  Se a flag de amostragem estiver ativa.
 * @retval false Se a flag de amostragem estiver inativa.
 */
bool Bsp_Tim6_IsSampleFlag(void);

/**
 * @brief  Limpa a flag de amostragem do TIM6.
 * @note   Deve ser chamada apos o processamento da amostra no laco principal.
 * @retval Nenhum.
 */
void Bsp_Tim6_ClearSampleFlag(void);

/**
 * @brief  Dispara o TIM7 em modo One Pulse para iniciar a janela de
 *         debounce de 30 ms.
 * @note   Chamada a partir da ISR da EXTI13 quando o botao e pressionado.
 * @retval Nenhum.
 */
void Bsp_Tim7_StartOnePulse(void);

/**
 * @brief  Verifica se a flag de debounce do TIM7 foi levantada.
 * @note   A flag e setada na ISR de overflow do TIM7 apos 30 ms.
 * @retval true  Se a flag de debounce estiver ativa.
 * @retval false Se a flag de debounce estiver inativa.
 */
bool Bsp_Tim7_IsDebounceFlag(void);

/**
 * @brief  Limpa a flag de debounce do TIM7.
 * @note   Deve ser chamada apos o processamento do evento de debounce.
 * @retval Nenhum.
 */
void Bsp_Tim7_ClearDebounceFlag(void);

/**
 * @brief  Verifica se a flag de interrupcao do botao foi levantada.
 * @retval true  Se o botao foi pressionado.
 * @retval false Se o botao nao foi pressionado.
 */
bool Bsp_Button_GetFlag(void);

/**
 * @brief  Limpa a flag de interrupcao do botao.
 * @retval Nenhum.
 */
void Bsp_Button_ClearFlag(void);

/**
 * @brief  Dispara a conversao ADC por software, aguarda a conclusao com
 *         timeout e retorna o valor bruto de 12 bits.
 * @note   Utiliza polling com timeout seguro definido em dBSP_ADC_TIMEOUT_MS.
 * @retval uint16_t Valor bruto do ADC (0 a 4095). Retorna 0 em caso de
 *                  timeout ou erro de conversao.
 */
uint16_t Bsp_Adc_Read(void);

/**
 * @brief  Inicia todos os canais PWM do TIM3 (CH1, CH2 e CH3).
 * @note   Os canais sao iniciados com duty cycle zerado.
 * @retval Nenhum.
 */
void Bsp_Pwm_StartAll(void);

/**
 * @brief  Atualiza o duty cycle de um canal PWM especifico.
 * @param  channel Canal do LED a ser atualizado (eLED_CHANNEL_1, _2 ou _3).
 * @param  percent Valor percentual do duty cycle (0 a 100 por cento).
 * @note   Valores acima de 100 sao saturados.
 * @retval Nenhum.
 */
void Bsp_Pwm_SetDutyCycle(eLedChannel_t channel, uint8_t percent);

/**
 * @brief  Transmite um unico caractere via USART3 de forma bloqueante.
 * @param  ch Caractere a ser transmitido.
 * @note   Funcao de suporte para redirecionamento futuro do printf.
 * @retval Nenhum.
 */
void Bsp_Uart_TransmitChar(uint8_t ch);

/**
 * @brief  Verifica de forma nao-bloqueante se ha um caractere disponivel
 *         no buffer de recepcao da USART3.
 * @retval true  Se houver pelo menos um caractere disponivel para leitura.
 * @retval false Se o buffer de recepcao estiver vazio.
 */
bool Bsp_Uart_IsRxReady(void);

/**
 * @brief  Le um unico caractere do buffer de recepcao da USART3.
 * @note   Deve ser chamada apenas quando Bsp_Uart_IsRxReady retornar true.
 * @retval uint8_t Caractere lido.
 */
uint8_t Bsp_Uart_ReadChar(void);

#endif /* dBSP_H */