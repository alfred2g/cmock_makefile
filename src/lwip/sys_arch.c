#include "sys_arch.h"

#include "queue.h"
#include "task.h"

#include <stdio.h>

err_t sys_mbox_new( sys_mbox_t * pxMailBox,
                    int iSize )
{
    err_t xReturn = ERR_MEM;
    sys_mbox_t pxTempMbox;

    pxTempMbox.xMbox = xQueueCreate( iSize, sizeof( void * ) );

    if( pxTempMbox.xMbox != NULL )
    {
        pxTempMbox.xTask = NULL;
        *pxMailBox = pxTempMbox;
        xReturn = ERR_OK;
    }

    return xReturn;
}


void sys_mbox_free( volatile sys_mbox_t * pxMailBox )
{
    unsigned long ulMessagesWaiting;
    QueueHandle_t xMbox;
    TaskHandle_t xTask;

    if( pxMailBox != NULL )
    {
        ulMessagesWaiting = uxQueueMessagesWaiting( pxMailBox->xMbox );
        configASSERT( ( ulMessagesWaiting == 0 ) );

        taskENTER_CRITICAL();
        xMbox = pxMailBox->xMbox;
        xTask = pxMailBox->xTask;
        pxMailBox->xMbox = NULL;
        taskEXIT_CRITICAL();

        if( xTask != NULL )
        {
            xTaskAbortDelay( xTask );
        }

        if( xMbox != NULL )
        {
            vQueueDelete( xMbox );
        }
    }
}
