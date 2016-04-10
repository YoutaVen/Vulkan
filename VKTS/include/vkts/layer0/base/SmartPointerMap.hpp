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

#ifndef VKTS_SMARTPOINTERMAP_HPP_
#define VKTS_SMARTPOINTERMAP_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

template<class K, class V>
class SmartPointerMap
{

protected:

    Vector<K> allKeys;
    SmartPointerVector<V> allValues;

    const V empty;

public:

    SmartPointerMap() :
        SmartPointerMap(VKTS_INITIAL_VECTOR_COUNT)
    {
    }

    SmartPointerMap(const size_t& allDataCount) :
        allKeys(allDataCount), allValues(allDataCount), empty(nullptr)
    {
    }

    SmartPointerMap(const SmartPointerMap& other) :
        allKeys(other.allKeys), allValues(other.allValues)
    {
    }

    SmartPointerMap(SmartPointerMap&& other) :
        allKeys(other.allKeys), allValues(other.allValues)
    {
    }

    SmartPointerMap& operator= (const SmartPointerMap& other)
    {
        allKeys = other.allKeys;
        allValues = other.allValues;

        //

    	return *this;
    }

    SmartPointerMap& operator= (SmartPointerMap&& other)
    {
        allKeys = other.allKeys;
        allValues = other.allValues;

        //

    	return *this;
    }

    ~SmartPointerMap()
    {
        clear();
    }

    void clear()
    {
        allKeys.clear();
        allValues.clear();
    }

    size_t find(const K& key) const
    {
        size_t start = 0;
        size_t end = allKeys.size();

        size_t result = 0;

        while (start < end)
        {
            result = (end + start) / 2;

            if (key == allKeys[result])
            {
                return result;
            }
            else if (end - start == 1)
            {
                break;
            }

            if (key < allKeys[result])
            {
                end = result;
            }
            else if (key > allKeys[result])
            {
                start = result;
            }
        }

        return allKeys.size();
    }

    VkBool32 set(const K& key, const V& value)
    {
        for (size_t i = 0; i < allKeys.size(); i++)
        {
            if (key == allKeys[i])
            {
                allValues[i] = value;

                return VK_TRUE;
            }
            else if (key < allKeys[i])
            {
                allKeys.insert(i, key);
                allValues.insert(i, value);

                return VK_TRUE;
            }
        }

        allKeys.append(key);
        allValues.append(value);

        return VK_TRUE;
    }

    VkBool32 remove(const K& key)
    {
        size_t index = find(key);

        if (index != allKeys.size())
        {
            return removeAt(index);
        }

        return VK_FALSE;
    }

    VkBool32 removeAt(const size_t index)
    {
        if (index >= allKeys.size())
        {
            return VK_FALSE;
        }

        allKeys.removeAt(index);
        allValues.removeAt(index);

        return VK_TRUE;
    }

    VkBool32 contains(const K& key) const
    {
        return find(key) != allKeys.size();
    }

    V& operator[](const K& key)
    {
        size_t index = find(key);

        if (index != allKeys.size())
        {
            return allValues[index];
        }

        V v;
        set(key, v);

        return allValues[find(key)];
    }

    const V& operator[](const K& key) const
    {
        size_t index = find(key);

        if (index == allKeys.size())
        {
            return empty;
        }

        return allValues[index];
    }

    K& keyAt(const size_t index)
    {
        return allKeys[index];
    }

    const K& keyAt(const size_t index) const
    {
        return allKeys[index];
    }

    V& valueAt(const size_t index)
    {
        return allValues[index];
    }

    const V& valueAt(const size_t index) const
    {
        return allValues[index];
    }

    const SmartPointerVector<K>& keys() const
    {
        return allKeys;
    }

    const SmartPointerVector<V>& values() const
    {
        return allValues;
    }

    size_t size() const
    {
        return allKeys.size();
    }
};

}

#endif /* VKTS_SMARTPOINTERMAP_HPP_ */
