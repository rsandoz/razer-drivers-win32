/*---------------------------------------------------------*\
| OpenRazer-Win32 Demo Application                          |
|                                                           |
| This application uses the OpenRazer-Win32 driver to test  |
| all modes of Razer devices.                               |
|                                                           |
| Original code:                                            |
|       https://github.com/rsandoz/razer-drivers-win32      |
|                                                           |
| Updated by Adam Honse (CalcProgrammer1), 4/21/2020        |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <tchar.h>
#include <wtypes.h>
#include <vector>

#include <linux/kernel.h>
#include <defines.h>
#include <linux/module.h>
#include <linux/hid.h>

/*---------------------------------------------------------*\
| This section selects between the 32 and 64 bit DLLs       |
\*---------------------------------------------------------*/
#ifdef _WIN64
#define OPENRAZERDLL        _T("OpenRazer64.dll")
#elif WIN32
#define OPENRAZERDLL        _T("OpenRazer.dll")
#endif

/*---------------------------------------------------------*\
| This structure holds all possible device attributes that  |
| are used in the demo application.  Device attributes are  |
| equivalent to sysfs entries on Linux.                     |
\*---------------------------------------------------------*/
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

    struct device_attribute* logo_led_effect;
    struct device_attribute* logo_led_rgb;
    struct device_attribute* logo_led_state;

    struct device_attribute* scroll_led_effect;
    struct device_attribute* scroll_led_rgb;
    struct device_attribute* scroll_led_state;
} device_fn_type;

typedef struct
{
    struct device_attribute* dev_attr_list[44];
} device_fn_list_type;

/*---------------------------------------------------------*\
| This is a table of device attribute names.  It should     |
| always match the order of the entries in the structure    |
\*---------------------------------------------------------*/
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

    "logo_led_effect",
    "logo_led_rgb",
    "logo_led_state",

    "scroll_led_effect",
    "scroll_led_rgb",
    "scroll_led_state"
};

/*---------------------------------------------------------*\
| This function searches the device attribute list of a     |
| given device to fill in a device_fn_type structure        |
\*---------------------------------------------------------*/
static void load_device_fn(device_fn_type* device_fn, device* dev)
{
    memset(device_fn, 0, sizeof(device_fn_type));

    for (int table_idx = 0; table_idx < 44; table_idx++)
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

/*---------------------------------------------------------*\
| Tables of test data                                       |
\*---------------------------------------------------------*/
static const COLORREF testColor[] =
{
        RGB(0x00,0x00,0xFF)
      , RGB(0x00,0xFF,0x00)
      , RGB(0xFF,0x00,0x00)
      , RGB(0xFF,0xFF,0x00)
      , RGB(0xFF,0x00,0xFF)
      , RGB(0x00,0xFF,0xFF)
};

static const char* testReactive[] =
{
        "\x00\xFF\x00\x00"
    ,	"\x20\xFF\xFF\x00"
    ,	"\x30\x00\xFF\x00"
};

static const char* testBrightness[] =
{
        "0"
      , "31"
      , "63"
      , "95"
      , "127"
      , "159"
      , "191"
      , "223"
      , "255"};

/*---------------------------------------------------------*\
| Functions to extract color components from COLORREF       |
\*---------------------------------------------------------*/
static unsigned char red(COLORREF color)
{
    return (char)(color & 0x0000FF);
}

static unsigned char green(COLORREF color)
{
    return (char)((color & 0x00FF00) >> 8);
}

static unsigned char blue(COLORREF color)
{
    return (char)((color & 0xFF0000) >> 16);
}

/*---------------------------------------------------------*\
| Main function of demo application                         |
\*---------------------------------------------------------*/
int main(int argc, char** argv)
{
    printf("Press enter to load DLL...");
    getc(stdin);
    printf("\n");

    HMODULE chromaLinuxModule = LoadLibrary(OPENRAZERDLL);
    if (chromaLinuxModule == nullptr)
        return 0;

    /*---------------------------------------------------------*\
    | Map DLL functions                                         |
    \*---------------------------------------------------------*/
    typedef unsigned int(*INITRAZERDRIVER)(struct hid_device** hdev);

    INITRAZERDRIVER init_razer_kbd_driver		= reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_kbd_driver"));
    INITRAZERDRIVER init_razer_mouse_driver		= reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_mouse_driver"));
    INITRAZERDRIVER init_razer_accessory_driver = reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_accessory_driver"));
    INITRAZERDRIVER init_razer_kraken_driver	= reinterpret_cast<INITRAZERDRIVER>(GetProcAddress(chromaLinuxModule, "init_razer_kraken_driver"));

    /*---------------------------------------------------------*\
    | Initialize all OpenRazer driver modules and store devices |
    \*---------------------------------------------------------*/
    printf("Press enter to init usb and devices...");
    getc(stdin);
    printf("\n");

    struct hid_device* hdev;
    std::vector<device*> devs;
    std::vector<device_fn_type> devices;
    unsigned int num;

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
    num = init_razer_kraken_driver(&hdev);
    for (unsigned int i = 0; i < num; i++)
    {
        device_fn_type new_dev;
        load_device_fn(&new_dev, &hdev[i].dev);
        devs.push_back(&hdev[i].dev);
        devices.push_back(new_dev);
    }

    /*---------------------------------------------------------*\
    | Start by setting static white                             |
    \*---------------------------------------------------------*/
    printf("Press enter to start...");
    getc(stdin);
    printf("\n");

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

        if (devices[dev_idx].logo_led_state)
        {
            const char state[1] = { '1' };

            devices[dev_idx].logo_led_state->store(devs[dev_idx], NULL, state, 1);
        }

        if (devices[dev_idx].scroll_led_state)
        {
            const char state[1] = { '1' };

            devices[dev_idx].scroll_led_state->store(devs[dev_idx], NULL, state, 1);
        }

        if (devices[dev_idx].logo_led_rgb && devices[dev_idx].logo_led_effect)
        {
            const char cmd[3] = { 0xFF, 0xFF, 0xFF };

            devices[dev_idx].logo_led_rgb->store(devs[dev_idx], NULL, cmd, 3);

            const char effect[1] = { '0' };

            devices[dev_idx].logo_led_effect->store(devs[dev_idx], NULL, effect, 1);
        }

        if (devices[dev_idx].scroll_led_rgb && devices[dev_idx].scroll_led_effect)
        {
            const char cmd[3] = { 0xFF, 0xFF, 0xFF };

            devices[dev_idx].scroll_led_rgb->store(devs[dev_idx], NULL, cmd, 3);

            const char effect[1] = { '0' };

            devices[dev_idx].scroll_led_effect->store(devs[dev_idx], NULL, effect, 1);
        }
    }

    /*---------------------------------------------------------*\
    | Test brightness                                           |
    \*---------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Test none (off)                                           |
    \*---------------------------------------------------------*/
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

        if (devices[dev_idx].logo_led_state)
        {
            const char state[1] = { '0' };

            devices[dev_idx].logo_led_state->store(devs[dev_idx], NULL, state, 1);
        }

        if (devices[dev_idx].scroll_led_state)
        {
            const char state[1] = { '0' };

            devices[dev_idx].scroll_led_state->store(devs[dev_idx], NULL, state, 1);
        }
    }

    /*---------------------------------------------------------*\
    | Test spectrum                                             |
    \*---------------------------------------------------------*/
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

        if (devices[dev_idx].logo_led_state && devices[dev_idx].logo_led_effect)
        {
            const char state[1] = { '1' };

            devices[dev_idx].logo_led_state->store(devs[dev_idx], NULL, state, 1);

            const char effect[1] = { '4' };

            devices[dev_idx].logo_led_effect->store(devs[dev_idx], NULL, effect, 1);
        }

        if (devices[dev_idx].scroll_led_state && devices[dev_idx].scroll_led_effect)
        {
            const char state[1] = { '1' };

            devices[dev_idx].scroll_led_state->store(devs[dev_idx], NULL, state, 1);

            const char effect[1] = { '4' };

            devices[dev_idx].scroll_led_effect->store(devs[dev_idx], NULL, effect, 1);
        }
    }

    /*---------------------------------------------------------*\
    | Test reactive                                             |
    \*---------------------------------------------------------*/
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

    /*---------------------------------------------------------*\
    | Test static                                               |
    \*---------------------------------------------------------*/
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

            if (devices[dev_idx].logo_led_rgb && devices[dev_idx].logo_led_effect)
            {
                const char effect[1] = { '0' };

                devices[dev_idx].logo_led_effect->store(devs[dev_idx], NULL, effect, 1);

                const char cmd[3] = { red(testColor[i]), green(testColor[i]), blue(testColor[i]) };

                devices[dev_idx].logo_led_rgb->store(devs[dev_idx], NULL, cmd, 3);
            }

            if (devices[dev_idx].scroll_led_rgb && devices[dev_idx].scroll_led_effect)
            {
                const char effect[1] = { '0' };

                devices[dev_idx].scroll_led_effect->store(devs[dev_idx], NULL, effect, 1);

                const char cmd[3] = { red(testColor[i]), green(testColor[i]), blue(testColor[i]) };

                devices[dev_idx].scroll_led_rgb->store(devs[dev_idx], NULL, cmd, 3);
            }
        }

        printf("Color RGB(%02X,%02X,%02X) sent.  Press enter to test next color...",red(testColor[i]), green(testColor[i]), blue(testColor[i]));
        getc(stdin);
        printf("\n");
    }

    /*---------------------------------------------------------*\
    | Test custom                                               |
    \*---------------------------------------------------------*/
    printf("Press enter to test custom effects...");
    getc(stdin);
    printf("\n");

    for(int i = 0;i < 10;i++)
    {
        for (unsigned int offset = 0; offset < _countof(testColor); offset++)
        {

        }
    }

    /*---------------------------------------------------------*\
    | Test is complete, time to exit                            |
    \*---------------------------------------------------------*/
    printf("Press enter to exit...");
    getc(stdin);
    printf("\n");

    return 0;
}
