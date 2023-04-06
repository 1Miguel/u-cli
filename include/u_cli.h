/**
 * @file u_cli.h
 * @author Miguel Ugsimar(jmjugsimar@gmai.com)
 * @brief u(micro) command line interface header file
 *        for embedded system application
 * @version 0.1
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __U_CLI_H
#define __U_CLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** @addtogroup u_cli
  * @{
  */

/* ***************************************************************************/
/*    Public Macro Defines                                                   */
/* ***************************************************************************/
/* max number of commands that can be registered */
#define MAX_NUM_COMMANDS         ( 10 )
/* max number of characters that can be received */
#define MAX_RECEIVE_BUFFER_CHAR  ( 100 )
/* max number of characters that can be printed  */
#define MAX_PRINT_BUFFER_CHAR    ( 100 )

/* ***************************************************************************/
/*    Public Type Defines                                                    */
/* ***************************************************************************/
/*!
   @brief CLI command structure definition.
 */
typedef struct uCli_command_s
{
    const char* cmd;     /*!< command name                               */
    const char* alias;   /*!< command alias name                         */
    const char* desc;    /*!< command help description                   */
    char*   args;    /*!< pointer to command command arguments       */
    void ( *pCallback )( struct uCli_command_s* command, char** pArgs, uint8_t numArgs );  /*!< pointer to callback handler function */
} uCli_command_t;

/* ***************************************************************************/
/*    Public Function Declarations                                           */
/* ***************************************************************************/

/**
 * @brief check if char is a letter
 *
 * @param[in] pCommandTable - pointer to command table instance
 * @param[in] numCommands - number of commands in table
 */
void uCli_init( uCli_command_t* pCommandTable, uint8_t numCommands );

/**
 * @brief process string input
 *
 * @param[in] string - string input
 */
void uCli_inputProcess( const char* string );


/**
 * @brief check if char word is a word
 *
 * @param[in] __char - char input
 * @return true - char is a letter
 * @return false - not a letter
 */
bool uCli_isWord( const char* string );

/**
 * @brief check if char word is a number
 *
 * @param[in] __char - char input
 * @return true - char is a number
 * @return false - not a number
 */
bool uCli_isNumeric( const char* string );

#ifdef __cplusplus
}
#endif

#endif // __U_CLI_H

/**
  * @}
  */