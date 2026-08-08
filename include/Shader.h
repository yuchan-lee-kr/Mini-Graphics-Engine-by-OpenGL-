#pragma once

const char* vertexShaderSource = 
R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoords;
       
    out vec3 Normal;
    out vec3 FragPos;
    out vec2 TexCoords;
    out vec4 FragPosLightSpace;

    uniform mat3 normalMatrix;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 lightSpace;

    void main()
    {
        FragPos = vec3(model * vec4(aPos,1.0f));
        Normal = normalMatrix * aNormal;
        TexCoords = aTexCoords;
        FragPosLightSpace = lightSpace * vec4(FragPos,1.0);
        gl_Position = projection * view *  vec4(FragPos, 1.0);
    }
)";

const char* fragmentShaderSource = 
R"(
    #version 330 core
    out vec4 FragColor;
    struct Material
    {
        vec3 diffuseColor;
        vec3 specularColor;
        float shininess;
        float specularStrength;
    };

    in vec3 Normal;
    in vec3 FragPos;
    in vec2 TexCoords;
    in vec4 FragPosLightSpace;

    uniform Material material;
    uniform vec3 viewPos;
    uniform vec3 lightColor;
    uniform vec3 lightDir;
    uniform float ele;
    uniform float azi;
    uniform sampler2D texture_diffuse;
    uniform sampler2D shadowMap;

    float ShadowCalculation(vec4 fragPosLightSpace,vec3 normal,vec3 lightDir)
    {
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;
        if(projCoords.z > 1.0)
            return 0.0;
        float closestDepth = texture(shadowMap, projCoords.xy).r; 
        float currentDepth = projCoords.z;
        float bias = max(0.0005 * (1.0 - dot(normal, -lightDir)), 0.005);
        float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
        return shadow;
    }
    void main()
    {
        vec3 norm = normalize(Normal);
        //vec3 lightDir = normalize(vec3(cos(ele)*sin(azi),
        //-sin(ele),cos(ele)*cos(azi)));
        vec3 TextColor = texture(texture_diffuse,TexCoords).rgb;
        if(length(TextColor) < 0.001f) {
        TextColor = vec3(1.0f,1.0f,1.0f);}
        vec3 albedo = TextColor * material.diffuseColor;
        float AmbientStrength = 0.4f;
        vec3 Ambient = AmbientStrength * lightColor * albedo;
        float diff = max(dot(norm,-lightDir),0.0);
        vec3 Diffuse = diff * lightColor * albedo;
        vec3 CameraDir = normalize(viewPos - FragPos);
        vec3 Halfvector = normalize(CameraDir+(-lightDir));
        float spec = pow(max(dot(norm,Halfvector),0.0),material.shininess);
        vec3 Specular = material.specularStrength * material.specularColor *spec * lightColor;
        float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);
        //vec3 result = Ambient + (1.0 - shadow) * (Diffuse + Specular);
         vec3 result = Ambient + Diffuse + Specular;
        FragColor = vec4(result,1.0f); 
    }
)";
const char* LightvertexShaderSource =
R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
       

    uniform mat4 model;
    uniform mat4 lightSpace;

    void main()
    {
        gl_Position = lightSpace * model * vec4(aPos, 1.0);
    }
)";

const char* LightfragmentShaderSource =
R"(
    #version 330 core
    void main()
    {
        
    }
)";
