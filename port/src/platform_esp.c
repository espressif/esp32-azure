// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#ifdef _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/xio.h"
#include "azure_c_shared_utility/tlsio_openssl.h"
#include "azure_c_shared_utility/xlogging.h"
#include "esp_log.h"
#include "tlsio_pal.h"
#include "agenttime_esp.h"
#include "lwip/apps/sntp.h"

static const char* TAG = "esp-azure";

int platform_init(void)
{
    initialize_sntp();
    ESP_LOGI(TAG, "ESP platform integration initialized!");
    time_t now = sntp_get_current_timestamp();

    char strftime_buf[64];
    struct tm timeinfo;

    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time is: %s", strftime_buf);

    return 0;
}

const IO_INTERFACE_DESCRIPTION* platform_get_default_tlsio(void)
{
    return tlsio_pal_get_interface_description();
    return NULL;
}

void platform_deinit(void)
{
	ESP_LOGI(TAG, "Shutting down ESP platform integration...");
    finalize_sntp();
}

STRING_HANDLE platform_get_platform_info(PLATFORM_INFO_OPTION options)
{
	//options is not used at the moment
	(void)options;
    // Expected format: "(<runtime name>; <operating system name>; <platform>)"
    return STRING_construct("(native; freertos; esp platform)");
}
