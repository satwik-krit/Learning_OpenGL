#version 460 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TextCoord;

uniform sampler2D texture2;
uniform sampler2D texture1;
/* uniform sampler2D ourTexture; */

void main()
{
    FragColor = mix(texture(texture1, TextCoord),texture(texture2, TextCoord), 0.5);
    // FragColor = texture(texture1, TextCoord);

}
