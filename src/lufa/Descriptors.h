#ifndef DESCRIPTORS_H_
#define DESCRIPTORS_H_

#include <avr/pgmspace.h>
#include <LUFA/Drivers/USB/USB.h>

typedef struct {
    USB_Descriptor_Configuration_Header_t Config;

    // Keyboard HID Interface
    USB_Descriptor_Interface_t            HID1_KeyboardInterface;
    USB_HID_Descriptor_HID_t              HID1_KeyboardHID;
    USB_Descriptor_Endpoint_t             HID1_ReportINEndpoint;

    // Mouse HID Interface
    USB_Descriptor_Interface_t            HID2_MouseInterface;
    USB_HID_Descriptor_HID_t              HID2_MouseHID;
    USB_Descriptor_Endpoint_t             HID2_ReportINEndpoint;
} USB_Descriptor_Configuration_t;

/** Enum for the device interface descriptor IDs within the device. Each interface descriptor
    *  should have a unique ID index associated with it, which can be used to refer to the
    *  interface from other descriptors.
    */
enum InterfaceDescriptors_t
{
    INTERFACE_ID_Keyboard = 0, /**< Keyboard interface descriptor ID */
    INTERFACE_ID_Mouse    = 1, /**< Mouse interface descriptor ID */
};

/** Enum for the device string descriptor IDs within the device. Each string descriptor should
    *  have a unique ID index associated with it, which can be used to refer to the string from
    *  other descriptors.
    */
enum StringDescriptors_t
{
    STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
    STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
    STRING_ID_Product      = 2, /**< Product string ID */
};

/** Endpoint address of the Keyboard HID reporting IN endpoint. */
#define KEYBOARD_IN_EPADDR        (ENDPOINT_DIR_IN | 1)

/** Endpoint address of the Mouse HID reporting IN endpoint. */
#define MOUSE_IN_EPADDR           (ENDPOINT_DIR_IN | 3)

/** Size in bytes of each of the HID reporting IN endpoints. */
#define HID_EPSIZE                8

uint16_t CALLBACK_USB_GetDescriptor(
    const uint16_t wValue,
    const uint16_t wIndex,
    const void** const DescriptorAddress
) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif // DESCRIPTORS_H_