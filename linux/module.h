#ifndef MODULE_H_
#define MODULE_H_

#include <stdio.h>
#include <hidapi/hidapi.h>

#define MODULE_AUTHOR( __Declaration__ )
#define MODULE_DESCRIPTION( __Declaration__ )
#define MODULE_VERSION( __Declaration__ )
#define MODULE_LICENSE( __Declaration__ )

#define USB_CTRL_SET_TIMEOUT    5000

#define USB_DIR_OUT                     0
#define USB_DIR_IN                      0x80

#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_RECIP_INTERFACE	0x01

#define usb_sndctrlpipe(u,d) 0
#define usb_rcvctrlpipe(u,d) 0

#define PATH_MAX 512

struct usb_interface_descriptor {
    unsigned char  bInterfaceProtocol;
	unsigned char  bInterfaceSubClass;
};

struct usb_host_interface {
	struct usb_interface_descriptor desc;
};

struct device {
	struct device					*parent;
	void							*p;
	const char						*init_name;
	void							*driver_data;
	unsigned int                    attr_count;
	struct device_attribute	*       attr_list[64];
struct usb_interface *parent_usb_interface;
};

struct usb_interface {
	struct device* dev;
    int num_altsetting;
	struct usb_host_interface *cur_altsetting;
struct usb_device *parent_usb_device;
};

struct usb_device_descriptor {
    unsigned short idVendor;
    unsigned short idProduct;
};

struct usb_device {
	struct device* dev;
    struct usb_device_descriptor descriptor;
};

/*---------------------------------------------------------*\
| Implementation of usb_control_msg using hidapi			|
\*---------------------------------------------------------*/
inline int usb_control_msg
	(
	struct usb_device *usb_dev,
	int				   usb_pipe,
	unsigned int	   request,
	unsigned int       request_type,
	unsigned int       value,
	unsigned int       report_index,
	unsigned char*     buf,
	unsigned int       size,
	unsigned int       timeout
	)
{
	/*---------------------------------------------------------*\
	| Kraken uses hid_write                                     |
	\*---------------------------------------------------------*/
	if(size == 37)
	{
		if ((request_type & USB_DIR_IN) == USB_DIR_IN)
		{
			return(hid_read((hidapi_device*)usb_dev->dev->p, buf, size));
		}
		else
		{
			return(hid_write((hidapi_device*)usb_dev->dev->p, buf, size));
		}
	}
	/*---------------------------------------------------------*\
	| Check request type to determine if we're reading or		|
	| writing the feature report								|
	\*---------------------------------------------------------*/
	else if ((request_type & USB_DIR_IN) == USB_DIR_IN)
	{
		/*---------------------------------------------------------*\
		| Create a buffer to receive report with index              |
		\*---------------------------------------------------------*/
		int			  cbRecvd = 0;
		unsigned char pkt[91];

		/*---------------------------------------------------------*\
		| Set the report index										|
		\*---------------------------------------------------------*/
		pkt[0] = report_index;

		/*---------------------------------------------------------*\
		| Get the feature report.  Add one to the size to account   |
		| for the report index										|
		\*---------------------------------------------------------*/
		cbRecvd = hid_get_feature_report((hidapi_device*)usb_dev->dev->p, pkt, size + 1);

		/*---------------------------------------------------------*\
		| For some reason, cbRecvd is sometimes 1 greater than size |
		| + 1.  Limit the return value.                             |
		\*---------------------------------------------------------*/
		if(cbRecvd > size + 1)
		{
			cbRecvd = size + 1;
		}

		/*---------------------------------------------------------*\
		| Copy the received report into the buffer                  |
		\*---------------------------------------------------------*/
		memcpy(buf, &pkt[1], size);

		/*---------------------------------------------------------*\
		| Return the number of bytes received, not including the    |
		| report index												|
		\*---------------------------------------------------------*/
		return(cbRecvd - 1);
	}
	else
	{
		/*---------------------------------------------------------*\
		| Create a buffer to send report with index					|
		\*---------------------------------------------------------*/
		int			  cbSent = 0;
		unsigned char pkt[91];

		/*---------------------------------------------------------*\
		| Set the report index and copy the report into the buffer  |
		\*---------------------------------------------------------*/
		pkt[0] = report_index;
		memcpy(&pkt[1], buf, size);

		/*---------------------------------------------------------*\
		| Send the feature report.  Add one to the size to account  |
		| for the report index										|
		\*---------------------------------------------------------*/
		cbSent = hid_send_feature_report((hidapi_device*)usb_dev->dev->p, pkt, size + 1);

		/*---------------------------------------------------------*\
		| Return the number of bytes sent, not including the		|
		| report index												|
		\*---------------------------------------------------------*/
		return cbSent - 1;
	}
}

inline struct usb_interface *to_usb_interface(struct device *dev) {
	return dev->parent_usb_interface;
}

inline struct usb_device *to_usb_device(struct device *dev) {
	return dev->parent_usb_interface->parent_usb_device;
}

inline struct usb_device *interface_to_usbdev(struct usb_interface *intf) {
	return to_usb_device(intf->dev->parent);
}

inline void usb_disable_autosuspend(struct usb_device *usb_dev) {
	printf("usb_disable_autosuspend\n");
}

struct device_attribute {
	const char* name;
	ssize_t(*show)(struct device *dev, struct device_attribute *attr, char *buf);
	ssize_t(*store)(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
};

inline int device_create_file(struct device *device, struct device_attribute *entry)
{
	if (device->attr_count < 64)
	{
		printf("device_create_file - Adding %s to list\n", entry->name);
		device->attr_list[device->attr_count] = entry;
		device->attr_count++;
	}
	else
	{
		printf("device_create_file - List is full\n");
	}
	return 0;
}

inline void device_remove_file(struct device *device, struct device_attribute *entry) {
	printf("device_remove_file %s\n", entry->name);
}

#define HID_USB_DEVICE(ven, prod) \
	  .vendor = (ven) \
	, .product = (prod)


#define __stringify(x)       #x

// Hack to turn Linux device macros into API calls
#define DEVICE_ATTR1(_device,_name, _mode, _show, _store)	\
	struct device_attribute dev_attr_##_name = { \
		  .name = __stringify(_name)                \
        , .show   = _show                           \
        , .store  = _store                          \
    };                                              \
	DLL_INTERNAL struct device_attribute dev##_device##_attr_##_name = {	\
          .name = __stringify(_name)				\
        , .show   = _show							\
        , .store  = _store							\
	};

#define MODULE_DEVICE_TABLE(type, name)

/*typedef struct hid_device_array_tag {
	unsigned int count;
	struct hid_device* hdev[];
} hid_device_array;*/

#define module_hid_driver(hdr) \
DLL_INTERNAL unsigned int init_##hdr## (struct hid_device** hdevo) {	\
	unsigned int numHdevs = 0; \
	struct hid_device* hdev = NULL; \
	openChromaDevice(&hdev, &numHdevs, hdr); \
	*hdevo = hdev; \
	return numHdevs; \
}

#endif /* MODULE_H_ */
