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
    out vec3 Tangent;
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
        Tangent = normalMatrix * aTangent;
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
    in vec3 Tangent;
    in vec2 TexCoords;
    in vec4 FragPosLightSpace;

    uniform Material material;
    uniform vec3 viewPos;
    uniform vec3 lightColor;
    uniform vec3 lightDir;
    uniform float AmbientStrength;
    uniform float NormalStrength;
    uniform float ShadowStrength;
    uniform sampler2D texture_diffuse;
    uniform sampler2D shadowMap;
    uniform sampler2D texture_normal;
    uniform bool hasNormal;

    float ShadowCalculation(vec4 fragPosLightSpace,vec3 normal,vec3 lightDir)
    {
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;
        if(projCoords.z > 1.0)
            return 0.0;
        float currentDepth = projCoords.z;
        float bias = max(0.0005 * (1.0 - dot(normal, -lightDir)), 0.005);
        float shadow = 0.0f;
        vec2 texSize = 1.0f / textureSize(shadowMap,0);
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1;j++)
            {
                float PCFDepth = texture(shadowMap,projCoords.xy + vec2(i,j) * texSize).r;
                shadow += currentDepth - bias > PCFDepth ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;
        return shadow;
    }
    void main()
    {
        vec3 finalNormal;
        if(hasNormal){
        vec3 T = normalize(Tangent);
        vec3 norm = normalize(Normal);
        T = normalize(T - dot(T,norm)* norm);
        vec3 B = normalize(cross(T,norm));
        mat3 TBN = mat3(T,B,norm); 
        vec3 tangentNormal = texture(texture_normal,TexCoords).rgb * 2.0 - 1.0;
         tangentNormal.xy *= NormalStrength;
         finalNormal = normalize(TBN * tangentNormal);
        }
        else finalNormal= normalize(Normal);
        vec3 TextColor = pow(texture(texture_diffuse,TexCoords).rgb,vec3(2.2));
        if(length(TextColor) < 0.001f) {
        TextColor = vec3(1.0f,1.0f,1.0f);}
        vec3 albedo = TextColor * material.diffuseColor;
        vec3 Ambient = AmbientStrength * lightColor * albedo;
        float diff = max(dot(finalNormal,-lightDir),0.0);
        vec3 Diffuse = diff * lightColor * albedo;
        vec3 CameraDir = normalize(viewPos - FragPos);
        vec3 Halfvector = normalize(CameraDir+(-lightDir));
        float spec = pow(max(dot(finalNormal,Halfvector),0.0),material.shininess);
        vec3 Specular = material.specularStrength * material.specularColor *spec * lightColor;
        float shadow = ShadowCalculation(FragPosLightSpace, finalNormal, lightDir);
        float lastshadow = shadow * ShadowStrength;
        vec3 result = Ambient + (1.0 - lastshadow) * (Diffuse + Specular);
        FragColor = vec4(pow(result ,vec3(1.0 / 2.2)),1.0); 
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
