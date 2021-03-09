#ifndef HID_H_
#define HID_H_

#include <hidapi/hidapi.h>

#pragma comment(lib, "hidapi.lib")

#define HID_STAT_ADDED					1
#define HID_STAT_PARSED					2

#define HID_CONNECT_HIDINPUT            0x01
#define HID_CONNECT_HIDRAW              0x04
#define HID_CONNECT_HIDDEV              0x08
#define HID_CONNECT_FF                  0x20
#define HID_CONNECT_DEFAULT     (HID_CONNECT_HIDINPUT|HID_CONNECT_HIDRAW| HID_CONNECT_HIDDEV|HID_CONNECT_FF)

#define HID_GD_WHEEL					0x00010038

#define HID_UP_GENDESK		0x00010000
#define HID_UP_BUTTON		0x00090000

#define HID_REQ_GET_REPORT              0x01
#define HID_REQ_SET_REPORT				0x09

#define MSC_SCAN		0x04

#define REL_HWHEEL		0x06

//#define USB_INTERFACE_PROTOCOL_KEYBOARD 1
//#define USB_INTERFACE_PROTOCOL_MOUSE    2

//Hack to make detection work without having to install WinUSB on the correct interface
#define USB_INTERFACE_PROTOCOL_KEYBOARD 0
#define USB_INTERFACE_PROTOCOL_MOUSE    0

static const GUID GUID_DEVINTERFACE = { 0xDEE824EF, 0x729B, 0x4A0E, 0x9C, 0x14, 0xB7, 0x11, 0x7D, 0x33, 0xA8, 0x17 };

typedef enum
{
HID_TYPE_OTHER,
HID_TYPE_USBMOUSE,
HID_TYPE_USBNONE
} hid_type;

struct hid_input {
	struct input_dev *input;
};

struct hid_field {
	unsigned  application;		/* application usage for this field */
	struct hid_input *hidinput;     /* associated input structure */
};

struct hid_usage {
	unsigned  hid;
	__u16     code;                 /* input driver code */
	__u8      type;                 /* input driver type */
};

struct hid_driver {
	char *name;
	const struct hid_device_id *id_table;
	bool (*match)(struct hid_device* dev, bool ignore_special_driver);
	int (*probe)(struct hid_device *dev, const struct hid_device_id *id);
	void (*remove)(struct hid_device *dev);
	int (*raw_event)(struct hid_device *hdev, struct hid_report *report, u8 *data, int size);
	int (*event)(struct hid_device *hdev, struct hid_field *field, struct hid_usage *usage, __s32 value);
	int (*input_configured)(struct hid_device *hdev,
					struct hid_input *hidinput);
	int (*input_mapping)(struct hid_device *hdev,
					struct hid_input *hidinput, struct hid_field *field,
					struct hid_usage *usage, unsigned long **bit, int *max);
};

struct hid_device_id {
	__u16 bus;
	__u32 vendor;
	__u32 product;
};

struct hid_device {
	__u16 product;
	enum hid_type type;
	struct device dev;
	struct hid_ll_driver *ll_driver;
	unsigned int status;
	struct hid_driver *driver;
};

struct hid_ll_driver {
	int (*start)(struct hid_device *hdev);
	void (*stop)(struct hid_device *hdev);
	int (*parse)(struct hid_device *hdev);
};

inline int ll_start(struct hid_device *hdev) {
	printf("ll_start\n");
	return 0;
}

inline void ll_stop(struct hid_device *hdev) {
	printf("ll_stop\n");
}

inline int ll_parse(struct hid_device *hdev) {
	printf("ll_parse\n");
	return 0;
}

inline void dev_err(struct device** dev, const char* msg) {
	printf("dev_err device=%s msg=%s", (*dev)->init_name, msg);
}

inline void dev_info(struct device** dev, const char* msg) {
	printf("dev_err device=%s msg=%s", (*dev)->init_name, msg);
}

inline void *dev_get_drvdata(const struct device *dev) {
	return dev->driver_data;
}

inline void dev_set_drvdata(struct device *dev, void *data) {
	dev->driver_data = data;
}

inline int hid_connect(struct hid_device *hdev, unsigned int connect_mask) {
	printf("hid_connect\n");
	return 0;
}

inline int hid_parse(struct hid_device *hdev) {
	int ret;

	if (hdev->status & HID_STAT_PARSED)
		return 0;

	ret = hdev->ll_driver->parse(hdev);
	if (!ret)
		hdev->status |= HID_STAT_PARSED;

	return ret;
}

inline void *hid_get_drvdata(struct hid_device *hdev) {
	return dev_get_drvdata(&hdev->dev);
}

inline void hid_set_drvdata(struct hid_device *hdev, void *data) {
	dev_set_drvdata(&hdev->dev, data);
}

inline int hid_hw_start(struct hid_device *hdev, unsigned int connect_mask) {
	int ret = hdev->ll_driver->start(hdev);
	if (ret || !connect_mask)
		return ret;
	ret = hid_connect(hdev, connect_mask);
	if (ret)
		hdev->ll_driver->stop(hdev);
	return ret;
}

inline void hid_hw_stop(struct hid_device *hdev) {
	hdev->ll_driver->stop(hdev);
}

inline void hid_err(struct hid_device *hdev, const char* msg, ...) {
	va_list args;
	va_start(args, msg);
	printf("hid_err device=%s", hdev->dev.init_name);
	printf(msg, args);
	va_end(args);
}

inline void hid_map_usage(struct hid_input* hidinput,
	struct hid_usage* usage, unsigned long** bit, int* max,
	__u8 type, __u16 c)
{

};

#define container_of(ptr, type, member) (type*)((char*)(ptr)-(char*)&((type *)0)->member)

inline void close(struct device* dev) {
	printf("close %04X\n", (to_usb_device(dev))->descriptor.idProduct);
	struct usb_interface *intf = to_usb_interface(dev->parent);
	struct usb_device *usb_dev = interface_to_usbdev(intf);
	struct hid_device *hdev = container_of(dev, struct hid_device, dev);
	free(hdev->ll_driver);
	free(usb_dev);
	free(intf->cur_altsetting);
	free(intf);
	free(hdev);
	//TODO:cleanup malloc memory, move this function into DLL
}

/*---------------------------------------------------------*\
| Tests if PID is from a kraken                             |
\*---------------------------------------------------------*/
inline bool is_kraken(unsigned short pid)
{
	// Codename Unknown
	#define USB_DEVICE_ID_RAZER_KRAKEN_CLASSIC 0x0501
	// Codename Rainie
	#define USB_DEVICE_ID_RAZER_KRAKEN 0x0504
	// Codename Unknown
	#define USB_DEVICE_ID_RAZER_KRAKEN_CLASSIC_ALT 0x0506
	// Codename Kylie
	#define USB_DEVICE_ID_RAZER_KRAKEN_V2 0x0510
	
	switch (pid)
	{
	case USB_DEVICE_ID_RAZER_KRAKEN_CLASSIC:
	case USB_DEVICE_ID_RAZER_KRAKEN:
	case USB_DEVICE_ID_RAZER_KRAKEN_CLASSIC_ALT:
	case USB_DEVICE_ID_RAZER_KRAKEN_V2:
		return true;

	default:
		return false;
	}
}

/*---------------------------------------------------------*\
| Function to open a device using hidapi                    |
\*---------------------------------------------------------*/
inline void openChromaDevice(struct hid_device** hdev, unsigned int* numHdev, struct hid_driver hdr)
{
	hid_init();

	/*-----------------------------------------------------------------*\
	| Loop through all IDs in ID table of header						|
	\*-----------------------------------------------------------------*/
	for (unsigned int i = 0; hdr.id_table[i].vendor != 0; i++)
	{
		struct hid_device_info* info = hid_enumerate(hdr.id_table[i].vendor, hdr.id_table[i].product);

		/*-------------------------------------------------------------*\
		| Loop through all device information entries in set            |
		\*-------------------------------------------------------------*/
		while (info)
		{
			/*-------------------------------------------------------------*\
			| Open the device.  Regular Razer devices use usage page 01     |
			| and either usage 02 or 03.  Razer Kraken devices use usage    |
			| page 0C and usage 03											|
			\*-------------------------------------------------------------*/
			if((info->vendor_id  == hdr.id_table[i].vendor)
			&& (info->product_id == hdr.id_table[i].product)
			&& ( ((is_kraken(hdr.id_table[i].product)
				 && (info->usage_page == 0x000C)
				 && (info->usage == 0x0001)))
			 || ((info->usage_page == 0x0001)
				 && ((info->usage     == 0x0002)
			      || (info->usage     == 0x0003)))))
			{
				/*---------------------------------------------------------*\
				| Open a handle to the device                               |
				\*---------------------------------------------------------*/
				hidapi_device* dev = hid_open_path(info->path);

				if (dev)
				{
					/*---------------------------------------------------------*\
					| Print debug message indicating device is opened           |
					\*---------------------------------------------------------*/
					printf("device %04X:%04X opened!\n", hdr.id_table[i].vendor, hdr.id_table[i].product);

					/*---------------------------------------------------------*\
					| Create or resize HID device struct buffer                 |
					\*---------------------------------------------------------*/
					*hdev = (struct hid_device*)realloc(*hdev, (*numHdev + 1) * sizeof(struct hid_device));

					if (!*hdev)
					{
						printf("out of memory\n");
						continue;
					}

					/*---------------------------------------------------------*\
					| If there are hdev entries from previous loop iterations,  |
					| copy the data from the previous location to the new.      |
					\*---------------------------------------------------------*/
					if (*numHdev > 0)
					{
						for (int old_dev = 0; old_dev < *numHdev; old_dev++)
						{
							(*hdev)[old_dev].dev.parent = &((*hdev)[old_dev].dev);
							(*hdev)[old_dev].dev.parent_usb_interface->dev = &((*hdev)[old_dev].dev);
							(*hdev)[old_dev].dev.parent_usb_interface->parent_usb_device->dev = &((*hdev)[old_dev].dev);
						}
					}

					/*---------------------------------------------------------*\
					| Allocate buffer for USB interface and USB host interface  |
					| structures                                                |
					\*---------------------------------------------------------*/
					struct usb_interface* intf = (struct usb_interface*)malloc(sizeof(struct usb_interface));
					intf->cur_altsetting = (struct usb_host_interface*)malloc(sizeof(struct usb_host_interface));

					intf->cur_altsetting->desc.bInterfaceProtocol = 0;// interface_descriptor.bInterfaceProtocol;

					/*---------------------------------------------------------*\
					| Allocate buffer for USB device structure                  |
					\*---------------------------------------------------------*/
					struct usb_device* usbdevice = (struct usb_device*)malloc(sizeof(struct usb_device));

					/*---------------------------------------------------------*\
					| Set up USB device and interface structures                |
					\*---------------------------------------------------------*/
					usbdevice->descriptor.idVendor = hdr.id_table[i].vendor;
					usbdevice->descriptor.idProduct = hdr.id_table[i].product;

					intf->parent_usb_device = usbdevice;

					(*hdev)[*numHdev].product = hdr.id_table[i].product;
					(*hdev)[*numHdev].dev.parent = &((*hdev)[*numHdev].dev);
					(*hdev)[*numHdev].dev.driver_data;
					(*hdev)[*numHdev].dev.p = dev;
					(*hdev)[*numHdev].dev.parent_usb_interface = intf;
					(*hdev)[*numHdev].dev.init_name = hdr.name;
					(*hdev)[*numHdev].dev.attr_count = 0;

					usbdevice->dev = &((*hdev)[*numHdev].dev);
					intf->dev = &((*hdev)[*numHdev].dev);

					(*hdev)[*numHdev].status = 2;
					(*hdev)[*numHdev].driver = &hdr;
					(*hdev)[*numHdev].ll_driver = (struct hid_ll_driver*)malloc(sizeof(struct hid_ll_driver));
					(*hdev)[*numHdev].ll_driver->parse = ll_parse;
					(*hdev)[*numHdev].ll_driver->start = ll_start;
					(*hdev)[*numHdev].ll_driver->stop = ll_stop;

					/*---------------------------------------------------------*\
					| Call the OpenRazer driver probe function                  |
					\*---------------------------------------------------------*/
					(*hdev)[*numHdev].driver->probe(&((*hdev)[*numHdev]), &(hdr.id_table[i]));

					(*numHdev)++;
				}
			}
			info = info->next;
		}
	}
}

#endif /* HID_H_ */
