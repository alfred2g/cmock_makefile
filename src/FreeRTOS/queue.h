#ifndef QUEUE_H
    #define QUEUE_H
    #ifdef __cplusplus
        extern "C" {
    #endif

    #include "task.h"

    struct QueueDefinition; /* Using old naming convention so as not to break kernel aware debuggers. */
    typedef struct QueueDefinition   * QueueHandle_t;

    typedef struct QueueDefinition   * QueueSetHandle_t;

    typedef struct QueueDefinition   * QueueSetMemberHandle_t;

    typedef struct xSTATIC_QUEUE
    {
        void * pvDummy1[ 3 ];

        union
        {
            void * pvDummy2;
            UBaseType_t uxDummy2;
        } u;

/*	StaticList_t xDummy3[ 2 ]; */
        UBaseType_t uxDummy4[ 3 ];
        uint8_t ucDummy5[ 2 ];

        #if ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
            uint8_t ucDummy6;
        #endif

        #if ( configUSE_QUEUE_SETS == 1 )
            void * pvDummy7;
        #endif

        #if ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t uxDummy8;
            uint8_t ucDummy9;
        #endif
    } StaticQueue_t;
    typedef StaticQueue_t StaticSemaphore_t;
/* For internal use only. */
    #define queueSEND_TO_BACK                     ( ( BaseType_t ) 0 )
    #define queueSEND_TO_FRONT                    ( ( BaseType_t ) 1 )
    #define queueOVERWRITE                        ( ( BaseType_t ) 2 )

/* For internal use only.  These definitions *must* match those in queue.c. */
    #define queueQUEUE_TYPE_BASE                  ( ( uint8_t ) 0U )
    #define queueQUEUE_TYPE_SET                   ( ( uint8_t ) 0U )
    #define queueQUEUE_TYPE_MUTEX                 ( ( uint8_t ) 1U )
    #define queueQUEUE_TYPE_COUNTING_SEMAPHORE    ( ( uint8_t ) 2U )
    #define queueQUEUE_TYPE_BINARY_SEMAPHORE      ( ( uint8_t ) 3U )
    #define queueQUEUE_TYPE_RECURSIVE_MUTEX       ( ( uint8_t ) 4U )

    #define xQueueCreate( uxQueueLength, uxItemSize )    xQueueGenericCreate( ( uxQueueLength ), ( uxItemSize ), ( queueQUEUE_TYPE_BASE ) )

/*
 *
 #if ( configSUPPORT_STATIC_ALLOCATION == 1 )
 #define xQueueCreateStatic( uxQueueLength, uxItemSize, pucQueueStorage, pxQueueBuffer )    xQueueGenericCreateStatic( ( uxQueueLength ), ( uxItemSize ), ( pucQueueStorage ), ( pxQueueBuffer ), ( queueQUEUE_TYPE_BASE ) )
 #endif
 #define xQueueSendToFront( xQueue, pvItemToQueue, xTicksToWait )    xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), ( xTicksToWait ), queueSEND_TO_FRONT )
 #define xQueueSendToBack( xQueue, pvItemToQueue, xTicksToWait )     xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), ( xTicksToWait ), queueSEND_TO_BACK )
 #define xQueueSend( xQueue, pvItemToQueue, xTicksToWait )           xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), ( xTicksToWait ), queueSEND_TO_BACK )
 #define xQueueOverwrite( xQueue, pvItemToQueue )                    xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), 0, queueOVERWRITE )
 *  BaseType_t xQueueGenericSend( QueueHandle_t xQueue,
 *                                const void * const pvItemToQueue,
 *                                TickType_t xTicksToWait,
 *                                const BaseType_t xCopyPosition );
 *  BaseType_t xQueuePeek( QueueHandle_t xQueue,
 *                         void * const pvBuffer,
 *                         TickType_t xTicksToWait );
 *  BaseType_t xQueuePeekFromISR( QueueHandle_t xQueue,
 *                                void * const pvBuffer ) PRIVILEGED_FUNCTION;
 *  BaseType_t xQueueReceive( QueueHandle_t xQueue,
 *                            void * const pvBuffer,
 *                            TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;
 *  */
    UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue );
    UBaseType_t uxQueueSpacesAvailable( const QueueHandle_t xQueue );
    void vQueueDelete( QueueHandle_t xQueue );

/*
 #define xQueueSendToFrontFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken )    xQueueGenericSendFromISR( ( xQueue ), ( pvItemToQueue ), ( pxHigherPriorityTaskWoken ), queueSEND_TO_FRONT )
 #define xQueueSendToBackFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken )     xQueueGenericSendFromISR( ( xQueue ), ( pvItemToQueue ), ( pxHigherPriorityTaskWoken ), queueSEND_TO_BACK )
 #define xQueueOverwriteFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken )      xQueueGenericSendFromISR( ( xQueue ), ( pvItemToQueue ), ( pxHigherPriorityTaskWoken ), queueOVERWRITE )
 #define xQueueSendFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken )           xQueueGenericSendFromISR( ( xQueue ), ( pvItemToQueue ), ( pxHigherPriorityTaskWoken ), queueSEND_TO_BACK )
 *  BaseType_t xQueueGenericSendFromISR( QueueHandle_t xQueue,
 *                                       const void * const pvItemToQueue,
 *                                       BaseType_t * const pxHigherPriorityTaskWoken,
 *                                       const BaseType_t xCopyPosition ) PRIVILEGED_FUNCTION;
 *  BaseType_t xQueueGiveFromISR( QueueHandle_t xQueue,
 *                                BaseType_t * const pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
 *  BaseType_t xQueueReceiveFromISR( QueueHandle_t xQueue,
 *                                   void * const pvBuffer,
 *                                   BaseType_t * const pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
 *  BaseType_t xQueueIsQueueEmptyFromISR( const QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
 *  BaseType_t xQueueIsQueueFullFromISR( const QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
 *  UBaseType_t uxQueueMessagesWaitingFromISR( const QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
 *  BaseType_t xQueueCRSendFromISR( QueueHandle_t xQueue,
 *                                  const void * pvItemToQueue,
 *                                  BaseType_t xCoRoutinePreviouslyWoken );
 *  BaseType_t xQueueCRReceiveFromISR( QueueHandle_t xQueue,
 *                                     void * pvBuffer,
 *                                     BaseType_t * pxTaskWoken );
 *  BaseType_t xQueueCRSend( QueueHandle_t xQueue,
 *                           const void * pvItemToQueue,
 *                           TickType_t xTicksToWait );
 *  BaseType_t xQueueCRReceive( QueueHandle_t xQueue,
 *                              void * pvBuffer,
 *                              TickType_t xTicksToWait );
 *  QueueHandle_t xQueueCreateMutex( const uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
 *  QueueHandle_t xQueueCreateMutexStatic( const uint8_t ucQueueType,
 *                                         StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
 *  QueueHandle_t xQueueCreateCountingSemaphore( const UBaseType_t uxMaxCount,
 *                                               const UBaseType_t uxInitialCount ) PRIVILEGED_FUNCTION;
 *  QueueHandle_t xQueueCreateCountingSemaphoreStatic( const UBaseType_t uxMaxCount,
 *                                                     const UBaseType_t uxInitialCount,
 *                                                     StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
 *  BaseType_t xQueueSemaphoreTake( QueueHandle_t xQueue,
 *                                  TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;
 *  TaskHandle_t xQueueGetMutexHolder( QueueHandle_t xSemaphore ) PRIVILEGED_FUNCTION;
 *  TaskHandle_t xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore ) PRIVILEGED_FUNCTION;
 *
 *  BaseType_t xQueueTakeMutexRecursive( QueueHandle_t xMutex,
 *                                       TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;
 *  BaseType_t xQueueGiveMutexRecursive( QueueHandle_t xMutex ) PRIVILEGED_FUNCTION;
 *
 #define xQueueReset( xQueue )    xQueueGenericReset( xQueue, pdFALSE )
 *
 #if ( configQUEUE_REGISTRY_SIZE > 0 )
 *      void vQueueAddToRegistry( QueueHandle_t xQueue,
 *                                const char * pcQueueName ) PRIVILEGED_FUNCTION;
 #endif
 *
 #if ( configQUEUE_REGISTRY_SIZE > 0 )
 *      void vQueueUnregisterQueue( QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
 #endif
 *
 #if ( configQUEUE_REGISTRY_SIZE > 0 )
 *      const char * pcQueueGetName( QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
 #endif
 */
    QueueHandle_t xQueueGenericCreate( const UBaseType_t uxQueueLength,
                                       const UBaseType_t uxItemSize,
                                       const uint8_t ucQueueType );

    #if ( configSUPPORT_STATIC_ALLOCATION == 1 )
        QueueHandle_t xQueueGenericCreateStatic( const UBaseType_t uxQueueLength,
                                                 const UBaseType_t uxItemSize,
                                                 uint8_t * pucQueueStorage,
                                                 StaticQueue_t * pxStaticQueue,
                                                 const uint8_t ucQueueType );
    #endif

    QueueSetHandle_t xQueueCreateSet( const UBaseType_t uxEventQueueLength );

    BaseType_t xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet );

    BaseType_t xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet );

    QueueSetMemberHandle_t xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                const TickType_t xTicksToWait );

    QueueSetMemberHandle_t xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet );

    void vQueueWaitForMessageRestricted( QueueHandle_t xQueue,
                                         TickType_t xTicksToWait,
                                         const BaseType_t xWaitIndefinitely );
    BaseType_t xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue );
    void vQueueSetQueueNumber( QueueHandle_t xQueue,
                               UBaseType_t uxQueueNumber );
    UBaseType_t uxQueueGetQueueNumber( QueueHandle_t xQueue );
    uint8_t ucQueueGetQueueType( QueueHandle_t xQueue );


    #ifdef __cplusplus
}
    #endif

#endif /* QUEUE_H */
