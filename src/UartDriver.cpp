#include "UartDriver.h"
#include "stm32f10x_dma.h" 
#include "InterruptManager.h"

/* 
   File:   UartDriver for STM32F100 series
   Author: Igor Belorus
   Date:   05/05/2012
*/

void UartDriver::GpioInit()
{
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, DISABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
    GPIO_PinRemapConfig( GPIO_Remap_USART1, DISABLE );

    GPIO_InitTypeDef GPIO;  
    /*Tx on PA9 as alternate function push-pull */
    GPIO.GPIO_Pin  = GPIO_Pin_9;
    GPIO.GPIO_Speed= GPIO_Speed_50MHz;
    GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO);
    /*Rx on PA10 as input floating*/
    GPIO.GPIO_Pin  = GPIO_Pin_10;
    GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO);
}

void UartDriver::DmaIrqInit()
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_InitStructure.NVIC_IRQChannel                    = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void UartDriver::DmaInit()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  
    DMA_DeInit(DMA1_Channel5);
    //while (DMA_GetFlagStatus(DMA1_Stream1) != DISABLE){};

    DMA_InitTypeDef  DMA_InitStructure;

    DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t )&USART1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr     = ( uint32_t )m_DataBuff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize         = m_RxBuffSize;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;

    DMA_Init( DMA1_Channel5, &DMA_InitStructure );
    DMA_Cmd( DMA1_Channel5, ENABLE );
    USART_DMACmd( USART1, USART_DMAReq_Rx, ENABLE );
    
    DmaIrqInit();
}

void UartDriver::DmaIrqHandler( void* pContext )
{
    UartDriver* pUart = static_cast< UartDriver* >( pContext );
    
    DMA_ClearITPendingBit( DMA1_IT_TC4 );  
    DMA_ITConfig( DMA1_Channel4, DMA_IT_TC, DISABLE ); 
    
    while( RESET == USART_GetFlagStatus( USART1, USART_FLAG_TC ) )
    {
    };
    
    pUart->fp_TxCallBack( pContext );
    pUart->m_IsTxBusy = false;
}

void UartDriver::SetWordLen( UartWordLen wordLen )
{
    m_WordLen = wordLen;
}

void UartDriver::SetStopBits( UartStopBits stopBits )
{
    m_StopBits = stopBits;
}

void UartDriver::SetParity( UartParity parity )
{
    m_Parity = parity;
}

void UartDriver::SetBaudRate( UartBaudRate baudrate )
{
    m_Baudrate = baudrate;
}

void UartDriver::SetRxCallBack( void* pContext, UartRxCallBack callBack )
{
    m_pRxContext  = pContext;
    fp_RxCallBack = callBack;
}

void UartDriver::SetTxCallBack( void* pContext, UartTxCompleateCallBack callBack )
{
    m_pTxContext  = pContext;
    fp_TxCallBack = callBack;
}

void UartDriver::Open( UartChannel channel )
{
    //m_Chanell = channel;
   
    m_RxBuffSize = sizeof( m_DataBuff );
    m_RxFirst    = 0;

    USART_DeInit( USART1 );

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
  
    InterruptManager_t::RegisterInterrupt( this, INTERRUPT_DMA1_Channel4_IRQ_VECTOR, DmaIrqHandler );
    GpioInit();
    DmaInit();
   
    USART_InitTypeDef uartInitStruct;
    
    uartInitStruct.USART_BaudRate   = m_Baudrate;
    uartInitStruct.USART_Parity     = m_Parity;
    uartInitStruct.USART_StopBits   = m_StopBits;
    uartInitStruct.USART_WordLength = m_WordLen;
    uartInitStruct.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
    uartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    
    USART_Init( USART1, &uartInitStruct );
    USART_ClearFlag( USART1, USART_FLAG_CTS | USART_FLAG_LBD | USART_FLAG_TC | USART_FLAG_RXNE );
    USART_ITConfig( USART1, USART_IT_TXE,  DISABLE );
    USART_ITConfig( USART1, USART_IT_RXNE, DISABLE );
  
    USART_Cmd( USART1, ENABLE );

    m_IsInitComplete = true;
    m_IsTxBusy = false;
}

void UartDriver::Run()
{
    if( true == m_IsInitComplete )//Если 3 UART проинициализирован
    {
        uint16_t size = DMA_GetCurrDataCounter( DMA1_Channel5 );//При первом запуске возвращает размер буфера 
      
        uint16_t rxLast = m_RxBuffSize - size;
        
        if( rxLast == m_RxBuffSize )
        {
            rxLast = 0;
        }
        
        uint16_t rxFirst = m_RxFirst;

        if( rxFirst != rxLast )//Анализ входного потока байт
        {
            uint16_t rxLen = 0;   
          
            if( rxLast > rxFirst )//Нормальный режим
            {
                m_RxFirst = rxLast;
                rxLen = rxLast - rxFirst;
            }
            else
            {
                m_RxFirst = 0;
                rxLen = m_RxBuffSize - rxFirst;
            }
            
            fp_RxCallBack( m_pRxContext, &m_DataBuff[ rxFirst ], rxLen );
        }
    }
}

void UartDriver::Write( uint8_t* pData, uint16_t len )
{
    DMA_InitTypeDef  DMA_InitStructure;  

    if( false == m_IsTxBusy )
    {
	m_IsTxBusy = true;

	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );  
	DMA_DeInit( DMA1_Channel4 );

	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t )&USART1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr     = ( uint32_t )pData;
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize         = len;
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;//DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;

	//while (DMA_GetFlagStatus(DMA1_Channel2) != DISABLE){};

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
    }
}

UartDriver::~UartDriver()
{

}

void UartDriver::Close()
{
    USART_DeInit( USART1 );
}

