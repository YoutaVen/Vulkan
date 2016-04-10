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

namespace vkts
{

static int32_t g_keys[512];

VkBool32 VKTS_APIENTRY _visualInitKey()
{
    memset(g_keys, VKTS_KEY_UNKNOWN, sizeof(g_keys));

    g_keys[57] = VKTS_KEY_SPACE;
    g_keys[40] = VKTS_KEY_APOSTROPHE;
    g_keys[51] = VKTS_KEY_COMMA;
    g_keys[12] = VKTS_KEY_MINUS;
    g_keys[52] = VKTS_KEY_PERIOD;
    g_keys[53] = VKTS_KEY_SLASH;
    g_keys[11] = VKTS_KEY_0;
    g_keys[2] = VKTS_KEY_1;
    g_keys[3] = VKTS_KEY_2;
    g_keys[4] = VKTS_KEY_3;
    g_keys[5] = VKTS_KEY_4;
    g_keys[6] = VKTS_KEY_5;
    g_keys[7] = VKTS_KEY_6;
    g_keys[8] = VKTS_KEY_7;
    g_keys[9] = VKTS_KEY_8;
    g_keys[10] = VKTS_KEY_9;

    g_keys[39] = VKTS_KEY_SEMICOLON;
    g_keys[86] = VKTS_KEY_LESS;
    g_keys[13] = VKTS_KEY_EQUALS;

    g_keys[26] = VKTS_KEY_LEFT_BRACKET;
    g_keys[43] = VKTS_KEY_BACKSLASH;
    g_keys[27] = VKTS_KEY_RIGHT_BRACKET;

    g_keys[41] = VKTS_KEY_GRAVE;
    g_keys[30] = VKTS_KEY_A;
    g_keys[48] = VKTS_KEY_B;
    g_keys[46] = VKTS_KEY_C;
    g_keys[32] = VKTS_KEY_D;
    g_keys[18] = VKTS_KEY_E;
    g_keys[33] = VKTS_KEY_F;
    g_keys[34] = VKTS_KEY_G;
    g_keys[35] = VKTS_KEY_H;
    g_keys[23] = VKTS_KEY_I;
    g_keys[36] = VKTS_KEY_J;
    g_keys[37] = VKTS_KEY_K;
    g_keys[38] = VKTS_KEY_L;
    g_keys[50] = VKTS_KEY_M;
    g_keys[49] = VKTS_KEY_N;
    g_keys[24] = VKTS_KEY_O;
    g_keys[25] = VKTS_KEY_P;
    g_keys[16] = VKTS_KEY_Q;
    g_keys[19] = VKTS_KEY_R;
    g_keys[31] = VKTS_KEY_S;
    g_keys[20] = VKTS_KEY_T;
    g_keys[22] = VKTS_KEY_U;
    g_keys[47] = VKTS_KEY_V;
    g_keys[17] = VKTS_KEY_W;
    g_keys[45] = VKTS_KEY_X;
    g_keys[21] = VKTS_KEY_Y;
    g_keys[44] = VKTS_KEY_Z;

    g_keys[1] = VKTS_KEY_ESCAPE;

    g_keys[59] = VKTS_KEY_F1;
    g_keys[60] = VKTS_KEY_F2;
    g_keys[61] = VKTS_KEY_F3;
    g_keys[62] = VKTS_KEY_F4;
    g_keys[63] = VKTS_KEY_F5;
    g_keys[64] = VKTS_KEY_F6;
    g_keys[65] = VKTS_KEY_F7;
    g_keys[66] = VKTS_KEY_F8;
    g_keys[67] = VKTS_KEY_F9;
    g_keys[68] = VKTS_KEY_F10;
    g_keys[87] = VKTS_KEY_F11;
    g_keys[88] = VKTS_KEY_F12;

    g_keys[14] = VKTS_KEY_BACKSPACE;
    g_keys[15] = VKTS_KEY_TAB;
    g_keys[28] = VKTS_KEY_ENTER;
    g_keys[58] = VKTS_KEY_CAPS_LOCK;
    g_keys[42] = VKTS_KEY_LEFT_SHIFT;
    g_keys[54] = VKTS_KEY_RIGHT_SHIFT;
    g_keys[29] = VKTS_KEY_LEFT_CONTROL;

    g_keys[56] = VKTS_KEY_LEFT_ALT;
    g_keys[100] = VKTS_KEY_RIGHT_ALT;

    g_keys[127] = VKTS_KEY_MENU;
    g_keys[97] = VKTS_KEY_RIGHT_CONTROL;

    g_keys[70] = VKTS_KEY_SCROLL_LOCK;
    g_keys[119] = VKTS_KEY_PAUSE;

    g_keys[110] = VKTS_KEY_INSERT;
    g_keys[111] = VKTS_KEY_DELETE;
    g_keys[102] = VKTS_KEY_HOME;
    g_keys[107] = VKTS_KEY_END;
    g_keys[104] = VKTS_KEY_PAGE_UP;
    g_keys[109] = VKTS_KEY_PAGE_DOWN;

    g_keys[105] = VKTS_KEY_LEFT;
    g_keys[106] = VKTS_KEY_RIGHT;
    g_keys[103] = VKTS_KEY_UP;
    g_keys[108] = VKTS_KEY_DOWN;

    g_keys[69] = VKTS_KEY_KP_NUM_LOCK;
    g_keys[98] = VKTS_KEY_KP_DIVIDE;
    g_keys[55] = VKTS_KEY_KP_MULTIPLY;
    g_keys[74] = VKTS_KEY_KP_SUBTRACT;
    g_keys[78] = VKTS_KEY_KP_ADD;
    g_keys[96] = VKTS_KEY_KP_ENTER;
    g_keys[83] = VKTS_KEY_KP_DECIMAL;

    g_keys[82] = VKTS_KEY_KP_0;
    g_keys[79] = VKTS_KEY_KP_1;
    g_keys[80] = VKTS_KEY_KP_2;
    g_keys[81] = VKTS_KEY_KP_3;
    g_keys[75] = VKTS_KEY_KP_4;
    g_keys[76] = VKTS_KEY_KP_5;
    g_keys[77] = VKTS_KEY_KP_6;
    g_keys[71] = VKTS_KEY_KP_7;
    g_keys[72] = VKTS_KEY_KP_8;
    g_keys[73] = VKTS_KEY_KP_9;

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
