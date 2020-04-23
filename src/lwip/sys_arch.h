
#include "lwip_defs.h"

err_t sys_mbox_new( sys_mbox_t *pxMailBox, int iSize );
void sys_mbox_free( volatile sys_mbox_t *pxMailBox );
