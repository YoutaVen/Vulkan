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

#include "fn_visual_linux_internal.hpp"

namespace vkts
{
    
static int  g_keyFileDescriptor = -1;    
static int  g_mouseFileDescriptor = -1;

static glm::ivec2 g_displayExtent = glm::ivec2(0, 0);
static glm::ivec2 g_mousePosition = glm::ivec2(0, 0); 

VkBool32 VKTS_APIENTRY _visualInitDisplay(const VkInstance instance, const VkPhysicalDevice physicalDevice)
{
	char eventFilename[256];
	int eventNumber;

	int fileDescriptor;

    // Detect keyboard and mouse.
    
	uint64_t eventBits;

	strcpy(eventFilename, "/dev/input/event0");

	for (eventNumber = 0; eventNumber < 10; eventNumber++)
	{
		eventFilename[16] = '0' + (char)eventNumber;

		fileDescriptor = open(eventFilename, O_RDONLY | O_NONBLOCK);
		
		if (fileDescriptor < 0)
		{
			continue;
		}

		eventBits = 0;

		if (ioctl(fileDescriptor, EVIOCGBIT(0, EV_MAX), &eventBits) < 0)
		{
			close(fileDescriptor);

			continue;
		}

		if (eventBits == 0x120013)
		{
            system("stty -echo");

            g_keyFileDescriptor = fileDescriptor;
		}
        else if (eventBits == 0x17)
        {
            g_mouseFileDescriptor = fileDescriptor;
        }
        else
        {
            close(fileDescriptor);    
        }
	}
    
    // Detect gamepad.
    
    if (!_visualInitGamepad(instance, physicalDevice))
    {
    	return VK_FALSE;
    }

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY _visualDispatchMessagesDisplay()
{
    // Keyboard.

    if (g_keyFileDescriptor >= 0)
    {
        struct input_event keyEvent;

		ssize_t numBytes;

		do
		{
			numBytes = read(g_keyFileDescriptor, &keyEvent, sizeof(struct input_event));

			if (numBytes > 0 && keyEvent.type == EV_KEY)
			{
                int32_t keyIndex = _visualTranslateKey(keyEvent.code);
                
                if (keyIndex != VKTS_KEY_UNKNOWN)
                {
                    switch (keyEvent.value)
                    {
                        case 1:	// Pressed
                        {
                            if (keyIndex == VKTS_KEY_ESCAPE)
                            {
                                return VK_FALSE;
                            }
                            
                            _visualKeySet(keyIndex, VK_TRUE);
                        }
                        break;

                        case 0: // Released
                        {
                            _visualKeySet(keyIndex, VK_FALSE);
                        }
                        break;
                    }
                }
            }            
        } while (numBytes > 0);        
    }
    
    // Mouse.
    
    if (g_mouseFileDescriptor >= 0)
    {
        struct input_event mouseEvent;

		ssize_t numBytes;

		do
		{
			numBytes = read(g_mouseFileDescriptor, &mouseEvent, sizeof(struct input_event));

			if (numBytes > 0)
			{
                // Buttons.
                if (mouseEvent.type == 1)
                {
                    switch (mouseEvent.code)
                    {
                        // Left
                        case 272:
                            _visualMouseSetButton(VKTS_MOUSE_BUTTON_LEFT, mouseEvent.value ? VK_TRUE : VK_FALSE);
                            break; 
                        // Rigtht
                        case 273: 
                            _visualMouseSetButton(VKTS_MOUSE_BUTTON_RIGHT, mouseEvent.value ? VK_TRUE : VK_FALSE);
                            break; 
                        // Middle
                        case 274: 
                            _visualMouseSetButton(VKTS_MOUSE_BUTTON_MIDDLE, mouseEvent.value ? VK_TRUE : VK_FALSE);
                            break; 
                    }
                }
                // Position and wheel.
                else if (mouseEvent.type == 2)
                {
                    switch (mouseEvent.code)
                    {
                        // Mouse x position delta.
                        case 0:
                            {
                                int32_t newX = -1;
                                    
                                if (mouseEvent.value < 0 && g_mousePosition.x > 0)
                                {
                                    newX = glm::max(0, g_mousePosition.x + (int32_t)mouseEvent.value);
                                }
                                else if (mouseEvent.value > 0 && g_mousePosition.x < g_displayExtent.x - 1)
                                {
                                    newX = glm::min(g_displayExtent.x - 1, g_mousePosition.x + (int32_t)mouseEvent.value);
                                }
                
                                if (newX != -1)
                                {
                                    g_mousePosition.x = newX;
                                    
                                    _visualMouseSetLocation(g_mousePosition);
                                }
                            }
                            break; 
                        // Mouse y position delta.
                        case 1:
                            { 
                                int32_t newY = -1;
                                    
                                if (mouseEvent.value < 0 && g_mousePosition.y > 0)
                                {
                                    newY = glm::max(0, g_mousePosition.y + (int32_t)mouseEvent.value);
                                }
                                else if (mouseEvent.value > 0 && g_mousePosition.y < g_displayExtent.y - 1)
                                {
                                    newY = glm::min(g_displayExtent.y - 1, g_mousePosition.y + (int32_t)mouseEvent.value);
                                }
                
                                if (newY != -1)
                                {
                                    g_mousePosition.y = newY;
                                    
                                    _visualMouseSetLocation(g_mousePosition);
                                }
                            }
                            break; 
                        // Mouse wheel position.
                        case 8:
                            _visualMouseSetMouseWheel((int32_t)mouseEvent.value); 
                            break; 
                    }
                }
            }
                
        } while (numBytes > 0);        
    }

    //
    
	VkBool32 result = _visualDispatchMessagesGamepad();

	if (!result)
	{
		return VK_FALSE;
	}

    return VK_TRUE;
}

void VKTS_APIENTRY _visualTerminateDisplay()
{
	_visualTerminateGamepad();
    
    if (g_mouseFileDescriptor >= 0)
    {
        close(g_mouseFileDescriptor);
        
        g_mouseFileDescriptor = -1; 
    }
    
    if (g_keyFileDescriptor >= 0)
    {
        close(g_keyFileDescriptor);
        
        g_keyFileDescriptor = -1; 
        
        system("stty echo");   
    }
}

//

void VKTS_APIENTRY _visualSetDisplayExtent(const glm::ivec2& extent)
{
    g_displayExtent = extent;
    
    g_mousePosition = extent / 2;
}

}
