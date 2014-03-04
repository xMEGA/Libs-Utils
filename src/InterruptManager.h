#ifndef INTERRUPT_MANAGER_H
#define INTERRUPT_MANAGER_H

#include <stdint.h>

typedef void ( *InterruptRoutine_t )( void* pContext );

#define INTERRUPT_MANAGER_VECTORS_AMOUNT 58

enum
{
    INTERRUPT_RESET_VECTOR          = 0 ,// Reset Handler
    INTERRUPT_NMI_VECTOR                ,
    INTERRUPT_HARD_FAULT_VECTOR         ,
    INTERRUPT_MPU_FAULT_VECTOR          ,
    INTERRUPT_BUS_FAULT_VECTOR          ,
    INTERRUPT_USAGE_FAULT_VECTOR        ,
    INTERRUPT_RESERVED_VECTOR0          ,
    INTERRUPT_RESERVED_VECTOR1          ,
    INTERRUPT_RESERVED_VECTOR2          ,
    INTERRUPT_RESERVED_VECTOR3          ,
    INTERRUPT_SVC_VECTOR                ,// SVCall Handler
    INTERRUPT_DEBUG_MON_VECTOR          ,// Debug Monitor Handler
    INTERRUPT_RESERVED_VECTOR4                         ,// Reserved
    INTERRUPT_PEND_SV_VECTOR            ,// PendSV Handler
    INTERRUPT_SYSTICK_VECTOR            ,// SysTick Handler

	    // External Interrupts
    INTERRUPT_WWDG_IRQ_VECTOR           ,// Window Watchdog
    INTERRUPT_PVD_IRQ_VECTOR            ,// PVD through EXTI Line detect
    INTERRUPT_TAMPER_IRQ_VECTOR         ,// Tamper
    INTERRUPT_RTC_IRQ_VECTOR            ,// RTC
    INTERRUPT_FLASH_IRQ_VECTOR          ,// Flash
    INTERRUPT_RCC_IRQ_VECTOR            ,// RCC
    INTERRUPT_EXTI0_IRQ_VECTOR          ,// EXTI Line 0
    INTERRUPT_EXTI1_IRQ_VECTOR          ,// EXTI Line 1
    INTERRUPT_EXTI2_IRQ_VECTOR          ,// EXTI Line 2
    INTERRUPT_EXTI3_IRQ_VECTOR          ,// EXTI Line 3
    INTERRUPT_EXTI4_IRQ_VECTOR          ,// EXTI Line 4
    INTERRUPT_DMA1_Channel1_IRQ_VECTOR  ,// DMA1 Channel 1
    INTERRUPT_DMA1_Channel2_IRQ_VECTOR  ,// DMA1 Channel 2
    INTERRUPT_DMA1_Channel3_IRQ_VECTOR  ,// DMA1 Channel 3
    INTERRUPT_DMA1_Channel4_IRQ_VECTOR  ,// DMA1 Channel 4
    INTERRUPT_DMA1_Channel5_IRQ_VECTOR  ,// DMA1 Channel 5
    INTERRUPT_DMA1_Channel6_IRQ_VECTOR  ,// DMA1 Channel 6
    INTERRUPT_DMA1_Channel7_IRQ_VECTOR  ,// DMA1 Channel 7
    INTERRUPT_ADC1_2_IRQ_VECTOR         ,// ADC1 & ADC2
    INTERRUPT_USB_HP_CAN1_TX_IRQ_VECTOR ,// USB High Priority or CAN1 TX
    INTERRUPT_USB_LP_CAN1_RX0_IRQ_VECTOR,// USB Low  Priority or CAN1 RX0
    INTERRUPT_CAN1_RX1_IRQ_VECTOR       ,// CAN1 RX1
    INTERRUPT_CAN1_SCE_IRQ_VECTOR       ,// CAN1 SCE
    INTERRUPT_EXTI9_5_IRQ_VECTOR        ,// EXTI Line 9..5
    INTERRUPT_TIM1_BRK_IRQ_VECTOR       ,// TIM1 Break
    INTERRUPT_TIM1_UP_IRQ_VECTOR        ,// TIM1 Update
    INTERRUPT_TIM1_TRG_COM_IRQ_VECTOR   ,// TIM1 Trigger and Commutation
    INTERRUPT_TIM1_CC_IRQ_VECTOR        ,// TIM1 Capture Compare
    INTERRUPT_TIM2_IRQ_VECTOR           ,// TIM2
    INTERRUPT_TIM3_IRQ_VECTOR           ,// TIM3
    INTERRUPT_RESERVED_VECTOR5          ,// Reserved
    INTERRUPT_I2C1_EV_IRQ_VECTOR        ,// I2C1 Event
    INTERRUPT_I2C1_ER_IRQ_VECTOR        ,// I2C1 Error
    INTERRUPT_RESERVED_VECTOR6          ,// Reserved
    INTERRUPT_RESERVED_VECTOR7          ,// Reserved
    INTERRUPT_SPI1_IRQ_VECTOR           ,// SPI1
    INTERRUPT_RESERVED_VECTOR8          ,// Reserved
    INTERRUPT_USART1_IRQ_VECTOR         ,// USART1
    INTERRUPT_USART2_IRQ_VECTOR         ,// USART2
    INTERRUPT_RESERVED_VECTOR9          ,// Reserved
    INTERRUPT_EXTI15_10_IRQ_VECTOR      ,// EXTI Line 15..10
    INTERRUPT_RTC_ALARM_IRQ_VECTOR      ,// RTC Alarm through EXTI Line
    INTERRUPT_USB_WAKEUP_IRQ_VECTOR     ,// USB Wakeup from suspend
};

class InterruptManager_t
{
public:
    static void Init();    
    static void RegisterInterrupt( void* pContext, uint16_t vector, InterruptRoutine_t fp_Routine );
    InterruptManager_t();
private:
    
    union HandlerItem_t
    {
        void ( *fp_Handler )( void );
        void *pHandler;
    };

    static const HandlerItem_t m_InterruptVectorTable[];
    static void*               m_pContextTable[];
    static InterruptRoutine_t  fp_IntRoutineTable[];
    
    
private:
    static void DefaultInterruptRoutine( void* pContext );   

    static void ResetHandler();   
    static void SysTickHandler(); 
    static void DefaultHandler();
    static void HardFaultHandler();
    static void DMA1Channel4IrqHandler();

};




#endif // INTERRUPT_MANAGER_H