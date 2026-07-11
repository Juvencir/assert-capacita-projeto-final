/**
 ******************************************************************************
 *  @file            Button.c
 *  @addtogroup      Button
 *  @brief           Modulo de gerencia do botao de usuario (USER_Btn).
 *  @author          Guilherme Juvencio
 *  @brief           Implementacao do modulo do botao.
 *                   Contem a ISR da EXTI15_10, o callback da HAL para
 *                   disparar o debounce e a maquina de estados toggle
 *                   processada no laco principal.
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include "Button.h"
#include "Bsp.h"

/*---------------------------------------------------------------------------*/
/* Defines Locais                                                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* Tipos Locais                                                              */
/*---------------------------------------------------------------------------*/

/// @brief Estrutura que agrupa as variaveis de estado do modulo Button
typedef struct
{
    bool frozen;           ///< Estado toggle de congelamento do sistema
    bool debounceActive;   ///< Indica se a janela de debounce esta ativa
} Button_t;

/// @brief Instancia unica do estado do Button
static Button_t button;


/*---------------------------------------------------------------------------*/
/* Funcoes Publicas                                                          */
/*---------------------------------------------------------------------------*/

void Button_Init(void)
{
    button.frozen = false;
    button.debounceActive = false;
}

bool Button_IsFrozen(void)
{
    return button.frozen;
}

void Button_Process(void)
{
    /* Verifica se houve interrupcao do botao */
    if (Bsp_Button_GetFlag())
    {
        /* Limpa a flag imediatamente para evitar reentradas */
        Bsp_Button_ClearFlag();

        /* Se nao houver um debounce em andamento, inicia */
        if (!button.debounceActive)
        {
            button.debounceActive = true;
            Bsp_Tim7_StartOnePulse();
        }
    }

    /* Aguarda a conclusao do debounce (30 ms) */
    if (button.debounceActive && Bsp_Tim7_IsDebounceFlag())
    {
        /* Limpa a flag do TIM7 */
        Bsp_Tim7_ClearDebounceFlag();

        /* Alterna o estado de congelamento (toggle) */
        button.frozen = !button.frozen;

        /* Finaliza a janela de debounce */
        button.debounceActive = false;
    }
}