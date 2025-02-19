#pragma once
#include "Tools.h"

namespace GamEncin
{
    float* Vector3::VerticesVectorToFloatArr(vector<Vector3> vertices)
    {
        size_t size = vertices.size() * 3; // coordinate count
        float* result = new float[size];

        int j = 0;
        for(int i = 0; i < size; i += 3)
        {
            Vector3 currV3 = vertices.at(j);
            result[i] = currV3.x;
            result[i + 1] = currV3.y;
            result[i + 2] = currV3.z;
            j++;
        }

        return result;
    }
}
