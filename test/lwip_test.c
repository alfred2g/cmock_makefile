#include <unity.h>
#include "sys_arch.h"
#include <mock_queue.h>
#include <mock_task.h>

#include <stdlib.h>
#include <string.h>

static int critical_count = 0;
void vPortEnterCritical( void )
{
    critical_count++;
}
void vPortExitCritical( void )
{
    critical_count--;
}

static uint8_t assert_result = 0;
void vPortConfigAssert( uint8_t result )
{
    assert_result = result;
}

void test_sys_mbox_new_happy_path( void )
{
    err_t ret;

    sys_mbox_t * pxMailBox = malloc( sizeof( sys_mbox_t ) );

    xQueueGenericCreate_ExpectAndReturn( 5,
                                         sizeof( void * ),
                                         queueQUEUE_TYPE_BASE,
                                         ( void * ) 50 );
    ret = sys_mbox_new( pxMailBox, 5 );

    TEST_ASSERT_EQUAL_INT( ERR_OK, ret );
    TEST_ASSERT_EQUAL( NULL, pxMailBox->xTask );

    free( pxMailBox );
}

void test_sys_mbox_new_Queue_create_fail( void )
{
    err_t ret;

    xQueueGenericCreate_ExpectAnyArgsAndReturn( NULL );
    ret = sys_mbox_new( NULL, 5 );

    TEST_ASSERT_EQUAL_INT( ERR_MEM, ret );
}


/* testig sys_mbos_free */
void test_sys_mbox_free_happy_path( void )
{
    sys_mbox_t * pxMailBox = malloc( sizeof( sys_mbox_t ) );

    memset( pxMailBox, 0x01, sizeof( sys_mbox_t ) );

    uxQueueMessagesWaiting_ExpectAnyArgsAndReturn( 0 );
    xTaskAbortDelay_ExpectAnyArgsAndReturn( 0 );
    vQueueDelete_ExpectAnyArgs();
    sys_mbox_free( pxMailBox );

    TEST_ASSERT_EQUAL( pxMailBox->xMbox, NULL );

    memset( pxMailBox, 0x00, sizeof( sys_mbox_t ) );
    uxQueueMessagesWaiting_ExpectAnyArgsAndReturn( 0 );
    sys_mbox_free( pxMailBox );

    TEST_ASSERT_EQUAL( pxMailBox->xMbox, NULL );

    TEST_ASSERT_EQUAL_MESSAGE( critical_count, 0,
                               "Crtitial functions enter and exit do not match" );
    free( pxMailBox );
}

void test_sys_mbox_free_appy_path_error( void )
{
    sys_mbox_t * pxMailBox = NULL;

    sys_mbox_free( pxMailBox );

    pxMailBox = malloc( sizeof( sys_mbox_t ) );
    memset( pxMailBox, 0x00, sizeof( sys_mbox_t ) );

    uxQueueMessagesWaiting_ExpectAnyArgsAndReturn( 1 );
    sys_mbox_free( pxMailBox );

    TEST_ASSERT_EQUAL( assert_result, 0 );
    TEST_ASSERT_EQUAL_MESSAGE( critical_count, 0,
                               "Crtitial functions enter and exit do not match" );
}
