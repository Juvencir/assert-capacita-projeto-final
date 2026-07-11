/**
 ******************************************************************************
 *  @file            Sampler.h
 *  @addtogroup      Sampler
 *  @brief           Modulo de amostragem e filtragem do ADC.
 *  @author          Guilherme Juvencio
 *  @brief           Interface publica do modulo de amostragem.
 *                   Acumula ate 100 amostras do ADC, calcula a media
 *                   aritmetica e converte para escala percentual de 0 a 100.
 *                   Nao acessa hardware diretamente.
 ******************************************************************************
 */

#ifndef dSAMPLER_H
#define dSAMPLER_H


/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>

/*---------------------------------------------------------------------------*/
/* Defines Publicos                                                          */
/*---------------------------------------------------------------------------*/

/// @brief Quantidade de amostras por janela de calculo (500 ms a 5 ms cada)
#define dSAMPLER_WINDOW_SIZE        (100U)

/// @brief Valor maximo do ADC 12 bits para conversao percentual
#define dSAMPLER_ADC_MAX_VALUE      (4095U)


/*---------------------------------------------------------------------------*/
/* Prototipos das Funcoes Publicas                                           */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Inicializa (zera) o acumulador e o contador de amostras.
 * @note   Deve ser chamada antes do primeiro uso ou para resetar a janela.
 * @retval Nenhum.
 */
void Sampler_Init(void);

/**
 * @brief  Adiciona uma nova amostra bruta do ADC ao acumulador.
 *         Quando o numero de amostras atinge dSAMPLER_WINDOW_SIZE,
 *         calcula automaticamente a media e a porcentagem correspondente.
 * @param  rawAdc Valor bruto do ADC (0 a 4095).
 * @note   Nao bloqueia. O resultado fica disponivel via Sampler_GetAveragePercent.
 * @retval Nenhum.
 */
void Sampler_AddSample(uint16_t rawAdc);

/**
 * @brief  Indica se a janela de amostras foi completada.
 * @note   A flag e limpa internamente apos a leitura do resultado.
 * @retval true  Se 100 amostras foram acumuladas e o resultado esta pronto.
 * @retval false Caso contrario.
 */
bool Sampler_IsWindowComplete(void);

/**
 * @brief  Retorna o valor percentual medio da janela de amostras mais recente.
 * @note   So e valido quando Sampler_IsWindowComplete retornou true.
 * @retval uint8_t Valor de 0 a 100 representando a media percentual.
 */
uint8_t Sampler_GetAveragePercent(void);

#endif /* dSAMPLER_H */
