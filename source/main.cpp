/**
 * @file
 * @author	Richard Osterloh <richard.osterloh@gmail.com>
 * @date 	  08 September 2015
 * @brief
 */

#include "mbed-drivers/mbed.h"
#include "ble/BLE.h"
#include "SSD1306.h"

static SSD1306 *display = SSD1306::Instance(p8, p9);

/*
 * This function is called every time we scan an advertisement.
 */
void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params)
{

}

void onBleInitError(BLE &ble, ble_error_t error)
{
    /* Initialization error handling should go here */
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        onBleInitError(ble, error);
        return;
    }

    if (ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    ble.gap().setScanParams(1800 /* scan interval */, 1500 /* scan window */);
    ble.gap().startScan(advertisementCallback);
}

void app_start(int, char *[])
{
    BLE::Instance().init(bleInitComplete);
}
