#pragma once
#include <gui/view.h>
#include <gui/modules/submenu.h>
#include <gui/modules/popup.h>
#include <gui/scene_manager.h>
#include <furi_hal.h>

typedef enum MTPState {
    MTPStateOffline,
    MTPStateReady,
    MTPStateBusy,
    MTPStateCanceled,
    MTPStateError
} MTPState;

typedef struct AppMTP {
    Submenu* menu;
    View* view;

    bool usb_connected;

    // usb stuff
    FuriHalUsbInterface* old_usb;
    FuriThread* worker_thread;
    usbd_device* dev;

    MTPState state;
    bool is_working;
} AppMTP;

AppMTP* MTP_alloc();
void MTP_free(void* ptr);
View* MTP_get_view(void* ptr);
void MTP_on_enter(void* context);
bool MTP_on_event(void* context, SceneManagerEvent event);
void MTP_on_exit(void* context);
