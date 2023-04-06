/**
 * @file u_cli.c
 * @author Miguel Ugsimar(jmjugsimar@gmai.com)
 * @brief u(micro) command line interface source file
 *        for embedded system application
 * @version 0.1
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "tiny_cli.h"

/* ***************************************************************************/
/*    Private Defines                                                        */
/* ***************************************************************************/
/* ASCII delimiter character */
#define DEL_CHAR                 ( ( char )0x08 )
/* max number of args that can be registered */
#define MAX_NUM_ARGS             ( 5 )
/* check if character is a letter */
#define IS_LETTER( c )           ( ( ( c >= 'a' ) || ( c <= 'z' ) ) || \
                                   ( ( c >= 'A' ) || ( c <= 'Z' ) ) )
/* check if character is a number */
#define IS_NUMBER( c )             ( ( c >= '0' ) || ( c <= '9' ) )

/* ***************************************************************************/
/*    Private Type Defines                                                   */
/* ***************************************************************************/

/*!
   @brief CLI command handler instance.
 */
typedef struct
{
    char          buf[MAX_RECEIVE_BUFFER_CHAR];
    bool          busy;
    uint8_t       numFill;
    char          printBuf[MAX_PRINT_BUFFER_CHAR];
    uCli_command_t* pCmdTable;
    uint8_t       numCommands;
} uCli_instance_t;

/* ***************************************************************************/
/*    Private Variables                                                      */
/* ***************************************************************************/

/* singleton instance */
uCli_instance_t m_cli_instance;

/* ***************************************************************************/
/*    Private Function Definitions                                           */
/* ***************************************************************************/

/**
 * @brief echo characters
 *
 * @param[in] __char - char input
 */
bool print( char* msg, ... )
{
    bool overflow = false;
    va_list vList;

    va_start( vList, msg );
    overflow = ( MAX_PRINT_BUFFER_CHAR == vsnprintf(  m_cli_instance.printBuf, sizeof( m_cli_instance.printBuf ), msg, vList ) );
    va_end( vList );
    
    printf( "%s", m_cli_instance.printBuf );
    return overflow;
}

/**
 * @brief check if char is a letter
 *
 * @param[in] pInstance - pointer to cli instance
 * 
 */
void parse( uCli_instance_t* pInstance )
{
    char* cmd;
    char* args[MAX_NUM_ARGS];
    char* sep = " ";
    uint8_t numArgs = 0;

    // get tokens
    cmd = strtok( pInstance->buf, sep );
    if( NULL != cmd )
    {
        for( numArgs = 0; numArgs < MAX_NUM_ARGS; numArgs++ )
        {
            args[numArgs] = strtok( NULL, sep );
            if( args[numArgs] == NULL )
            {
                break;
            }
        }

        for( uint8_t i = 0; i < MAX_NUM_COMMANDS; i++ )
        {
            if( !strcmp( m_cli_instance.pCmdTable[i].cmd, cmd ) || 
                !strcmp( m_cli_instance.pCmdTable[i].alias, cmd ) && 
                ( NULL != m_cli_instance.pCmdTable[i].pCallback ) )
            {
                // found by command and alias
                pInstance->pCmdTable[i].pCallback( &pInstance->pCmdTable[i], args, numArgs );
                break;
            }
        }
    }
}

/**
 * @brief check if char is a letter
 *
 * @param[in] pInstance - pointer to cli instance
 * @param[in] __char - char input
 */
void input( uCli_instance_t* pInstance, char const __char )
{
    if( ( __char == '\b' ) || ( __char == ( ( char )0x7F ) ) )
    {
        if( pInstance->numFill > 0 )
        {
            // backspace was pressed, delete a char from buffer
            pInstance->buf[pInstance->numFill--] = '\0';
        }
        // echo backspace
        print( "%c", __char );
    }
    
    else if( __char == '\r' ) // enter was pressed
    {
        // process input
        parse( pInstance );
        // clear buffer, prepare next receive
        memset( pInstance->buf, '\0', MAX_RECEIVE_BUFFER_CHAR );
        pInstance->numFill = 0;
    }
    
    else // fill up the buffer
    {
        pInstance->buf[pInstance->numFill] = __char;
        if( pInstance->numFill <= MAX_RECEIVE_BUFFER_CHAR )
        {
            pInstance->numFill += 1;    
        }
        else
        {
            pInstance->numFill = 0;
        }
    }
}

/* ***************************************************************************/
/*    Private Function Definitions                                           */
/* ***************************************************************************/

bool uCli_isWord( const char* string )
{
    while( '\0' != *string )
    {
        if( !IS_LETTER( *( string++ ) ) )
        {
            return false;
        }
    }
    return true;
}

bool uCli_isNumeric( const char* string )
{
    while( '\0' != *string )
    {
        if( !IS_NUMBER( *( string++ ) ) )
        {
            return false;
        }
    }
    return true;
}

void uCli_inputProcess( const char* string )
{
    while( '\0' != *string )
    {
        input( &m_cli_instance, *( string++ ) );
    }
}

bool uCli_init( uCli_command_t* pCommandTable, uint8_t numCommands )
{
    if( ( NULL == pCommandTable ) || ( numCommands == 0 ) )
    {
        return false;
    }
    if( ( NULL != m_cli_instance->pCmdTable ) )
    {
        return false;
    }
    memset( &m_cli_instance, 0, sizeof( m_cli_instance ) );
    m_cli_instance.pCmdTable = pCommandTable;
    m_cli_instance.numCommands = numCommands;

    return true;
}
