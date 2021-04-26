#pragma once


#include "Core/Common/CoreCommon.h"


class Algorithm
{
public:
    template<typename TGen, typename TLambda>
    FORCEINLINE static bool SortedIsSubsetOf(TGen smaller[], TGen bigger[], int smallerSize, int biggerSize, TLambda getter = [](TGen& value) -> TGen& { return value; })
    {

        int i = 0, j = 0;
        if (biggerSize < smallerSize)
            return false;

        while (i < smallerSize && j < biggerSize)
        {
            auto left = getter(bigger[j]);
            auto right = getter(smaller[i]);


            if (left < right)
                j++;
            else if (left == right)
            {
                j++;
                i++;
            }
            else if (left > right)
                return false;

        }

        return (i < smallerSize) ? false : true;
    }
};
