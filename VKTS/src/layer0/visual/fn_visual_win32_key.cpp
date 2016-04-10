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

#include <windows.h>

namespace vkts
{

static int32_t g_keys[0x15E];

VkBool32 VKTS_APIENTRY _visualInitKey()
{
    memset(g_keys, VKTS_KEY_UNKNOWN, sizeof(g_keys));

    g_keys[0x039] = VKTS_KEY_SPACE;

    g_keys[0x028] = VKTS_KEY_APOSTROPHE;

    g_keys[0x033] = VKTS_KEY_COMMA;
    g_keys[0x00C] = VKTS_KEY_MINUS;
    g_keys[0x034] = VKTS_KEY_PERIOD;
    g_keys[0x035] = VKTS_KEY_SLASH;
    g_keys[0x00B] = VKTS_KEY_0;
    g_keys[0x002] = VKTS_KEY_1;
    g_keys[0x003] = VKTS_KEY_2;
    g_keys[0x004] = VKTS_KEY_3;
    g_keys[0x005] = VKTS_KEY_4;
    g_keys[0x006] = VKTS_KEY_5;
    g_keys[0x007] = VKTS_KEY_6;
    g_keys[0x008] = VKTS_KEY_7;
    g_keys[0x009] = VKTS_KEY_8;
    g_keys[0x00A] = VKTS_KEY_9;

    g_keys[0x027] = VKTS_KEY_SEMICOLON;
    g_keys[0x056] = VKTS_KEY_LESS;
    g_keys[0x00D] = VKTS_KEY_EQUALS;

    g_keys[0x01A] = VKTS_KEY_LEFT_BRACKET;
    g_keys[0x02B] = VKTS_KEY_BACKSLASH;
    g_keys[0x01B] = VKTS_KEY_RIGHT_BRACKET;

    g_keys[0x029] = VKTS_KEY_GRAVE;
    g_keys[0x01E] = VKTS_KEY_A;
    g_keys[0x030] = VKTS_KEY_B;
    g_keys[0x02E] = VKTS_KEY_C;
    g_keys[0x020] = VKTS_KEY_D;
    g_keys[0x012] = VKTS_KEY_E;
    g_keys[0x021] = VKTS_KEY_F;
    g_keys[0x022] = VKTS_KEY_G;
    g_keys[0x023] = VKTS_KEY_H;
    g_keys[0x017] = VKTS_KEY_I;
    g_keys[0x024] = VKTS_KEY_J;
    g_keys[0x025] = VKTS_KEY_K;
    g_keys[0x026] = VKTS_KEY_L;
    g_keys[0x032] = VKTS_KEY_M;
    g_keys[0x031] = VKTS_KEY_N;
    g_keys[0x018] = VKTS_KEY_O;
    g_keys[0x019] = VKTS_KEY_P;
    g_keys[0x010] = VKTS_KEY_Q;
    g_keys[0x013] = VKTS_KEY_R;
    g_keys[0x01F] = VKTS_KEY_S;
    g_keys[0x014] = VKTS_KEY_T;
    g_keys[0x016] = VKTS_KEY_U;
    g_keys[0x02F] = VKTS_KEY_V;
    g_keys[0x011] = VKTS_KEY_W;
    g_keys[0x02D] = VKTS_KEY_X;
    g_keys[0x015] = VKTS_KEY_Y;
    g_keys[0x02C] = VKTS_KEY_Z;

    g_keys[0x001] = VKTS_KEY_ESCAPE;

    g_keys[0x03B] = VKTS_KEY_F1;
    g_keys[0x03C] = VKTS_KEY_F2;
    g_keys[0x03D] = VKTS_KEY_F3;
    g_keys[0x03E] = VKTS_KEY_F4;
    g_keys[0x03F] = VKTS_KEY_F5;
    g_keys[0x040] = VKTS_KEY_F6;
    g_keys[0x041] = VKTS_KEY_F7;
    g_keys[0x042] = VKTS_KEY_F8;
    g_keys[0x043] = VKTS_KEY_F9;
    g_keys[0x044] = VKTS_KEY_F10;
    g_keys[0x057] = VKTS_KEY_F11;
    g_keys[0x058] = VKTS_KEY_F12;

    g_keys[0x00E] = VKTS_KEY_BACKSPACE;
    g_keys[0x00F] = VKTS_KEY_TAB;
    g_keys[0x01C] = VKTS_KEY_ENTER;
    g_keys[0x03A] = VKTS_KEY_CAPS_LOCK;
    g_keys[0x02A] = VKTS_KEY_LEFT_SHIFT;
    g_keys[0x036] = VKTS_KEY_RIGHT_SHIFT;
    g_keys[0x01D] = VKTS_KEY_LEFT_CONTROL;

    g_keys[0x038] = VKTS_KEY_LEFT_ALT;
    g_keys[0x138] = VKTS_KEY_RIGHT_ALT;

    g_keys[0x15D] = VKTS_KEY_MENU;
    g_keys[0x11D] = VKTS_KEY_RIGHT_CONTROL;

    g_keys[0x046] = VKTS_KEY_SCROLL_LOCK;
    g_keys[0x045] = VKTS_KEY_PAUSE;

    g_keys[0x152] = VKTS_KEY_INSERT;
    g_keys[0x153] = VKTS_KEY_DELETE;
    g_keys[0x147] = VKTS_KEY_HOME;
    g_keys[0x14F] = VKTS_KEY_END;
    g_keys[0x149] = VKTS_KEY_PAGE_UP;
    g_keys[0x151] = VKTS_KEY_PAGE_DOWN;

    g_keys[0x14B] = VKTS_KEY_LEFT;
    g_keys[0x14D] = VKTS_KEY_RIGHT;
    g_keys[0x148] = VKTS_KEY_UP;
    g_keys[0x150] = VKTS_KEY_DOWN;

    g_keys[0x145] = VKTS_KEY_KP_NUM_LOCK;
    g_keys[0x135] = VKTS_KEY_KP_DIVIDE;
    g_keys[0x037] = VKTS_KEY_KP_MULTIPLY;
    g_keys[0x04A] = VKTS_KEY_KP_SUBTRACT;
    g_keys[0x04E] = VKTS_KEY_KP_ADD;
    g_keys[0x11C] = VKTS_KEY_KP_ENTER;
    g_keys[0x053] = VKTS_KEY_KP_DECIMAL;

    g_keys[0x052] = VKTS_KEY_KP_0;
    g_keys[0x04F] = VKTS_KEY_KP_1;
    g_keys[0x050] = VKTS_KEY_KP_2;
    g_keys[0x051] = VKTS_KEY_KP_3;
    g_keys[0x04B] = VKTS_KEY_KP_4;
    g_keys[0x04C] = VKTS_KEY_KP_5;
    g_keys[0x04D] = VKTS_KEY_KP_6;
    g_keys[0x047] = VKTS_KEY_KP_7;
    g_keys[0x048] = VKTS_KEY_KP_8;
    g_keys[0x049] = VKTS_KEY_KP_9;

    return VK_TRUE;
}

int32_t VKTS_APIENTRY _visualTranslateKey(WPARAM wParam, LPARAM lParam)
{
    int32_t keyCode = HIWORD(lParam) & 0x1FF;

    if (keyCode < 0 || keyCode >= 0x15E)
    {
        return VKTS_KEY_UNKNOWN;
    }

    return g_keys[keyCode];
}

}

