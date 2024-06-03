#pragma once
#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_usb.h>

/* === START furi_hal_usb_i.h === */
// https://github.com/flipperdevices/flipperzero-firmware/blob/03196fa11007c0f1e002cbb0b82102d8492456b5/targets/f7/furi_hal/furi_hal_usb_i.h#L5
#define USB_EP0_SIZE 8

enum UsbDevDescStr {
    UsbDevLang = 0,
    UsbDevManuf = 1,
    UsbDevProduct = 2,
    UsbDevSerial = 3,
};
/* ===   END furi_hal_usb_i.h === */

void usb_mtp_init(void);
void usb_mtp_deinit(void);
void usb_mtp_wakeup(void);
void usb_mtp_suspend(void);

#define USB_EP0_SIZE 64
#define USB_MTP_RX_EP 0x01
#define USB_MTP_TX_EP 0x81
#define USB_MTP_RX_EP_SIZE 64
#define USB_MTP_TX_EP_SIZE 64

static const struct usb_string_descriptor dev_manuf_desc = USB_STRING_DESC("Flipper Devices Inc.");
static const struct usb_string_descriptor dev_prod_desc = USB_STRING_DESC("MTP Device");

struct MtpDescriptor {
    struct usb_config_descriptor config;
    struct usb_interface_descriptor intf;
    struct usb_endpoint_descriptor ep_rx;
    struct usb_endpoint_descriptor ep_tx;
} __attribute__((packed));

static const struct usb_device_descriptor usb_mtp_dev_descr = {
    .bLength = sizeof(struct usb_device_descriptor),
    .bDescriptorType = USB_DTYPE_DEVICE,
    .bcdUSB = VERSION_BCD(2, 0, 0),
    .bDeviceClass = USB_CLASS_STILL_IMAGE, // MTP falls under Still Image class
    .bDeviceSubClass = 1, // Subclass for MTP
    .bDeviceProtocol = 1, // Protocol for MTP
    .bMaxPacketSize0 = USB_EP0_SIZE,
    .idVendor = 0x0483, // STMicroelectronics
    .idProduct = 0x5741, // Custom Product ID
    .bcdDevice = VERSION_BCD(1, 0, 0),
    .iManufacturer = UsbDevManuf, // UsbDevManuf
    .iProduct = UsbDevProduct, // UsbDevProduct
    .iSerialNumber = UsbDevSerial, // UsbDevSerial
    .bNumConfigurations = 1,
};

static const struct MtpDescriptor usb_mtp_cfg_descr = {
    .config =
        {
            .bLength = sizeof(struct usb_config_descriptor),
            .bDescriptorType = USB_DTYPE_CONFIGURATION,
            .wTotalLength = sizeof(struct MtpDescriptor),
            .bNumInterfaces = 1,
            .bConfigurationValue = 1,
            .iConfiguration = NO_DESCRIPTOR,
            .bmAttributes = USB_CFG_ATTR_RESERVED | USB_CFG_ATTR_SELFPOWERED,
            .bMaxPower = USB_CFG_POWER_MA(100),
        },
    .intf =
        {
            .bLength = sizeof(struct usb_interface_descriptor),
            .bDescriptorType = USB_DTYPE_INTERFACE,
            .bInterfaceNumber = 0,
            .bAlternateSetting = 0,
            .bNumEndpoints = 2,
            .bInterfaceClass = USB_CLASS_STILL_IMAGE,
            .bInterfaceSubClass = 1, // Subclass for MTP
            .bInterfaceProtocol = 1, // Protocol for MTP
            .iInterface = NO_DESCRIPTOR,
        },
    .ep_rx =
        {
            .bLength = sizeof(struct usb_endpoint_descriptor),
            .bDescriptorType = USB_DTYPE_ENDPOINT,
            .bEndpointAddress = USB_MTP_RX_EP,
            .bmAttributes = USB_EPTYPE_BULK,
            .wMaxPacketSize = USB_MTP_RX_EP_SIZE,
            .bInterval = 0,
        },
    .ep_tx =
        {
            .bLength = sizeof(struct usb_endpoint_descriptor),
            .bDescriptorType = USB_DTYPE_ENDPOINT,
            .bEndpointAddress = USB_MTP_TX_EP,
            .bmAttributes = USB_EPTYPE_BULK,
            .wMaxPacketSize = USB_MTP_TX_EP_SIZE,
            .bInterval = 0,
        },
};

FuriHalUsbInterface usb_mtp_interface = {
    .init = usb_mtp_init,
    .deinit = usb_mtp_deinit,
    .wakeup = usb_mtp_wakeup,
    .suspend = usb_mtp_suspend,

    .dev_descr = (struct usb_device_descriptor*)&usb_mtp_dev_descr,

    .str_manuf_descr = (void*)&dev_manuf_desc,
    .str_prod_descr = (void*)&dev_prod_desc,
    .str_serial_descr = NULL,

    .cfg_descr = (void*)&usb_mtp_cfg_descr,
};