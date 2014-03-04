#include <stdint.h>
#include "stm32f10x.h"

/* 
   File:   UartDriver for STM32F100 series
   Author: Igor Belorus
   Date:   05/05/2012
*/

#define UART_DRIVER_RX_BUFF_SIZE 200

enum UartChannel
{
    UART_CHANNEL0,
    UART_CHANNEL1,
    UART_CHANNEL2,
    UART_CHANNEL3
};

enum UartWordLen
{
    USART_DATA8 = USART_WordLength_8b,
    USART_DATA9 = USART_WordLength_9b
};

enum UartStopBits
{
    USART_STOPBIT_1   = USART_StopBits_1,
    USART_STOPBIT_0_5 = USART_StopBits_0_5,
    USART_STOPBIT_2   = USART_StopBits_2,
    USART_STOPBIT_1_5 = USART_StopBits_1_5
};


enum UsartFlowControl
{
    USART_FLOW_CONTROL_NONE    = USART_HardwareFlowControl_None,
    USART_FLOW_CONTROL_RTS     = USART_HardwareFlowControl_RTS,
    USART_FLOW_CONTROL_CTS     = USART_HardwareFlowControl_CTS,
    USART_FLOW_CONTROL_RTS_CTS = USART_HardwareFlowControl_RTS_CTS,
};


enum UartParity
{
    USART_PARITY_NONE = USART_Parity_No,
    USART_PARITY_EVEN = USART_Parity_Even,
    USART_PARITY_ODD  = USART_Parity_Odd,
};

enum UartBaudRate
{ 
    UART_BAUD_RATE_2400   = 2400  ,
    UART_BAUD_RATE_4800   = 4800  ,
    UART_BAUD_RATE_9600   = 9600  ,
    UART_BAUD_RATE_19200  = 19200 ,
    UART_BAUD_RATE_38400  = 38400 ,
    UART_BAUD_RATE_115200 = 115200
};

typedef void ( *UartRxCallBack )( void* pContext, uint8_t* pData, uint32_t len );
typedef void ( *UartTxCompleateCallBack )( void* pContext );

class UartDriver
{
public:
    void SetWordLen( UartWordLen wordLen );
    void SetStopBits( UartStopBits stopBits );
    void SetParity( UartParity parity );
    void SetBaudRate( UartBaudRate baudrate ); 
    void SetRxCallBack( void* pContext, UartRxCallBack callBack );
    void SetTxCallBack( void* pContext, UartTxCompleateCallBack callBack );
    void Open( UartChannel channel );
    void Close();
    void Write( uint8_t* pData, uint16_t len );

    ~UartDriver();
    
    void Run();
private:    
    void GpioInit();
    void DmaIrqInit();
    void DmaInit();

private:
   static void DmaIrqHandler( void* pContext );
    
private:  
    void*        m_pRxContext;
    void*        m_pTxContext;
    UartWordLen  m_WordLen;
    UartStopBits m_StopBits;
    UartParity   m_Parity;
    UartBaudRate m_Baudrate;
    int          m_FileDescriptor;
    uint8_t      m_DataBuff[ UART_DRIVER_RX_BUFF_SIZE ];
    uint16_t     m_RxBuffSize;
    uint16_t     m_RxFirst;
    bool         m_IsTxBusy;
    bool         m_IsInitComplete;  

private:
    UartRxCallBack          fp_RxCallBack;
    UartTxCompleateCallBack fp_TxCallBack;
    
};

