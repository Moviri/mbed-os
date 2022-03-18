#include "zboss_api.h"
#include "zb_error_handler.h"
#include "zigbee_helpers.h"



/**@brief Zigbee stack event handler.
 *
 * @param[in]   bufid   Reference to the Zigbee stack buffer used to pass signal.
 */
__WEAK void zboss_signal_handler(zb_bufid_t bufid)
{
    /* No application-specific behavior is required. Call default signal handler. */
    ZB_ERROR_CHECK(zigbee_default_signal_handler(bufid));

    if (bufid)
    {
        zb_buf_free(bufid);
    }
}
