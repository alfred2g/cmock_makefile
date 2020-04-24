#include <unity.h>

#include "sys_arch.h"
#include <mock_queue.h>
#include <mock_task.h>

void test_sys_mbox_new( void )
{
    xQueueGenericCreate_ExpectAnyArgsAndReturn( 0 );
    sys_mbox_new( NULL, 5 );
}


void test_sys_mbox_free( void )
{
}
