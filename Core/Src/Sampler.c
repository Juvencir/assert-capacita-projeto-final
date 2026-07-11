/**
 ******************************************************************************
 *  @file            Sampler.c
 *  @addtogroup      Sampler
 *  @brief           Modulo de amostragem e filtragem do ADC.
 *  @author          Guilherme Juvencio
 *  @brief           Implementacao do modulo de amostragem.
 *                   Acumula amostras brutas do ADC, calcula a media
 *                   aritmetica ao completar a janela e converte para
 *                   escala percentual de 0 a 100. Nao acessa hardware.
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include "Sampler.h"

/*---------------------------------------------------------------------------*/
/* Defines Locais                                                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Tipos Locais                                                              */
/*---------------------------------------------------------------------------*/

/// @brief Estrutura que agrupa as variaveis de estado do modulo Sampler
typedef struct
{
    uint32_t sum;              ///< Somatorio das amostras da janela atual
    uint16_t sampleCount;      ///< Contador de amostras (0 a dSAMPLER_WINDOW_SIZE)
    bool     windowComplete;   ///< Flag de janela concluida
    uint8_t  lastPercent;      ///< Ultimo valor percentual calculado (0 a 100)
} Sampler_t;

/// @brief Instancia unica do estado do Sampler
static Sampler_t sampler;


/*---------------------------------------------------------------------------*/
/* Funcoes Locais                                                            */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Reinicia o acumulador e o contador para iniciar uma nova janela.
 * @retval Nenhum.
 */
static void Sampler_ResetWindow(void)
{
    sampler.sum         = 0U;
    sampler.sampleCount = 0U;
}


/*---------------------------------------------------------------------------*/
/* Funcoes Publicas                                                          */
/*---------------------------------------------------------------------------*/

void Sampler_Init(void)
{
    Sampler_ResetWindow();
    sampler.windowComplete = false;
    sampler.lastPercent    = 0U;
}

void Sampler_AddSample(uint16_t rawAdc)
{
    sampler.sum += rawAdc;
    sampler.sampleCount++;

    if (sampler.sampleCount >= dSAMPLER_WINDOW_SIZE)
    {
        /*
         * Como WINDOW_SIZE = 100:
         *   percent = sum / ADC_MAX
         */
        sampler.lastPercent = (uint8_t)(sampler.sum / dSAMPLER_ADC_MAX_VALUE);

        sampler.windowComplete = true;

        Sampler_ResetWindow();
    }
}

bool Sampler_IsWindowComplete(void)
{
    return sampler.windowComplete;
}

uint8_t Sampler_GetAveragePercent(void)
{
    sampler.windowComplete = false;

    return sampler.lastPercent;
}
