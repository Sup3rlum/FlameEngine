
float LinearizeDepth(float d, float zNear, float zFar)
{
    return 2 * zNear * zFar / (zFar + zNear + d * (zNear - zFar));
}

void swap(inout float a, inout float b)
{
    float t = a;
    a = b;
    b = t;
}

float GetLuminance(float3 color)
{
    return dot(color, float3(0.299, 0.587, 0.114));

}

