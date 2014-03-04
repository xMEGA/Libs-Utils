#include "InterruptManager.h"

extern "C" void __iar_program_start( void );
extern "C" void __main( void );

void* InterruptManager_t::m_pContextTable[ INTERRUPT_MANAGER_VECTORS_AMOUNT ] = 
{ 
  0
};

InterruptRoutine_t InterruptManager_t::fp_IntRoutineTable[ INTERRUPT_MANAGER_VECTORS_AMOUNT ] = 
{ 
    DefaultInterruptRoutine 
};

InterruptManager_t::InterruptManager_t()
{
    Init();  
}

void InterruptManager_t::Init()
{
    for( uint8_t idx = 0; idx < INTERRUPT_MANAGER_VECTORS_AMOUNT; idx++ )
    {
        m_pContextTable   [ idx ] = 0;
        fp_IntRoutineTable[ idx ] = DefaultInterruptRoutine;
    }
}

void InterruptManager_t::RegisterInterrupt( void* pContext, uint16_t vector, InterruptRoutine_t fp_Routine )
{
    if( vector < INTERRUPT_MANAGER_VECTORS_AMOUNT )
    {
        m_pContextTable   [ vector ] = pContext;
        fp_IntRoutineTable[ vector ] = fp_Routine;
    }
}
 
void InterruptManager_t::DefaultInterruptRoutine( void* pContext )
{
    for( ;; ) // Trap
    {
        asm("nop");
    }
}

// --------------------------- Interrupt Handlers --------------------------- //
void InterruptManager_t::DefaultHandler()
{
    DefaultInterruptRoutine( 0 );
}

void InterruptManager_t::SysTickHandler()
{
    fp_IntRoutineTable[ INTERRUPT_SYSTICK_VECTOR ]( m_pContextTable[ INTERRUPT_SYSTICK_VECTOR ] );  
}

void InterruptManager_t::DMA1Channel4IrqHandler()
{
    fp_IntRoutineTable[ INTERRUPT_DMA1_Channel4_IRQ_VECTOR ]( m_pContextTable[ INTERRUPT_DMA1_Channel4_IRQ_VECTOR ] );  
}

void InterruptManager_t::ResetHandler()
{
    __iar_program_start();
}

void InterruptManager_t::HardFaultHandler()
{
    DefaultInterruptRoutine( 0 );
}


//--------------------------------------------------------------------------- //


// --------------------------- Interrupt Handlers Table ----------------------//

#pragma language = extended   
#pragma segment  = "CSTACK"
#pragma location = ".intvec"
__root const InterruptManager_t::HandlerItem_t InterruptManager_t::m_InterruptVectorTable[] = 
{
    { .pHandler = __sfe( "CSTACK" ) },
    ResetHandler          , //Reset_Handler
    DefaultHandler        ,//NMI_Handler
    HardFaultHandler      ,//HardFault_Handler
    DefaultHandler        ,//MemManage_Handler         ; MPU Fault Handler
    DefaultHandler        ,//BusFault_Handler
    DefaultHandler        ,//UsageFault_Handler
    DefaultHandler        ,//0                         ; Reserved
    DefaultHandler        ,//0                         ; Reserved
    DefaultHandler        ,//0                         ; Reserved
    DefaultHandler        ,//0                         ; Reserved
    DefaultHandler        ,//SVCall_Handler
    DefaultHandler        ,//DebugMon_Handler
    DefaultHandler        ,//0                         ; Reserved
    DefaultHandler        ,//PendSV_Handler
    SysTickHandler        ,//SysTick_Handler

    //         ; External Interrupts
    DefaultHandler        ,//WWDG_IRQHandler           ; Window Watchdog
    DefaultHandler        ,//PVD_IRQHandler            ; PVD through EXTI Line detect
    DefaultHandler        ,//TAMPER_IRQHandler
    DefaultHandler        ,//RTC_IRQHandler
    DefaultHandler        ,//FLASH_IRQHandler
    DefaultHandler        ,//RCC_IRQHandler
    DefaultHandler        ,//EXTI0_IRQHandler
    DefaultHandler        ,//EXTI1_IRQHandler
    DefaultHandler        ,//EXTI2_IRQHandler
    DefaultHandler        ,//EXTI3_IRQHandler
    DefaultHandler        ,//EXTI4_IRQHandler
    DefaultHandler        ,//DMA1_Channel1_IRQHandler
    DefaultHandler        ,//DMA1_Channel2_IRQHandler
    DefaultHandler        ,//DMA1_Channel3_IRQHandler
    DMA1Channel4IrqHandler,//DMA1_Channel4_IRQHandler
    DefaultHandler        ,//DMA1_Channel5_IRQHandler
    DefaultHandler        ,//DMA1_Channel6_IRQHandler
    DefaultHandler        ,//DMA1_Channel7_IRQHandler
    DefaultHandler        ,//ADC1_2_IRQHandler
    DefaultHandler        ,//USB_High_Priority_CAN1_TX_IRQHandler
    DefaultHandler        ,//USB_LowPriority CAN1_RX0_IRQHandler
    DefaultHandler        ,//CAN1_RX1_IRQHandler
    DefaultHandler        ,//CAN1_SCE_IRQHandler
    DefaultHandler        ,//EXTI9_5_IRQHandler
    DefaultHandler        ,//TIM1_BREAK_IRQHandler
    DefaultHandler        ,//TIM1_Update_IRQHandler
    DefaultHandler        ,//TIM1_TRigger_Commutation_IRQHandler
    DefaultHandler        ,//TIM1_CaptureCompare_IRQHandler
    DefaultHandler        ,//TIM2_IRQHandler
    DefaultHandler        ,//TIM3_IRQHandler
    DefaultHandler        ,//0                   
    DefaultHandler        ,//I2C1_Event_IRQHandler
    DefaultHandler        ,//I2C1_Error_IRQHandler
    DefaultHandler        ,//0
    DefaultHandler        ,//0
    DefaultHandler        ,//SPI1_IRQHandler
    DefaultHandler        ,//0
    DefaultHandler        ,//USART1_IRQHandler
    DefaultHandler        ,//USART2_IRQHandler
    DefaultHandler        ,//0
    DefaultHandler        ,//EXTI15_10_IRQHandler
    DefaultHandler        ,//RTCAlarm_IRQHandler
    DefaultHandler        ,//USBWakeUp_IRQHandler
};

//----------------------------------------------------------------------------//