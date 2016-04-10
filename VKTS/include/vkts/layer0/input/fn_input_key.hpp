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

#ifndef VKTS_FN_INPUT_KEY_HPP_
#define VKTS_FN_INPUT_KEY_HPP_

#define VKTS_MAX_KEYS                       309

#define VKTS_KEY_UNKNOWN                    -1

// Reference: Ubuntu English (US) keyboard layout chart using an European keyboard.

//
// Printable ASCII keys.
//
#define VKTS_KEY_SPACE                      32      // ' '

#define VKTS_KEY_APOSTROPHE                 39      // '''

#define VKTS_KEY_COMMA                      44      // ','
#define VKTS_KEY_MINUS                      45      // '-'
#define VKTS_KEY_PERIOD                     46      // '.'
#define VKTS_KEY_SLASH                      47      // '/'
#define VKTS_KEY_0                          48      // '0'
#define VKTS_KEY_1                          49      // '1'
#define VKTS_KEY_2                          50      // '2'
#define VKTS_KEY_3                          51      // '3'
#define VKTS_KEY_4                          52      // '4'
#define VKTS_KEY_5                          53      // '5'
#define VKTS_KEY_6                          54      // '6'
#define VKTS_KEY_7                          55      // '7'
#define VKTS_KEY_8                          56      // '8'
#define VKTS_KEY_9                          57      // '9'

#define VKTS_KEY_SEMICOLON                  59      // ';'
#define VKTS_KEY_LESS                       60      // '<'
#define VKTS_KEY_EQUALS                     61      // '='

#define VKTS_KEY_LEFT_BRACKET               91      // '['
#define VKTS_KEY_BACKSLASH                  92      // '\'
#define VKTS_KEY_RIGHT_BRACKET              93      // ']'

#define VKTS_KEY_GRAVE                      96      // '`'
#define VKTS_KEY_A                          97      // 'a'
#define VKTS_KEY_B                          98      // 'b'
#define VKTS_KEY_C                          99      // 'c'
#define VKTS_KEY_D                          100     // 'd'
#define VKTS_KEY_E                          101     // 'e'
#define VKTS_KEY_F                          102     // 'f'
#define VKTS_KEY_G                          103     // 'g'
#define VKTS_KEY_H                          104     // 'h'
#define VKTS_KEY_I                          105     // 'i'
#define VKTS_KEY_J                          106     // 'j'
#define VKTS_KEY_K                          107     // 'k'
#define VKTS_KEY_L                          108     // 'l'
#define VKTS_KEY_M                          109     // 'm'
#define VKTS_KEY_N                          110     // 'n'
#define VKTS_KEY_O                          111     // 'o'
#define VKTS_KEY_P                          112     // 'p'
#define VKTS_KEY_Q                          113     // 'q'
#define VKTS_KEY_R                          114     // 'r'
#define VKTS_KEY_S                          115     // 's'
#define VKTS_KEY_T                          116     // 't'
#define VKTS_KEY_U                          117     // 'u'
#define VKTS_KEY_V                          118     // 'v'
#define VKTS_KEY_W                          119     // 'w'
#define VKTS_KEY_X                          120     // 'x'
#define VKTS_KEY_Y                          121     // 'y'
#define VKTS_KEY_Z                          122     // 'z'

//
// Function keys are located above ASCII.
//
#define VKTS_KEY_ESCAPE                     256

#define VKTS_KEY_F1                         257
#define VKTS_KEY_F2                         258
#define VKTS_KEY_F3                         259
#define VKTS_KEY_F4                         260
#define VKTS_KEY_F5                         261
#define VKTS_KEY_F6                         262
#define VKTS_KEY_F7                         263
#define VKTS_KEY_F8                         264
#define VKTS_KEY_F9                         265
#define VKTS_KEY_F10                        266
#define VKTS_KEY_F11                        267
#define VKTS_KEY_F12                        268

#define VKTS_KEY_BACKSPACE                  269
#define VKTS_KEY_TAB                        270
#define VKTS_KEY_ENTER                      271
#define VKTS_KEY_CAPS_LOCK                  272
#define VKTS_KEY_LEFT_SHIFT                 273
#define VKTS_KEY_RIGHT_SHIFT                274
#define VKTS_KEY_LEFT_CONTROL               275

#define VKTS_KEY_LEFT_ALT                   276
#define VKTS_KEY_RIGHT_ALT                  277

#define VKTS_KEY_MENU                       278
#define VKTS_KEY_RIGHT_CONTROL              279

#define VKTS_KEY_SCROLL_LOCK                280
#define VKTS_KEY_PAUSE                      281

#define VKTS_KEY_INSERT                     282
#define VKTS_KEY_DELETE                     283
#define VKTS_KEY_HOME                       284
#define VKTS_KEY_END                        285
#define VKTS_KEY_PAGE_UP                    286
#define VKTS_KEY_PAGE_DOWN                  287

#define VKTS_KEY_LEFT                       288
#define VKTS_KEY_RIGHT                      289
#define VKTS_KEY_UP                         290
#define VKTS_KEY_DOWN                       291

#define VKTS_KEY_KP_NUM_LOCK                292
#define VKTS_KEY_KP_DIVIDE                  293
#define VKTS_KEY_KP_MULTIPLY                294
#define VKTS_KEY_KP_SUBTRACT                295
#define VKTS_KEY_KP_ADD                     296
#define VKTS_KEY_KP_ENTER                   297
#define VKTS_KEY_KP_DECIMAL                 298

#define VKTS_KEY_KP_0                       299
#define VKTS_KEY_KP_1                       300
#define VKTS_KEY_KP_2                       301
#define VKTS_KEY_KP_3                       302
#define VKTS_KEY_KP_4                       303
#define VKTS_KEY_KP_5                       304
#define VKTS_KEY_KP_6                       305
#define VKTS_KEY_KP_7                       306
#define VKTS_KEY_KP_8                       307
#define VKTS_KEY_KP_9                       308

#endif /* VKTS_FN_INPUT_KEY_HPP_ */
