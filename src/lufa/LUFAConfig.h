#ifndef LUFA_CONFIG_H_
#define LUFA_CONFIG_H_
	#if (ARCH == ARCH_AVR8)
		#define USE_STATIC_OPTIONS               (USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)
		#define USB_DEVICE_ONLY
		#define USE_FLASH_DESCRIPTORS
		#define FIXED_CONTROL_ENDPOINT_SIZE      8
		#define FIXED_NUM_CONFIGURATIONS         1
	#elif (ARCH == ARCH_XMEGA)
		#define USE_STATIC_OPTIONS               (USB_DEVICE_OPT_FULLSPEED | USB_OPT_RC32MCLKSRC | USB_OPT_BUSEVENT_PRIHIGH)
		#define USE_FLASH_DESCRIPTORS
		#define FIXED_CONTROL_ENDPOINT_SIZE      8
		#define FIXED_NUM_CONFIGURATIONS         1
		#define MAX_ENDPOINT_INDEX               3
	#else
		#error Unsupported architecture for this LUFA configuration file.
	#endif
#endif // LUFA_CONFIG_H_
