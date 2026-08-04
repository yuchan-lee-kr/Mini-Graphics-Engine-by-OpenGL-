#pragma once

const char* vertexShaderSource = 
R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoords;
    layout (location = 3) in vec3 aTangent;
       
    out vec3 Normal;
    out vec3 FragPos;
    out vec2 TexCoords;

    uniform mat3 normalMatrix;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main()
    {
        FragPos = vec3(model * vec4(aPos,1.0f));
        Normal = normalMatrix * aNormal;
        TexCoords = aTexCoords;
        gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char* fragmentShaderSource = 
R"(
    #version 330 core
    out vec4 FragColor;

    in vec3 Normal;
    in vec3 FragPos;
    in vec2 TexCoords;

    uniform vec3 viewPos;
    uniform vec3 lightColor;
    uniform vec3 meshColor;
    uniform vec3 objectColor;
    uniform float ele;
    uniform float azi;
    uniform sampler2D texture_diffuse;

    void main()
    {
        vec3 norm = normalize(Normal);
        float elevation = radians(45.0f);  // 지평선 위 45도
        float azimuth   = radians(30.0f);  // 방위각
        vec3 lightDir = normalize(vec3(cos(elevation)*sin(azimuth),
        -sin(elevation),cos(elevation)*cos(azimuth)));
        float Ambientcoefficient = 0.5f;
        vec3 Ambient = Ambientcoefficient * lightColor;
        float diff = max(dot(norm,-lightDir),0.0);
        vec3 Diffuse = diff * lightColor;
        float Specularcoefficient = 0.1f;
        vec3 CameraDir = normalize(viewPos - FragPos);
        vec3 Halfvector = normalize(CameraDir+lightDir);
        float spec = pow(max(dot(norm,Halfvector),0.0),64.0);
        vec3 Specular = Specularcoefficient * spec * lightColor;
        vec3 TextColor = texture(texture_diffuse,TexCoords).rgb;
        if(length(TextColor) < 0.001f) {
        TextColor = vec3(1.0f,1.0f,1.0f);}
        vec3 baseColor = TextColor.rgb * meshColor;
        vec3 result = (Ambient+Diffuse+Specular) * baseColor;
        FragColor = vec4(result, 1.0f); 
    }
)";