#pragma once


#include "Core/Common/CoreCommon.h"

class Sort
{
public:
    template<typename TValue, typename TLambda>
    FORCEINLINE static void Insertion(TValue arr[], size_t size, TLambda lambda = [](TValue& value) -> TValue& { return value; })
    {
        int i, j;

        auto key = arr[0];

        for (i = 1; i < size; i++)
        {
            key = arr[i];
            j = i - 1;

            /* Move elements of arr[0..i-1], that are
            greater than key, to one position ahead
            of their current position */
            while (j >= 0 && lambda(arr[j]) > lambda(key))
            {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
            arr[j + 1] = key;
        }
    }
};