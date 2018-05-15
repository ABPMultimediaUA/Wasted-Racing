#version 450 core

uniform sampler2D texture;

in vec2 TextCoord;
out vec4 FragColor;

void main()
{

    FragColor = texture2D(texture, TextCoord);

    //FragColor = vec4(1.0, 0.0, 0.5, 1.0);


    if(FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0)
    {
        discard;
    }


}