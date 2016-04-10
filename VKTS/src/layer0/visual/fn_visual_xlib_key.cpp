/**
 * VKTS - VulKan ToolS.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) since 2014 Norbert Nopper
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <vkts/vkts.hpp>

#include <X11/XKBlib.h>
#include <X11/Xutil.h>

namespace vkts
{

typedef struct _NameKey
{
    char name[256];
    int32_t key;
} NameKey;

static NameKey g_nameKey[] =
{
    { "SPCE", VKTS_KEY_SPACE },
    { "AC11", VKTS_KEY_APOSTROPHE },
    { "AB08", VKTS_KEY_COMMA },
    { "AE11", VKTS_KEY_MINUS },
    { "AB09", VKTS_KEY_PERIOD },
    { "AB10", VKTS_KEY_SLASH },
    { "AE10", VKTS_KEY_0 },
    { "AE01", VKTS_KEY_1 },
    { "AE02", VKTS_KEY_2 },
    { "AE03", VKTS_KEY_3 },
    { "AE04", VKTS_KEY_4 },
    { "AE05", VKTS_KEY_5 },
    { "AE06", VKTS_KEY_6 },
    { "AE07", VKTS_KEY_7 },
    { "AE08", VKTS_KEY_8 },
    { "AE09", VKTS_KEY_9 },

    { "AC10", VKTS_KEY_SEMICOLON },
    { "LSGT", VKTS_KEY_LESS },
    { "AE12", VKTS_KEY_EQUALS },

    { "AD11", VKTS_KEY_LEFT_BRACKET },
    { "BKSL", VKTS_KEY_BACKSLASH },
    { "AD12", VKTS_KEY_RIGHT_BRACKET },

    { "TLDE", VKTS_KEY_GRAVE },
    { "AC01", VKTS_KEY_A },
    { "AB05", VKTS_KEY_B },
    { "AB03", VKTS_KEY_C },
    { "AC03", VKTS_KEY_D },
    { "AD03", VKTS_KEY_E },
    { "AC04", VKTS_KEY_F },
    { "AC05", VKTS_KEY_G },
    { "AC06", VKTS_KEY_H },
    { "AD08", VKTS_KEY_I },
    { "AC07", VKTS_KEY_J },
    { "AC08", VKTS_KEY_K },
    { "AC09", VKTS_KEY_L },
    { "AB07", VKTS_KEY_M },
    { "AB06", VKTS_KEY_N },
    { "AD09", VKTS_KEY_O },
    { "AD10", VKTS_KEY_P },
    { "AD01", VKTS_KEY_Q },
    { "AD04", VKTS_KEY_R },
    { "AC02", VKTS_KEY_S },
    { "AD05", VKTS_KEY_T },
    { "AD07", VKTS_KEY_U },
    { "AB04", VKTS_KEY_V },
    { "AD02", VKTS_KEY_W },
    { "AB02", VKTS_KEY_X },
    { "AD06", VKTS_KEY_Y },
    { "AB01", VKTS_KEY_Z },

    { "ESC", VKTS_KEY_ESCAPE },

    { "FK01", VKTS_KEY_F1 },
    { "FK02", VKTS_KEY_F2 },
    { "FK03", VKTS_KEY_F3 },
    { "FK04", VKTS_KEY_F4 },
    { "FK05", VKTS_KEY_F5 },
    { "FK06", VKTS_KEY_F6 },
    { "FK07", VKTS_KEY_F7 },
    { "FK08", VKTS_KEY_F8 },
    { "FK09", VKTS_KEY_F9 },
    { "FK10", VKTS_KEY_F10 },
    { "FK11", VKTS_KEY_F11 },
    { "FK12", VKTS_KEY_F12 },

    { "BKSP", VKTS_KEY_BACKSPACE },
    { "TAB", VKTS_KEY_TAB },
    { "RTRN", VKTS_KEY_ENTER },
    { "CAPS", VKTS_KEY_CAPS_LOCK },
    { "LFSH", VKTS_KEY_LEFT_SHIFT },
    { "RTSH", VKTS_KEY_RIGHT_SHIFT },
    { "LCTL", VKTS_KEY_LEFT_CONTROL },

    { "LALT", VKTS_KEY_LEFT_ALT },
    { "RALT", VKTS_KEY_RIGHT_ALT },

    { "COMP", VKTS_KEY_MENU },
    { "RCTL", VKTS_KEY_RIGHT_CONTROL },

    { "SCLK", VKTS_KEY_SCROLL_LOCK },
    { "PAUS", VKTS_KEY_PAUSE },

    { "INS", VKTS_KEY_INSERT },
    { "DELE", VKTS_KEY_DELETE },
    { "HOME", VKTS_KEY_HOME },
    { "END", VKTS_KEY_END },
    { "PGUP", VKTS_KEY_PAGE_UP },
    { "PGDN", VKTS_KEY_PAGE_DOWN },

    { "LEFT", VKTS_KEY_LEFT },
    { "RGHT", VKTS_KEY_RIGHT },
    { "UP", VKTS_KEY_UP },
    { "DOWN", VKTS_KEY_DOWN },

    { "NMLK", VKTS_KEY_KP_NUM_LOCK },
    { "KPDV", VKTS_KEY_KP_DIVIDE },
    { "KPMU", VKTS_KEY_KP_MULTIPLY },
    { "KPSU", VKTS_KEY_KP_SUBTRACT },
    { "KPAD", VKTS_KEY_KP_ADD },
    { "KPEN", VKTS_KEY_KP_ENTER },
    { "KPDL", VKTS_KEY_KP_DECIMAL },

    { "KP0", VKTS_KEY_KP_0 },
    { "KP1", VKTS_KEY_KP_1 },
    { "KP2", VKTS_KEY_KP_2 },
    { "KP3", VKTS_KEY_KP_3 },
    { "KP4", VKTS_KEY_KP_4 },
    { "KP5", VKTS_KEY_KP_5 },
    { "KP6", VKTS_KEY_KP_6 },
    { "KP7", VKTS_KEY_KP_7 },
    { "KP8", VKTS_KEY_KP_8 },
    { "KP9", VKTS_KEY_KP_9 }
};

static int32_t g_keys[512];

int32_t _visualMapKey(const char* name)
{
    if (!name)
    {
        return VKTS_KEY_UNKNOWN;
    }

    VkBool32 search = VK_TRUE;

    int32_t i = 0;

    while (search)
    {
        if (strcmp(name, g_nameKey[i].name) == 0)
        {
            return g_nameKey[i].key;
        }
        else if (strcmp("KP9", g_nameKey[i].name) == 0)
        {
            search = VK_FALSE;
        }

        i++;
    }

    return VKTS_KEY_UNKNOWN;
}

VkBool32 VKTS_APIENTRY _visualInitKey(Display* display)
{
    memset(g_keys, VKTS_KEY_UNKNOWN, sizeof(g_keys));

    XkbDescPtr desc = XkbGetKeyboard(display, XkbAllComponentsMask, XkbUseCoreKbd);

    if (!desc)
    {
        return VK_FALSE;
    }

    char name[XkbKeyNameLength + 1];
    name[XkbKeyNameLength] = '\0';

    for (int32_t keyCode = desc->min_key_code; keyCode <= desc->max_key_code; keyCode++)
    {
        if (keyCode < 0 || keyCode >= 512)
        {
            continue;
        }

        memcpy(name, desc->names->keys[keyCode].name, XkbKeyNameLength);

        g_keys[keyCode] = _visualMapKey(name);
    }

    XkbFreeKeyboard(desc, 0, True);

    return VK_TRUE;
}

int32_t VKTS_APIENTRY _visualTranslateKey(int32_t keyCode)
{
    if (keyCode < 0 || keyCode >= 512)
    {
        return VKTS_KEY_UNKNOWN;
    }

    return g_keys[keyCode];
}

}
