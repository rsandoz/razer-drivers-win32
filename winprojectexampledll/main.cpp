#include <set>
#include <stdio.h>
#include <tchar.h>
#include <wtypes.h>
#include <vector>

#include <linux/kernel.h>
#include <defines.h>
#include <linux/module.h>
#include <linux/hid.h>

typedef struct
{
	struct device_attribute* device_type;
	struct device_attribute* device_serial;
	struct device_attribute* firmware_version;

	struct device_attribute* matrix_custom_frame;
	struct device_attribute* matrix_brightness;

	struct device_attribute* matrix_effect_custom;
	struct device_attribute* matrix_effect_none;
	struct device_attribute* matrix_effect_static;
	struct device_attribute* matrix_effect_breath;
	struct device_attribute* matrix_effect_spectrum;
	struct device_attribute* matrix_effect_reactive;
	struct device_attribute* matrix_effect_wave;

	struct device_attribute* logo_led_brightness;
	struct device_attribute* logo_matrix_effect_none;
	struct device_attribute* logo_matrix_effect_static;
	struct device_attribute* logo_matrix_effect_breath;
	struct device_attribute* logo_matrix_effect_spectrum;
	struct device_attribute* logo_matrix_effect_reactive;

	struct device_attribute* scroll_led_brightness;
	struct device_attribute* scroll_matrix_effect_none;
	struct device_attribute* scroll_matrix_effect_static;
	struct device_attribute* scroll_matrix_effect_breath;
	struct device_attribute* scroll_matrix_effect_spectrum;
	struct device_attribute* scroll_matrix_effect_reactive;

	struct device_attribute* left_led_brightness;
	struct device_attribute* left_matrix_effect_none;
	struct device_attribute* left_matrix_effect_static;
	struct device_attribute* left_matrix_effect_breath;
	struct device_attribute* left_matrix_effect_spectrum;
	struct device_attribute* left_matrix_effect_reactive;
	struct device_attribute* left_matrix_effect_wave;

	struct device_attribute* right_led_brightness;
	struct device_attribute* right_matrix_effect_none;
	struct device_attribute* right_matrix_effect_static;
	struct device_attribute* right_matrix_effect_breath;
	struct device_attribute* right_matrix_effect_spectrum;
	struct device_attribute* right_matrix_effect_reactive;
	struct device_attribute* right_matrix_effect_wave;

	struct device_attribute* scroll_led_effect;
	struct device_attribute* scroll_led_rgb;
} device_fn_type;

typedef struct
{
	struct device_attribute* dev_attr_list[40];
} device_fn_list_type;

static const char* device_fn_names[] =
{
	"device_type",
	"device_serial",
	"firmware_version",

	"matrix_custom_frame",
	"matrix_brightness",

	"matrix_effect_custom",
	"matrix_effect_none",
	"matrix_effect_static",
	"matrix_effect_breath",
	"matrix_effect_spectrum",
	"matrix_effect_reactive",
	"matrix_effect_wave",

	"logo_led_brightness",
	"logo_matrix_effect_none",
	"logo_matrix_effect_static",
	"logo_matrix_effect_breath",
	"logo_matrix_effect_spectrum",
	"logo_matrix_effect_reactive",

	"scroll_led_brightness",
	"scroll_matrix_effect_none",
	"scroll_matrix_effect_static",
	"scroll_matrix_effect_breath",
	"scroll_matrix_effect_spectrum",
	"scroll_matrix_effect_reactive",

	"left_led_brightness",
	"left_matrix_effect_none",
	"left_matrix_effect_static",
	"left_matrix_effect_breath",
	"left_matrix_effect_spectrum",
	"left_matrix_effect_reactive",
	"left_matrix_effect_wave",

	"right_led_brightness",
	"right_matrix_effect_none",
	"right_matrix_effect_static",
	"right_matrix_effect_breath",
	"right_matrix_effect_spectrum",
	"right_matrix_effect_reactive",
	"right_matrix_effect_wave",

	"scroll_led_effect",
	"scroll_led_rgb"
};

static void load_device_fn(device_fn_type* device_fn, device* dev)
{
	memset(device_fn, 0, sizeof(device_fn_type));

	for (int table_idx = 0; table_idx < 40; table_idx++)
	{
		for (int list_idx = 0; list_idx < dev->attr_count; list_idx++)
		{
			if (strcmp(device_fn_names[table_idx], dev->attr_list[list_idx]->name) == 0)
			{
				((device_fn_list_type*)device_fn)->dev_attr_list[table_idx] = dev->attr_list[list_idx];
			}
		}
	}
}

#ifdef DLL_INTERNAL
// Hack to turn Linux device macros into API calls
#define DEVICE_ATTRH1(_device,_name, _mode, _show, _store)	\
	struct device_attribute dev1##_device##_attr_##_name; \
	DLL_INTERNAL struct device_attribute dev##_device##_attr_##_name;

#define module_hid_driverh(hdr) \
unsigned int init_##hdr## (struct hid_device** hdevo);

extern "C" {
#include "razerchromacommon.h"
#include "razerkbd_driver.h"
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTRH1(kbd, _name, _mode, _show, _store)
static DEVICE_ATTR(game_led_state,          0660, razer_attr_read_mode_game,                  razer_attr_write_mode_game);
static DEVICE_ATTR(macro_led_state,         0660, razer_attr_read_mode_macro,                 razer_attr_write_mode_macro);
static DEVICE_ATTR(macro_led_effect,        0660, razer_attr_read_mode_macro_effect,          razer_attr_write_mode_macro_effect);
static DEVICE_ATTR(logo_led_state,          0220, NULL,                                       razer_attr_write_set_logo);
static DEVICE_ATTR(profile_led_red,         0660, razer_attr_read_tartarus_profile_led_red,   razer_attr_write_tartarus_profile_led_red);
static DEVICE_ATTR(profile_led_green,       0660, razer_attr_read_tartarus_profile_led_green, razer_attr_write_tartarus_profile_led_green);
static DEVICE_ATTR(profile_led_blue,        0660, razer_attr_read_tartarus_profile_led_blue,  razer_attr_write_tartarus_profile_led_blue);


static DEVICE_ATTR(test,                    0660, razer_attr_read_test,                       razer_attr_write_test);
static DEVICE_ATTR(version,                 0440, razer_attr_read_version,                    NULL);
static DEVICE_ATTR(firmware_version,        0440, razer_attr_read_get_firmware_version,       NULL);
static DEVICE_ATTR(fn_toggle,               0220, NULL,                                       razer_attr_write_set_fn_toggle);

static DEVICE_ATTR(device_type,             0440, razer_attr_read_device_type,                NULL);
static DEVICE_ATTR(device_mode,             0660, razer_attr_read_device_mode,                razer_attr_write_device_mode);
static DEVICE_ATTR(device_serial,           0440, razer_attr_read_get_serial,                 NULL);

static DEVICE_ATTR(matrix_effect_none,      0220, NULL,                                       razer_attr_write_mode_none);
static DEVICE_ATTR(matrix_effect_wave,      0220, NULL,                                       razer_attr_write_mode_wave);
static DEVICE_ATTR(matrix_effect_spectrum,  0220, NULL,                                       razer_attr_write_mode_spectrum);
static DEVICE_ATTR(matrix_effect_reactive,  0220, NULL,                                       razer_attr_write_mode_reactive);
static DEVICE_ATTR(matrix_effect_static,    0220, NULL,                                       razer_attr_write_mode_static);
static DEVICE_ATTR(matrix_effect_starlight, 0220, NULL,                                       razer_attr_write_mode_starlight);
static DEVICE_ATTR(matrix_effect_breath,    0220, NULL,                                       razer_attr_write_mode_breath);
static DEVICE_ATTR(matrix_effect_pulsate,   0660, razer_attr_read_mode_pulsate,               razer_attr_write_mode_pulsate);
static DEVICE_ATTR(matrix_brightness,       0660, razer_attr_read_set_brightness,             razer_attr_write_set_brightness);
static DEVICE_ATTR(matrix_effect_custom,    0220, NULL,                                       razer_attr_write_mode_custom);
static DEVICE_ATTR(matrix_custom_frame,     0220, NULL,                                       razer_attr_write_matrix_custom_frame);


static DEVICE_ATTR(key_super,               0660, razer_attr_read_key_super,                  razer_attr_write_key_super);
static DEVICE_ATTR(key_alt_tab,             0660, razer_attr_read_key_alt_tab,                razer_attr_write_key_alt_tab);
static DEVICE_ATTR(key_alt_f4,              0660, razer_attr_read_key_alt_f4,                 razer_attr_write_key_alt_f4);
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTR1(kbd, _name, _mode, _show, _store)
module_hid_driverh(razer_kbd_driver);

#include "razermouse_driver.h"
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTRH1(mouse, _name, _mode, _show, _store)
static DEVICE_ATTR(version,                   0440, razer_attr_read_version,               NULL);
static DEVICE_ATTR(firmware_version,          0440, razer_attr_read_get_firmware_version,  NULL);
static DEVICE_ATTR(test,                      0220, NULL,                                  razer_attr_write_test);
static DEVICE_ATTR(poll_rate,                 0660, razer_attr_read_poll_rate,             razer_attr_write_poll_rate);
static DEVICE_ATTR(dpi,                       0660, razer_attr_read_mouse_dpi,             razer_attr_write_mouse_dpi);
                                                                                           
static DEVICE_ATTR(device_type,               0440, razer_attr_read_device_type,           NULL);
static DEVICE_ATTR(device_mode,               0660, razer_attr_read_device_mode,           razer_attr_write_device_mode);
static DEVICE_ATTR(device_serial,             0440, razer_attr_read_get_serial,            NULL);
static DEVICE_ATTR(device_idle_time,          0220, NULL,                                  razer_attr_write_set_idle_time);
                                                                                           
static DEVICE_ATTR(charge_level,              0440, razer_attr_read_get_battery,           NULL);
static DEVICE_ATTR(charge_status,             0440, razer_attr_read_is_charging,           NULL);
static DEVICE_ATTR(charge_effect,             0220, NULL,                                  razer_attr_write_set_charging_effect);
static DEVICE_ATTR(charge_colour,             0220, NULL,                                  razer_attr_write_set_charging_colour);
static DEVICE_ATTR(charge_low_threshold,      0220, NULL,                                  razer_attr_write_set_low_battery_threshold);
                                                                                           
static DEVICE_ATTR(matrix_brightness,         0660, razer_attr_read_set_brightness,        razer_attr_write_set_brightness);
static DEVICE_ATTR(matrix_custom_frame,       0220, NULL,                                  razer_attr_write_set_key_row);
static DEVICE_ATTR(matrix_effect_none,        0220, NULL,                                  razer_attr_write_mode_none);   // Matrix
static DEVICE_ATTR(matrix_effect_custom,      0220, NULL,                                  razer_attr_write_mode_custom);   // Matrix
static DEVICE_ATTR(matrix_effect_static,      0220, NULL,                                  razer_attr_write_mode_static);   // Matrix
static DEVICE_ATTR(matrix_effect_wave,        0220, NULL,                                  razer_attr_write_mode_wave);   // Matrix
static DEVICE_ATTR(matrix_effect_spectrum,    0220, NULL,                                  razer_attr_write_mode_spectrum);   // Matrix
static DEVICE_ATTR(matrix_effect_reactive,    0220, NULL,                                  razer_attr_write_mode_reactive);   // Matrix
static DEVICE_ATTR(matrix_effect_breath,      0220, NULL,                                  razer_attr_write_mode_breath);   // Matrix


static DEVICE_ATTR(scroll_led_brightness,     0660, razer_attr_read_scroll_led_brightness, razer_attr_write_scroll_led_brightness); 
// For old-school led commands
static DEVICE_ATTR(scroll_led_state,          0660, razer_attr_read_scroll_led_state,      razer_attr_write_scroll_led_state); 
static DEVICE_ATTR(scroll_led_rgb,            0660, razer_attr_read_scroll_led_rgb,        razer_attr_write_scroll_led_rgb);
static DEVICE_ATTR(scroll_led_effect,         0660, razer_attr_read_scroll_led_effect,     razer_attr_write_scroll_led_effect);
// For "extended" matrix effects
static DEVICE_ATTR(scroll_matrix_effect_spectrum,    0220, NULL,                           razer_attr_write_scroll_mode_spectrum);
static DEVICE_ATTR(scroll_matrix_effect_reactive,    0220, NULL,                           razer_attr_write_scroll_mode_reactive);
static DEVICE_ATTR(scroll_matrix_effect_breath,      0220, NULL,                           razer_attr_write_scroll_mode_breath);
static DEVICE_ATTR(scroll_matrix_effect_static,      0220, NULL,                           razer_attr_write_scroll_mode_static);
static DEVICE_ATTR(scroll_matrix_effect_none,        0220, NULL,                           razer_attr_write_scroll_mode_none);

static DEVICE_ATTR(logo_led_brightness,       0660, razer_attr_read_logo_led_brightness,   razer_attr_write_logo_led_brightness); 
// For old-school led commands
static DEVICE_ATTR(logo_led_state,            0660, razer_attr_read_logo_led_state,        razer_attr_write_logo_led_state); 
static DEVICE_ATTR(logo_led_rgb,              0660, razer_attr_read_logo_led_rgb,          razer_attr_write_logo_led_rgb);
static DEVICE_ATTR(logo_led_effect,           0660, razer_attr_read_logo_led_effect,       razer_attr_write_logo_led_effect);
// For "extended" matrix effects
static DEVICE_ATTR(logo_matrix_effect_spectrum,    0220, NULL,                             razer_attr_write_logo_mode_spectrum);
static DEVICE_ATTR(logo_matrix_effect_reactive,    0220, NULL,                             razer_attr_write_logo_mode_reactive);
static DEVICE_ATTR(logo_matrix_effect_breath,      0220, NULL,                             razer_attr_write_logo_mode_breath);
static DEVICE_ATTR(logo_matrix_effect_static,      0220, NULL,                             razer_attr_write_logo_mode_static);
static DEVICE_ATTR(logo_matrix_effect_none,        0220, NULL,                             razer_attr_write_logo_mode_none);
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTR1(mouse, _name, _mode, _show, _store)
module_hid_driverh(razer_mouse_driver);

#include "razerkraken_driver.h"
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTRH1(kraken, _name, _mode, _show, _store)
static DEVICE_ATTR(test,                    0660, razer_attr_read_test,                       razer_attr_write_test);
static DEVICE_ATTR(version,                 0440, razer_attr_read_version,                    NULL);
static DEVICE_ATTR(device_type,             0440, razer_attr_read_device_type,                NULL);
static DEVICE_ATTR(device_serial,           0440, razer_attr_read_get_serial,                 NULL);
static DEVICE_ATTR(device_mode,             0660, razer_attr_read_device_mode,                razer_attr_write_device_mode);
static DEVICE_ATTR(firmware_version,        0440, razer_attr_read_get_firmware_version,       NULL);

static DEVICE_ATTR(matrix_current_effect,	0440, razer_attr_read_matrix_current_effect,      NULL);
static DEVICE_ATTR(matrix_effect_none,      0220, NULL,                                       razer_attr_write_mode_none);
static DEVICE_ATTR(matrix_effect_spectrum,  0220, NULL,                                       razer_attr_write_mode_spectrum);
static DEVICE_ATTR(matrix_effect_static,    0660, razer_attr_read_mode_static,                razer_attr_write_mode_static);
static DEVICE_ATTR(matrix_effect_breath,    0660, razer_attr_read_mode_breath,                razer_attr_write_mode_breath);
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTR1(kraken, _name, _mode, _show, _store)
module_hid_driverh(razer_kraken_driver);

#include "razermousemat_driver.h"
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTRH1(mousemat, _name, _mode, _show, _store)
static DEVICE_ATTR(firmware_version,        0440, razer_attr_read_get_firmware_version, NULL);
static DEVICE_ATTR(device_type,             0440, razer_attr_read_device_type,          NULL);
static DEVICE_ATTR(device_serial,           0440, razer_attr_read_get_serial,           NULL);
static DEVICE_ATTR(device_mode,             0660, razer_attr_read_device_mode,          razer_attr_write_device_mode);
static DEVICE_ATTR(version,                 0440, razer_attr_read_version,              NULL);
static DEVICE_ATTR(matrix_brightness,       0664, razer_attr_read_set_brightness,       razer_attr_write_set_brightness);
static DEVICE_ATTR(matrix_effect_none,      0220, NULL,                                 razer_attr_write_mode_none);
static DEVICE_ATTR(matrix_effect_wave,      0220, NULL,                                 razer_attr_write_mode_wave);
static DEVICE_ATTR(matrix_effect_spectrum,  0220, NULL,                                 razer_attr_write_mode_spectrum);
static DEVICE_ATTR(matrix_effect_reactive,  0220, NULL,                                 razer_attr_write_mode_reactive);
static DEVICE_ATTR(matrix_effect_breath,    0220, NULL,                                 razer_attr_write_mode_breath);
static DEVICE_ATTR(matrix_effect_custom,    0220, NULL,                                 razer_attr_write_mode_custom);
static DEVICE_ATTR(matrix_effect_static,    0220, NULL,                                 razer_attr_write_mode_static);
static DEVICE_ATTR(matrix_custom_frame,     0220, NULL,                                 razer_attr_write_set_key_row);
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTR1(mousemat, _name, _mode, _show, _store)
module_hid_driverh(razer_mousemat_driver);

#include "razeraccessory_driver.h"
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTRH1(accessory, _name, _mode, _show, _store)
static DEVICE_ATTR(test,                    0660, razer_attr_read_test,                       razer_attr_write_test);
static DEVICE_ATTR(version,                 0440, razer_attr_read_version,                    NULL);
static DEVICE_ATTR(device_type,             0440, razer_attr_read_device_type,                NULL);
static DEVICE_ATTR(device_mode,             0660, razer_attr_read_device_mode,                razer_attr_write_device_mode);
static DEVICE_ATTR(device_serial,           0440, razer_attr_read_get_serial,                 NULL);
static DEVICE_ATTR(firmware_version,        0440, razer_attr_read_get_firmware_version,       NULL);

static DEVICE_ATTR(matrix_effect_none,      0220, NULL,                                       razer_attr_write_mode_none);
static DEVICE_ATTR(matrix_effect_spectrum,  0220, NULL,                                       razer_attr_write_mode_spectrum);
static DEVICE_ATTR(matrix_effect_static,    0220, NULL,                                       razer_attr_write_mode_static);
static DEVICE_ATTR(matrix_effect_breath,    0220, NULL,                                       razer_attr_write_mode_breath);
static DEVICE_ATTR(matrix_effect_custom,    0220, NULL,                                       razer_attr_write_mode_custom);
static DEVICE_ATTR(matrix_effect_wave,      0220, NULL,                                       razer_attr_write_mode_wave);
static DEVICE_ATTR(matrix_effect_blinking,  0220, NULL,                                       razer_attr_write_mode_blinking);
static DEVICE_ATTR(matrix_brightness,       0660, razer_attr_read_set_brightness,             razer_attr_write_set_brightness);
static DEVICE_ATTR(matrix_custom_frame,     0220, NULL,                                       razer_attr_write_set_key_row);

static DEVICE_ATTR(is_mug_present,          0440, razer_attr_read_get_cup_state,              NULL);
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTR1(accessory, _name, _mode, _show, _store)
module_hid_driverh(razer_accessory_driver);

#include "razercore_driver.h"
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTRH1(core, _name, _mode, _show, _store)
static DEVICE_ATTR(firmware_version,        0440, razer_attr_read_get_firmware_version, NULL);
static DEVICE_ATTR(device_type,             0440, razer_attr_read_device_type,          NULL);
static DEVICE_ATTR(device_serial,           0440, razer_attr_read_get_serial,           NULL);
static DEVICE_ATTR(device_mode,             0660, razer_attr_read_device_mode,          razer_attr_write_device_mode);
static DEVICE_ATTR(version,                 0440, razer_attr_read_version,              NULL);
static DEVICE_ATTR(matrix_brightness,       0664, razer_attr_read_set_brightness,       razer_attr_write_set_brightness);
static DEVICE_ATTR(matrix_effect_none,      0220, NULL,                                 razer_attr_write_mode_none);
static DEVICE_ATTR(matrix_effect_wave,      0220, NULL,                                 razer_attr_write_mode_wave);
static DEVICE_ATTR(matrix_effect_spectrum,  0220, NULL,                                 razer_attr_write_mode_spectrum);
static DEVICE_ATTR(matrix_effect_reactive,  0220, NULL,                                 razer_attr_write_mode_reactive);
static DEVICE_ATTR(matrix_effect_breath,    0220, NULL,                                 razer_attr_write_mode_breath);
static DEVICE_ATTR(matrix_effect_custom,    0220, NULL,                                 razer_attr_write_mode_custom);
static DEVICE_ATTR(matrix_effect_static,    0220, NULL,                                 razer_attr_write_mode_static);
static DEVICE_ATTR(matrix_custom_frame,     0220, NULL,                                 razer_attr_write_set_key_row);
#undef DEVICE_ATTR
#define DEVICE_ATTR(_name, _mode, _show, _store) DEVICE_ATTR1(core, _name, _mode, _show, _store)
module_hid_driverh(razer_core_driver);

}
#else

#ifdef _WIN64
#define OPENRAZERDLL        _T("OpenRazer64.dll")
#elif WIN32
#define OPENRAZERDLL        _T("OpenRazer.dll")
#endif
#endif


static const COLORREF testColor[] = {
		RGB(0x00,0x00,0xFF)
	  , RGB(0x00,0xFF,0x00)
	  , RGB(0xFF,0x00,0x00)
	  , RGB(0xFF,0xFF,0x00)
	  , RGB(0xFF,0x00,0xFF)
	  , RGB(0x00,0xFF,0xFF)};

static const char* testReactive[] = {
		"\x00\xFF\x00\x00"
	,	"\x20\xFF\xFF\x00"
	,	"\x30\x00\xFF\x00"};

static const char* testBrightness[] = {
		"0"
	  , "31"
	  , "63"
	  , "95"
	  , "127"
	  , "159"
	  , "191"
	  , "223"
	  , "255"};

static unsigned char red(COLORREF color) {
	return (char)(color & 0x0000FF);
}

static unsigned char green(COLORREF color) {
	return (char)((color & 0x00FF00) >> 8);
}

static unsigned char blue(COLORREF color) {
	return (char)((color & 0xFF0000) >> 16);
}

static	std::set<struct device*> deviceFireflies;
int main(int argc, char** argv) {
#ifndef DLL_INTERNAL
	printf("Press enter to load DLL...");
	getc(stdin);
	printf("\n");

	HMODULE chromaLinuxModule = LoadLibrary(OPENRAZERDLL);
	if (chromaLinuxModule == nullptr)
		return 0;

	// map DLL calls
	typedef unsigned int(*INITRAZERDRIVER)(struct hid_device** hdev);

	INITRAZERDRIVER init_razer_kbd_driver		= reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_kbd_driver"));
	INITRAZERDRIVER init_razer_mousemat_driver	= reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_mousemat_driver"));
	INITRAZERDRIVER init_razer_mouse_driver		= reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_mouse_driver"));
	INITRAZERDRIVER init_razer_accessory_driver = reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_accessory_driver"));
	INITRAZERDRIVER init_razer_kraken_driver	= reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_kraken_driver"));
	INITRAZERDRIVER init_razer_core_driver		= reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_core_driver"));
#endif

	printf("Press enter to init usb and devices...");
	getc(stdin);
	printf("\n");

	struct hid_device* hdev;
	std::vector<device*> devs;
	std::vector<device_fn_type> devices;
	unsigned int num;

	hdev = NULL;
	num = init_razer_mousemat_driver(&hdev);
	for (unsigned int i = 0; i < num; i++)
	{
		device_fn_type new_dev;
		load_device_fn(&new_dev, &hdev[i].dev);
		devs.push_back(&hdev[i].dev);
		devices.push_back(new_dev);
	}

	hdev = NULL;
	num = init_razer_kbd_driver(&hdev);
	for (unsigned int i = 0; i < num; i++)
	{
		device_fn_type new_dev;
		load_device_fn(&new_dev, &hdev[i].dev);
		devs.push_back(&hdev[i].dev);
		devices.push_back(new_dev);
	}

	hdev = NULL;
	num = init_razer_mouse_driver(&hdev);
	for (unsigned int i = 0; i < num; i++)
	{
		device_fn_type new_dev;
		load_device_fn(&new_dev, &hdev[i].dev);
		devs.push_back(&hdev[i].dev);
		devices.push_back(new_dev);
	}

	hdev = NULL;
	num = init_razer_accessory_driver(&hdev);
	for (unsigned int i = 0; i < num; i++)
	{
		device_fn_type new_dev;
		load_device_fn(&new_dev, &hdev[i].dev);
		devs.push_back(&hdev[i].dev);
		devices.push_back(new_dev);
	}

	hdev = NULL;
	num = init_razer_core_driver(&hdev);
	for (unsigned int i = 0; i < num; i++)
	{
		device_fn_type new_dev;
		load_device_fn(&new_dev, &hdev[i].dev);
		devs.push_back(&hdev[i].dev);
		devices.push_back(new_dev);
	}

	hdev = NULL;
	num = init_razer_kraken_driver(&hdev);
	for (unsigned int i = 0; i < num; i++)
	{
		device_fn_type new_dev;
		load_device_fn(&new_dev, &hdev[i].dev);
		devs.push_back(&hdev[i].dev);
		devices.push_back(new_dev);
	}

	printf("Press enter to start...");
	getc(stdin);
	printf("\n");

	//Start by setting static white
	for (int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
	{
		if (devices[dev_idx].matrix_effect_static)
		{
			const char cmd[3] = { 0xFF, 0xFF, 0xFF };

			devices[dev_idx].matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
		}

		if (devices[dev_idx].logo_matrix_effect_static)
		{
			const char cmd[3] = { 0xFF, 0xFF, 0xFF };

			devices[dev_idx].logo_matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
		}

		if (devices[dev_idx].scroll_matrix_effect_static)
		{
			const char cmd[3] = { 0xFF, 0xFF, 0xFF };

			devices[dev_idx].scroll_matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
		}

		if (devices[dev_idx].left_matrix_effect_static)
		{
			const char cmd[3] = { 0xFF, 0xFF, 0xFF };

			devices[dev_idx].left_matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
		}

		if (devices[dev_idx].right_matrix_effect_static)
		{
			const char cmd[3] = { 0xFF, 0xFF, 0xFF };

			devices[dev_idx].right_matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
		}
	}

	//Test brightness
	for (int i = 0; i < _countof(testBrightness); i++)
	{
		printf("Press enter to test brightness level %s ...", testBrightness[i]);
		getc(stdin);
		printf("\n");

		for (int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
		{
			if (devices[dev_idx].matrix_brightness)
			{
				devices[dev_idx].matrix_brightness->store(devs[dev_idx], NULL, testBrightness[i], strlen(testBrightness[i]) - 1);
			}

			if (devices[dev_idx].logo_led_brightness)
			{
				devices[dev_idx].logo_led_brightness->store(devs[dev_idx], NULL, testBrightness[i], strlen(testBrightness[i]) - 1);
			}

			if (devices[dev_idx].scroll_led_brightness)
			{
				devices[dev_idx].scroll_led_brightness->store(devs[dev_idx], NULL, testBrightness[i], strlen(testBrightness[i]) - 1);
			}

			if (devices[dev_idx].left_led_brightness)
			{
				devices[dev_idx].left_led_brightness->store(devs[dev_idx], NULL, testBrightness[i], strlen(testBrightness[i]) - 1);
			}

			if (devices[dev_idx].right_led_brightness)
			{
				devices[dev_idx].right_led_brightness->store(devs[dev_idx], NULL, testBrightness[i], strlen(testBrightness[i]) - 1);
			}
		}
	}

	//Test none (off)
	printf("Press enter to test none (turn everything off)...");
	getc(stdin);
	printf("\n");

	for (int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
	{
		if (devices[dev_idx].matrix_effect_none)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].matrix_effect_none->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].logo_matrix_effect_none)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].logo_matrix_effect_none->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].scroll_matrix_effect_none)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].scroll_matrix_effect_none->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].left_matrix_effect_none)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].left_matrix_effect_none->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].right_matrix_effect_none)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].right_matrix_effect_none->store(devs[dev_idx], NULL, cmd, 1);
		}
	}

	//Test spectrum
	printf("Press enter to test spectrum effects...");
	getc(stdin);
	printf("\n");

	for (int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
	{
		if (devices[dev_idx].matrix_effect_spectrum)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].matrix_effect_spectrum->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].logo_matrix_effect_spectrum)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].logo_matrix_effect_spectrum->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].scroll_matrix_effect_spectrum)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].scroll_matrix_effect_spectrum->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].left_matrix_effect_spectrum)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].left_matrix_effect_spectrum->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].right_matrix_effect_spectrum)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].right_matrix_effect_spectrum->store(devs[dev_idx], NULL, cmd, 1);
		}
	}

	//Test reactive
	printf("Press enter to test reactive effects...");
	getc(stdin);
	printf("\n");

	for (int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
	{
		if (devices[dev_idx].matrix_effect_reactive)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].matrix_effect_reactive->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].logo_matrix_effect_reactive)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].logo_matrix_effect_reactive->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].scroll_matrix_effect_reactive)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].scroll_matrix_effect_reactive->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].left_matrix_effect_reactive)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].left_matrix_effect_reactive->store(devs[dev_idx], NULL, cmd, 1);
		}

		if (devices[dev_idx].right_matrix_effect_reactive)
		{
			const char cmd[1] = { 0x00 };

			devices[dev_idx].right_matrix_effect_reactive->store(devs[dev_idx], NULL, cmd, 1);
		}
	}

	//Test static
	printf("Press enter to test static effects (loop through 6 colors)...");
	getc(stdin);
	printf("\n");

	for (int i = 0; i < _countof(testColor); i++)
	{
		for (int dev_idx = 0; dev_idx < devices.size(); dev_idx++)
		{
			if (devices[dev_idx].matrix_effect_static)
			{
				const char cmd[3] = { red(testColor[i]), green(testColor[i]), blue(testColor[i]) };

				devices[dev_idx].matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
			}

			if (devices[dev_idx].logo_matrix_effect_static)
			{
				const char cmd[3] = { red(testColor[i]), green(testColor[i]), blue(testColor[i]) };

				devices[dev_idx].logo_matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
			}

			if (devices[dev_idx].scroll_matrix_effect_static)
			{
				const char cmd[3] = { red(testColor[i]), green(testColor[i]), blue(testColor[i]) };

				devices[dev_idx].scroll_matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
			}

			if (devices[dev_idx].left_matrix_effect_static)
			{
				const char cmd[3] = { red(testColor[i]), green(testColor[i]), blue(testColor[i]) };

				devices[dev_idx].left_matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
			}

			if (devices[dev_idx].right_matrix_effect_static)
			{
				const char cmd[3] = { red(testColor[i]), green(testColor[i]), blue(testColor[i]) };

				devices[dev_idx].right_matrix_effect_static->store(devs[dev_idx], NULL, cmd, 3);
			}
		}

		printf("Color RGB(%02X,%02X,%02X) sent.  Press enter to test next color...",red(testColor[i]), green(testColor[i]), blue(testColor[i]));
		getc(stdin);
		printf("\n");
	}

	//Test custom
	printf("Press enter to test custom effects...");
	getc(stdin);
	printf("\n");

	for(int i = 0;i < 10;i++)
	{
		for (unsigned int offset = 0; offset < _countof(testColor); offset++)
		{

		}
	}

	printf("Press enter to exit...");
	getc(stdin);
	printf("\n");

	return 0;
}
