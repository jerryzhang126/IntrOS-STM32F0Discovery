/*******************************************************************************
@file     startup.c
@author   Rajmund Szymanski
@date     21.05.2016
@brief    STM32F0xx startup file.
          After reset the Cortex-M4 processor is in thread mode,
          priority is privileged, and the stack is set to main.
*******************************************************************************/

#include <stm32f0xx.h>

/*******************************************************************************
 Specific definitions for the chip
*******************************************************************************/

#define __ram_start 0x20000000
#define __ram_end   0x20002000

/*******************************************************************************
 Configuration of stacks
*******************************************************************************/

#ifndef main_stack_size
#define main_stack_size 1024 // <- default size of main stack
#endif

#ifndef proc_stack_size
#define proc_stack_size    0 // <- default size of process stack
#endif

#define main_stack (((main_stack_size)+7)&(~7))
#define proc_stack (((proc_stack_size)+7)&(~7))

/*******************************************************************************
 Initial stacks' pointers
*******************************************************************************/

extern char __initial_msp[];
extern char __initial_sp [];

/*******************************************************************************
 Default fault handler
*******************************************************************************/

static __attribute__ ((used, noreturn)) void Fault_Handler( void )
{
	/* Go into an infinite loop */
	for (;;);
}

/*******************************************************************************
 Default exit handlers
*******************************************************************************/

#if   defined(__ARMCC_VERSION) && defined(__MICROLIB)
void _microlib_exit( void ) __attribute__ ((weak, noreturn, alias("Fault_Handler")));
#elif defined(__ARMCC_VERSION)
void      _sys_exit( void ) __attribute__ ((weak, noreturn, alias("Fault_Handler")));
#elif defined(__GNUC__)
void          _exit( int  ) __attribute__ ((weak, noreturn, alias("Fault_Handler")));
#endif

/*******************************************************************************
 Declaration of exception handlers
*******************************************************************************/

/* Core exceptions */
void Reset_Handler                (void) __attribute__ ((weak, noreturn));
void NMI_Handler                  (void) __attribute__ ((weak, alias("Fault_Handler")));
void HardFault_Handler            (void) __attribute__ ((weak, alias("Fault_Handler")));
void MemManage_Handler            (void) __attribute__ ((weak, alias("Fault_Handler")));
void BusFault_Handler             (void) __attribute__ ((weak, alias("Fault_Handler")));
void UsageFault_Handler           (void) __attribute__ ((weak, alias("Fault_Handler")));
void SVC_Handler                  (void) __attribute__ ((weak, alias("Fault_Handler")));
void DebugMon_Handler             (void) __attribute__ ((weak, alias("Fault_Handler")));
void PendSV_Handler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void SysTick_Handler              (void) __attribute__ ((weak, alias("Fault_Handler")));

/* External interrupts */
void WWDG_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void PVD_IRQHandler                (void) __attribute__ ((weak, alias("Fault_Handler")));
void RTC_IRQHandler                (void) __attribute__ ((weak, alias("Fault_Handler")));
void FLASH_IRQHandler              (void) __attribute__ ((weak, alias("Fault_Handler")));
void RCC_IRQHandler                (void) __attribute__ ((weak, alias("Fault_Handler")));
void EXTI0_1_IRQHandler            (void) __attribute__ ((weak, alias("Fault_Handler")));
void EXTI2_3_IRQHandler            (void) __attribute__ ((weak, alias("Fault_Handler")));
void EXTI4_5_IRQHandler            (void) __attribute__ ((weak, alias("Fault_Handler")));
void TSC_IRQHandler                (void) __attribute__ ((weak, alias("Fault_Handler")));
void DMA1_Channel1_IRQHandler      (void) __attribute__ ((weak, alias("Fault_Handler")));
void DMA1_Channel2_3_IRQHandler    (void) __attribute__ ((weak, alias("Fault_Handler")));
void DMA1_Channel4_5_IRQHandler    (void) __attribute__ ((weak, alias("Fault_Handler")));
void ADC1_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM1_BRK_UP_TRG_COM_IRQHandler(void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM1_CC_IRQHandler            (void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM2_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM3_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM6_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM7_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM14_IRQHandler              (void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM15_IRQHandler              (void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM16_IRQHandler              (void) __attribute__ ((weak, alias("Fault_Handler")));
void TIM17_IRQHandler              (void) __attribute__ ((weak, alias("Fault_Handler")));
void I2C1_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void I2C2_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void SPI1_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void SPI2_IRQHandler               (void) __attribute__ ((weak, alias("Fault_Handler")));
void USART1_IRQHandler             (void) __attribute__ ((weak, alias("Fault_Handler")));
void USART2_IRQHandler             (void) __attribute__ ((weak, alias("Fault_Handler")));
void USART3_6_IRQHandler           (void) __attribute__ ((weak, alias("Fault_Handler")));
void CEC_CAN_IRQHandler            (void) __attribute__ ((weak, alias("Fault_Handler")));
void USB_IRQHandler                (void) __attribute__ ((weak, alias("Fault_Handler")));

/*******************************************************************************
 Vector table for STM32F0xx (Cortex-M0)
*******************************************************************************/

void (* const vectors[])(void) __attribute__ ((used, section(".vectors"))) =
{
	/* Initial stack pointer */
	(void(*)(void))__initial_msp,

	/* Core exceptions */
	Reset_Handler,      /* Reset                                   */
	NMI_Handler,        /* Non-maskable interrupt                  */
	HardFault_Handler,  /* All classes of faults                   */
	MemManage_Handler,  /* Memory management                       */
	BusFault_Handler,   /* Pre-fetch fault, memory access fault    */
	UsageFault_Handler, /* Undefined instruction or illegal state  */
	0, 0, 0, 0,         /* Reserved                                */
	SVC_Handler,        /* System service call via SWI instruction */
	DebugMon_Handler,   /* Debug Monitor                           */
	0,                  /* Reserved                                */
	PendSV_Handler,     /* Pendable request for system service     */
	SysTick_Handler,    /* System tick timer                       */

#ifndef __NO_EXTERNAL_INTERRUPTS

	/* External interrupts */
	WWDG_IRQHandler,
	PVD_IRQHandler,
	RTC_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_1_IRQHandler,
	EXTI2_3_IRQHandler,
	EXTI4_5_IRQHandler,
	TSC_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_3_IRQHandler,
	DMA1_Channel4_5_IRQHandler,
	ADC1_IRQHandler,
	TIM1_BRK_UP_TRG_COM_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	TIM6_IRQHandler,
	TIM7_IRQHandler,
	TIM14_IRQHandler,
	TIM15_IRQHandler,
	TIM16_IRQHandler,
	TIM17_IRQHandler,
	I2C1_IRQHandler,
	I2C2_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler,
#if defined(USB_IRQn)||defined(CEC_CAN_IRQn)||defined(USART3_6_IRQn)||defined(USART2_IRQn)||defined(USART1_IRQn)
	USART1_IRQHandler,
#endif
#if defined(USB_IRQn)||defined(CEC_CAN_IRQn)||defined(USART3_6_IRQn)||defined(USART2_IRQn)
	USART2_IRQHandler,
#endif
#if defined(USB_IRQn)||defined(CEC_CAN_IRQn)||defined(USART3_6_IRQn)
	USART3_6_IRQHandler,
#endif
#if defined(USB_IRQn)||defined(CEC_CAN_IRQn)
	CEC_CAN_IRQHandler,
#endif
#if defined(USB_IRQn)
	USB_IRQHandler,
#endif

#endif//__NO_EXTERNAL_INTERRUPTS
};

/*******************************************************************************
 Specific definitions for the compiler
*******************************************************************************/

#if   defined(__CC_ARM)
#include "ARMCC/startup.h"
#elif defined(__ARMCOMPILER_VERSION)
#include "CLANG/startup.h"
#elif defined(__GNUC__)
#include "GNUCC/startup.h"
#else
#error Unknown compiler!
#endif

/*******************************************************************************
 Default reset handler
*******************************************************************************/

void Reset_Handler( void )
{
#if main_stack_size + proc_stack_size > 0
	/* Initialize the process stack pointer */
	__set_PSP((unsigned)__initial_sp);
	__set_CONTROL(CONTROL_SPSEL_Msk);
#endif
#if __FPU_USED
	/* Set CP10 and CP11 Full Access */
	SCB->CPACR = 0x00F00000U;
#endif
#ifndef __NO_SYSTEM_INIT
	/* Call the system clock intitialization function */
	SystemInit();
#endif
	/* Call the application's entry point */
	__main();
}

/******************************************************************************/
