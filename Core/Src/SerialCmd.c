/**
 ******************************************************************************
 *  @file            SerialCmd.c
 *  @addtogroup      SerialCmd
 *  @brief           Modulo de recepcao e interpretacao de comandos seriais.
 *  @author          Guilherme Juvencio
 *  @brief           Implementacao do parser de comandos UART.
 *                   Acumula caracteres recebidos via USART3 de forma
 *                   nao-bloqueante, identifica as strings LED1, LED2 e LED3
 *                   e disponibiliza o ultimo comando valido para a aplicacao.
 *                   Nao acessa hardware diretamente (delega ao BSP).
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* Includes                                                                  */
/*---------------------------------------------------------------------------*/

#include "SerialCmd.h"

/*---------------------------------------------------------------------------*/
/* Defines Locais                                                            */
/*---------------------------------------------------------------------------*/

/// @brief Tamanho do buffer de linha para acumulo de caracteres
#define dSERIALCMD_BUFFER_SIZE      (16U)


/*---------------------------------------------------------------------------*/
/* Tipos Locais                                                              */
/*---------------------------------------------------------------------------*/

/// @brief Estrutura que agrupa as variaveis de estado do modulo SerialCmd
typedef struct
{
    char          rxBuffer[dSERIALCMD_BUFFER_SIZE]; ///< Buffer de acumulo de linha
    uint8_t       rxIndex;                          ///< Indice atual no buffer
    bool          commandPending;                   ///< Flag de comando valido recebido
    eLedChannel_t lastCommand;                      ///< Ultimo comando de LED valido
} SerialCmd_t;

/// @brief Instancia unica do estado do SerialCmd
static SerialCmd_t serialCmd;


/*---------------------------------------------------------------------------*/
/* Funcoes Locais                                                            */
/*---------------------------------------------------------------------------*/

/**
 * @brief  Reinicia o buffer de recepcao para uma nova linha.
 * @retval Nenhum.
 */
static void SerialCmd_ResetBuffer(void)
{
    serialCmd.rxIndex  = 0U;
    serialCmd.rxBuffer[0] = '\0';
}

/**
 * @brief  Interpreta a string acumulada no buffer e, se for um comando
 *         valido (LED1, LED2 ou LED3), armazena o resultado.
 * @note   A comparacao e exata e case-sensitive, conforme especificacao.
 * @retval Nenhum.
 */
static void SerialCmd_ParseBuffer(void)
{
    /*
     * Comparacao exata contra os tres comandos validos.
     * A string no buffer esta terminada em '\0'.
     */
    if (serialCmd.rxBuffer[0] == 'L' &&
        serialCmd.rxBuffer[1] == 'E' &&
        serialCmd.rxBuffer[2] == 'D')
    {
        if (serialCmd.rxBuffer[3] == '1' && serialCmd.rxBuffer[4] == '\0')
        {
            serialCmd.lastCommand    = eLED_CHANNEL_1;
            serialCmd.commandPending = true;
        }
        else if (serialCmd.rxBuffer[3] == '2' && serialCmd.rxBuffer[4] == '\0')
        {
            serialCmd.lastCommand    = eLED_CHANNEL_2;
            serialCmd.commandPending = true;
        }
        else if (serialCmd.rxBuffer[3] == '3' && serialCmd.rxBuffer[4] == '\0')
        {
            serialCmd.lastCommand    = eLED_CHANNEL_3;
            serialCmd.commandPending = true;
        }
    }
}


/*---------------------------------------------------------------------------*/
/* Funcoes Publicas                                                          */
/*---------------------------------------------------------------------------*/

void SerialCmd_Init(void)
{
    SerialCmd_ResetBuffer();
    serialCmd.commandPending = false;
    serialCmd.lastCommand    = eLED_CHANNEL_1;
}

void SerialCmd_Process(void)
{
    uint8_t ch;

    /* Le todos os caracteres disponiveis no buffer de RX */
    while (Bsp_Uart_IsRxReady())
    {
        ch = Bsp_Uart_ReadChar();

        /* Terminador de linha: processa o comando acumulado */
        if ((ch == '\r') || (ch == '\n'))
        {
            /* So processa se houver algo no buffer */
            if (serialCmd.rxIndex > 0U)
            {
                serialCmd.rxBuffer[serialCmd.rxIndex] = '\0';
                SerialCmd_ParseBuffer();
                SerialCmd_ResetBuffer();
            }
        }
        /* Caracteres imprimiveis: acumula no buffer */
        else if ((ch >= 0x20U) && (ch <= 0x7EU))
        {
            if (serialCmd.rxIndex < (dSERIALCMD_BUFFER_SIZE - 1U))
            {
                serialCmd.rxBuffer[serialCmd.rxIndex] = (char)ch;
                serialCmd.rxIndex++;
            }
            else
            {
                /* Estouro do buffer: descarta a linha e reinicia */
                SerialCmd_ResetBuffer();
            }
        }
        /* Outros caracteres (backspace, escape, etc.) sao ignorados */
    }
}

bool SerialCmd_IsCommandPending(void)
{
    return serialCmd.commandPending;
}

eLedChannel_t SerialCmd_GetCommand(void)
{
    /* A leitura do comando consome a flag */
    serialCmd.commandPending = false;

    return serialCmd.lastCommand;
}
