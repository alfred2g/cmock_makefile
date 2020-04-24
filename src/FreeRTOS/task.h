/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef INC_TASK_H
#define INC_TASK_H

#include <portmacro.h>

//#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * MACROS AND DEFINITIONS
 *----------------------------------------------------------*/

#define tskKERNEL_VERSION_NUMBER "V10.3.1"
#define tskKERNEL_VERSION_MAJOR 10
#define tskKERNEL_VERSION_MINOR 3
#define tskKERNEL_VERSION_BUILD 1

/* MPU region parameters passed in ulParameters
 * of MemoryRegion_t struct. */
#define tskMPU_REGION_READ_ONLY			( 1UL << 0UL )
#define tskMPU_REGION_READ_WRITE		( 1UL << 1UL )
#define tskMPU_REGION_EXECUTE_NEVER		( 1UL << 2UL )
#define tskMPU_REGION_NORMAL_MEMORY		( 1UL << 3UL )
#define tskMPU_REGION_DEVICE_MEMORY		( 1UL << 4UL )

/**
 * task. h
 *
 * Type by which tasks are referenced.  For example, a call to xTaskCreate
 * returns (via a pointer parameter) an TaskHandle_t variable that can then
 * be used as a parameter to vTaskDelete to delete the task.
 *
 * \defgroup TaskHandle_t TaskHandle_t
 * \ingroup Tasks
 */
struct tskTaskControlBlock; /* The old naming convention is used to prevent breaking kernel aware debuggers. */
typedef struct tskTaskControlBlock* TaskHandle_t;

/*
 * Defines the prototype to which the application task hook function must
 * conform.
 */
typedef BaseType_t (*TaskHookFunction_t)( void * );

/* Task states returned by eTaskGetState. */
typedef enum
{
	eRunning = 0,	/* A task is querying the state of itself, so must be running. */
	eReady,			/* The task being queried is in a read or pending ready list. */
	eBlocked,		/* The task being queried is in the Blocked state. */
	eSuspended,		/* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
	eDeleted,		/* The task being queried has been deleted, but its TCB has not yet been freed. */
	eInvalid		/* Used as an 'invalid state' value. */
} eTaskState;

/* Actions that can be performed when vTaskNotify() is called. */
typedef enum
{
	eNoAction = 0,				/* Notify the task without updating its notify value. */
	eSetBits,					/* Set bits in the task's notification value. */
	eIncrement,					/* Increment the task's notification value. */
	eSetValueWithOverwrite,		/* Set the task's notification value to a specific value even if the previous value has not yet been read by the task. */
	eSetValueWithoutOverwrite	/* Set the task's notification value if the previous value has been read by the task. */
} eNotifyAction;

/*
 * Used internally only.
 */
typedef struct xTIME_OUT
{
	BaseType_t xOverflowCount;
	TickType_t xTimeOnEntering;
} TimeOut_t;

/*
 * Defines the memory ranges allocated to the task when an MPU is used.
 */
typedef struct xMEMORY_REGION
{
	void *pvBaseAddress;
	uint32_t ulLengthInBytes;
	uint32_t ulParameters;
} MemoryRegion_t;

/*
typedef struct xTASK_PARAMETERS
{
	TaskFunction_t pvTaskCode;
	const char * const pcName;
	configSTACK_DEPTH_TYPE usStackDepth;
	void *pvParameters;
	UBaseType_t uxPriority;
	StackType_t *puxStackBuffer;
	MemoryRegion_t xRegions[ portNUM_CONFIGURABLE_REGIONS ];
	#if ( ( portUSING_MPU_WRAPPERS == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
		StaticTask_t * const pxTaskBuffer;
	#endif
} TaskParameters_t;
*/

/*
typedef struct xTASK_STATUS
{
	TaskHandle_t xHandle;
	const char *pcTaskName;
	UBaseType_t xTaskNumber;
	eTaskState eCurrentState;
	UBaseType_t uxCurrentPriority;
	UBaseType_t uxBasePriority;
	uint32_t ulRunTimeCounter;
	StackType_t *pxStackBase;
	configSTACK_DEPTH_TYPE usStackHighWaterMark;
} TaskStatus_t;
*/

/* Possible return values for eTaskConfirmSleepModeStatus(). */
typedef enum
{
	eAbortSleep = 0,		/* A task has been made ready or a context switch pended since portSUPPORESS_TICKS_AND_SLEEP() was called - abort entering a sleep mode. */
	eStandardSleep,			/* Enter a sleep mode that will not last any longer than the expected idle time. */
	eNoTasksWaitingTimeout	/* No tasks are waiting for a timeout so it is safe to enter a sleep mode that can only be exited by an external interrupt. */
} eSleepModeStatus;

/**
 * Defines the priority used by the idle task.  This must not be modified.
 *
 * \ingroup TaskUtils
 */
#define tskIDLE_PRIORITY			( ( UBaseType_t ) 0U )

/**
 * task. h
 *
 * Macro for forcing a context switch.
 *
 * \defgroup taskYIELD taskYIELD
 * \ingroup SchedulerControl
 */
#define taskYIELD()					portYIELD()

#define taskENTER_CRITICAL()		portENTER_CRITICAL()
#define taskENTER_CRITICAL_FROM_ISR() portSET_INTERRUPT_MASK_FROM_ISR()

#define taskEXIT_CRITICAL()			portEXIT_CRITICAL()
#define taskEXIT_CRITICAL_FROM_ISR( x ) portCLEAR_INTERRUPT_MASK_FROM_ISR( x )
#define taskDISABLE_INTERRUPTS()	portDISABLE_INTERRUPTS()
#define taskENABLE_INTERRUPTS()		portENABLE_INTERRUPTS()

#define taskSCHEDULER_SUSPENDED		( ( BaseType_t ) 0 )
#define taskSCHEDULER_NOT_STARTED	( ( BaseType_t ) 1 )
#define taskSCHEDULER_RUNNING		( ( BaseType_t ) 2 )


/*-----------------------------------------------------------
 * TASK CREATION API
 *----------------------------------------------------------*/
/*
#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
	BaseType_t xTaskCreate(	TaskFunction_t pxTaskCode,
							const char * const pcName,
							const configSTACK_DEPTH_TYPE usStackDepth,
							void * const pvParameters,
							UBaseType_t uxPriority,
							TaskHandle_t * const pxCreatedTask ) PRIVILEGED_FUNCTION;
#endif

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,
									const char * const pcName,
									const uint32_t ulStackDepth,
									void * const pvParameters,
									UBaseType_t uxPriority,
									StackType_t * const puxStackBuffer,
									StaticTask_t * const pxTaskBuffer ) PRIVILEGED_FUNCTION;
#endif

#if( portUSING_MPU_WRAPPERS == 1 )
	BaseType_t xTaskCreateRestricted( const TaskParameters_t * const pxTaskDefinition, TaskHandle_t *pxCreatedTask ) PRIVILEGED_FUNCTION;
#endif

#if( ( portUSING_MPU_WRAPPERS == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
	BaseType_t xTaskCreateRestrictedStatic( const TaskParameters_t * const pxTaskDefinition, TaskHandle_t *pxCreatedTask ) PRIVILEGED_FUNCTION;
#endif
void vTaskAllocateMPURegions( TaskHandle_t xTask, const MemoryRegion_t * const pxRegions ) PRIVILEGED_FUNCTION;
void vTaskDelete( TaskHandle_t xTaskToDelete ) PRIVILEGED_FUNCTION;
void vTaskDelay( const TickType_t xTicksToDelay ) PRIVILEGED_FUNCTION;
void vTaskDelayUntil( TickType_t * const pxPreviousWakeTime, const TickType_t xTimeIncrement ) PRIVILEGED_FUNCTION;
*/
BaseType_t xTaskAbortDelay( TaskHandle_t xTask ) ;
/*
UBaseType_t uxTaskPriorityGet( const TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t uxTaskPriorityGetFromISR( const TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
eTaskState eTaskGetState( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
void vTaskGetInfo( TaskHandle_t xTask, TaskStatus_t *pxTaskStatus, BaseType_t xGetFreeStackSpace, eTaskState eState ) PRIVILEGED_FUNCTION;
void vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxNewPriority ) PRIVILEGED_FUNCTION;
void vTaskSuspend( TaskHandle_t xTaskToSuspend ) PRIVILEGED_FUNCTION;
void vTaskResume( TaskHandle_t xTaskToResume ) PRIVILEGED_FUNCTION;
BaseType_t xTaskResumeFromISR( TaskHandle_t xTaskToResume ) PRIVILEGED_FUNCTION;
void vTaskStartScheduler( void ) PRIVILEGED_FUNCTION;
void vTaskEndScheduler( void ) PRIVILEGED_FUNCTION;
void vTaskSuspendAll( void ) PRIVILEGED_FUNCTION;
BaseType_t xTaskResumeAll( void ) PRIVILEGED_FUNCTION;
TickType_t xTaskGetTickCount( void ) PRIVILEGED_FUNCTION;
TickType_t xTaskGetTickCountFromISR( void ) PRIVILEGED_FUNCTION;
UBaseType_t uxTaskGetNumberOfTasks( void ) PRIVILEGED_FUNCTION;
char *pcTaskGetName( TaskHandle_t xTaskToQuery ) PRIVILEGED_FUNCTION;
TaskHandle_t xTaskGetHandle( const char *pcNameToQuery ) PRIVILEGED_FUNCTION;
UBaseType_t uxTaskGetStackHighWaterMark( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
configSTACK_DEPTH_TYPE uxTaskGetStackHighWaterMark2( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
#ifdef configUSE_APPLICATION_TASK_TAG
	#if configUSE_APPLICATION_TASK_TAG == 1
		void vTaskSetApplicationTaskTag( TaskHandle_t xTask, TaskHookFunction_t pxHookFunction ) PRIVILEGED_FUNCTION;
		TaskHookFunction_t xTaskGetApplicationTaskTag( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
		TaskHookFunction_t xTaskGetApplicationTaskTagFromISR( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
	#endif
#endif
#if( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )
	void vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet, BaseType_t xIndex, void *pvValue ) PRIVILEGED_FUNCTION;
	void *pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery, BaseType_t xIndex ) PRIVILEGED_FUNCTION;

#endif
BaseType_t xTaskCallApplicationTaskHook( TaskHandle_t xTask, void *pvParameter ) PRIVILEGED_FUNCTION;
TaskHandle_t xTaskGetIdleTaskHandle( void ) PRIVILEGED_FUNCTION;
UBaseType_t uxTaskGetSystemState( TaskStatus_t * const pxTaskStatusArray, const UBaseType_t uxArraySize, uint32_t * const pulTotalRunTime ) PRIVILEGED_FUNCTION;
void vTaskList( char * pcWriteBuffer ) PRIVILEGED_FUNCTION;
void vTaskGetRunTimeStats( char *pcWriteBuffer ) PRIVILEGED_FUNCTION;
uint32_t ulTaskGetIdleRunTimeCounter( void ) PRIVILEGED_FUNCTION;
BaseType_t xTaskGenericNotify( TaskHandle_t xTaskToNotify, uint32_t ulValue, eNotifyAction eAction, uint32_t *pulPreviousNotificationValue ) PRIVILEGED_FUNCTION;
#define xTaskNotify( xTaskToNotify, ulValue, eAction ) xTaskGenericNotify( ( xTaskToNotify ), ( ulValue ), ( eAction ), NULL )
#define xTaskNotifyAndQuery( xTaskToNotify, ulValue, eAction, pulPreviousNotifyValue ) xTaskGenericNotify( ( xTaskToNotify ), ( ulValue ), ( eAction ), ( pulPreviousNotifyValue ) )
BaseType_t xTaskGenericNotifyFromISR( TaskHandle_t xTaskToNotify, uint32_t ulValue, eNotifyAction eAction, uint32_t *pulPreviousNotificationValue, BaseType_t *pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
#define xTaskNotifyFromISR( xTaskToNotify, ulValue, eAction, pxHigherPriorityTaskWoken ) xTaskGenericNotifyFromISR( ( xTaskToNotify ), ( ulValue ), ( eAction ), NULL, ( pxHigherPriorityTaskWoken ) )
#define xTaskNotifyAndQueryFromISR( xTaskToNotify, ulValue, eAction, pulPreviousNotificationValue, pxHigherPriorityTaskWoken ) xTaskGenericNotifyFromISR( ( xTaskToNotify ), ( ulValue ), ( eAction ), ( pulPreviousNotificationValue ), ( pxHigherPriorityTaskWoken ) )
BaseType_t xTaskNotifyWait( uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;
#define xTaskNotifyGive( xTaskToNotify ) xTaskGenericNotify( ( xTaskToNotify ), ( 0 ), eIncrement, NULL )
void vTaskNotifyGiveFromISR( TaskHandle_t xTaskToNotify, BaseType_t *pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
uint32_t ulTaskNotifyTake( BaseType_t xClearCountOnExit, TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;
BaseType_t xTaskNotifyStateClear( TaskHandle_t xTask );
uint32_t ulTaskNotifyValueClear( TaskHandle_t xTask, uint32_t ulBitsToClear ) PRIVILEGED_FUNCTION;
void vTaskSetTimeOutState( TimeOut_t * const pxTimeOut ) PRIVILEGED_FUNCTION;
BaseType_t xTaskCheckForTimeOut( TimeOut_t * const pxTimeOut, TickType_t * const pxTicksToWait ) PRIVILEGED_FUNCTION;
BaseType_t xTaskIncrementTick( void ) PRIVILEGED_FUNCTION;
void vTaskPlaceOnEventList( List_t * const pxEventList, const TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;
void vTaskPlaceOnUnorderedEventList( List_t * pxEventList, const TickType_t xItemValue, const TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;
void vTaskPlaceOnEventListRestricted( List_t * const pxEventList, TickType_t xTicksToWait, const BaseType_t xWaitIndefinitely ) PRIVILEGED_FUNCTION;
BaseType_t xTaskRemoveFromEventList( const List_t * const pxEventList ) PRIVILEGED_FUNCTION;
void vTaskRemoveFromUnorderedEventList( ListItem_t * pxEventListItem, const TickType_t xItemValue ) PRIVILEGED_FUNCTION;
portDONT_DISCARD void vTaskSwitchContext( void ) PRIVILEGED_FUNCTION;
TickType_t uxTaskResetEventItemValue( void ) PRIVILEGED_FUNCTION;
TaskHandle_t xTaskGetCurrentTaskHandle( void ) PRIVILEGED_FUNCTION;
void vTaskMissedYield( void ) PRIVILEGED_FUNCTION;
BaseType_t xTaskGetSchedulerState( void ) PRIVILEGED_FUNCTION;
BaseType_t xTaskPriorityInherit( TaskHandle_t const pxMutexHolder ) PRIVILEGED_FUNCTION;
BaseType_t xTaskPriorityDisinherit( TaskHandle_t const pxMutexHolder ) PRIVILEGED_FUNCTION;
void vTaskPriorityDisinheritAfterTimeout( TaskHandle_t const pxMutexHolder, UBaseType_t uxHighestPriorityWaitingTask ) PRIVILEGED_FUNCTION;
UBaseType_t uxTaskGetTaskNumber( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
void vTaskSetTaskNumber( TaskHandle_t xTask, const UBaseType_t uxHandle ) PRIVILEGED_FUNCTION;
void vTaskStepTick( const TickType_t xTicksToJump ) PRIVILEGED_FUNCTION;
BaseType_t xTaskCatchUpTicks( TickType_t xTicksToCatchUp ) PRIVILEGED_FUNCTION;
eSleepModeStatus eTaskConfirmSleepModeStatus( void ) PRIVILEGED_FUNCTION;
TaskHandle_t pvTaskIncrementMutexHeldCount( void ) PRIVILEGED_FUNCTION;
void vTaskInternalSetTimeOutState( TimeOut_t * const pxTimeOut ) PRIVILEGED_FUNCTION;

*/
#ifdef __cplusplus
}
#endif
#endif /* INC_TASK_H */



