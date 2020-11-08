#version 450 core

/*layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;*/

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gWorldPosition;

in vec2 TexCoords;
in vec4 FragPos;
in vec4 FragPosWorld;
in vec3 Normal;

uniform sampler2D Texture;

void main()
{    

    gPosition = FragPos;
    gWorldPosition = FragPosWorld;

    gNormal = vec4((normalize(Normal) + 1.0f) / 2.0f, 1.0);

    gAlbedo = texture(Texture, TexCoords);

}