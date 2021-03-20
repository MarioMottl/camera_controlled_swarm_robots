#version 460 core

struct DirLightSource
{
    vec3 direction;
    vec3 color;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    sampler2D shadowMap;
};

struct SpotlightSource
{
    vec3 direction;
    vec3 position;
    vec3 color;

    float innerAngle;
    float outherAngle;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;

    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    int isSource;
    sampler2D diffuse;      // ambient has the same color (texture) with another reflection strength from the lightsource
    sampler2D specular;
};

uniform DirLightSource worldLight;
//uniform SpotlightSource lampSpotlight;
uniform Material material;
uniform vec3 camPos;

in VS_OUT
{
    vec2 fragTexCoord;
    vec3 fragPos;
    vec3 normalVec;
    vec4 fragPosLightSpace;
} fs_in;

out vec4 resultColor;

float getShadowValue(DirLightSource lightSource, vec4 vecLightPos, vec3 normal);
vec3 lightAdd(vec3 u, vec3 v);
vec3 directionalLight(DirLightSource light, Material mtl, vec3 normal, vec3 view);
vec3 spotLight(SpotlightSource light, Material mtl, vec3 normal, vec3 view);

float getShadowValue(DirLightSource lightSource, vec4 vecLightPos, vec3 normal)
{
    // transform clip-space light position to NDC
    vec3 projectPos = vecLightPos.xyz / vecLightPos.w;

    // transform light-NDC-pos to a range of 0 to for texture sampling
    projectPos = projectPos * 0.5f + 0.5f;

    // get depth of current fragment
    float fragDepth = projectPos.z; // in NDC-space the z component is the depth value
    
    float shadowValue = 0.0f;
    vec2 texelSize = 1.0f / textureSize(lightSource.shadowMap, 0);
    int range = 0;
    for(int y = -range; y <= range; y++)
    {
        for(int x = -range; x <= range; x++)
        {
            float pcfDepth = texture(lightSource.shadowMap, projectPos.xy + texelSize * vec2(x, y)).r;
            shadowValue += (fragDepth > pcfDepth && projectPos.z < 1.0f) ? 1.0f : 0.0f;
        }
    }
    shadowValue /= 1;

    return (1.0f - shadowValue);
}

vec3 lightAdd(vec3 u, vec3 v)
{
    return (u + v) / (vec3(1.0f) + u*v);
}

vec3 directionalLight(DirLightSource light, Material mtl, vec3 normal, vec3 viewVec)
{
    vec3 diffuseAmbientColor = light.color * texture(mtl.diffuse, fs_in.fragTexCoord).rgb;
    vec3 specularColor = light.color * texture(mtl.specular, fs_in.fragTexCoord).r;
    float specularHighlight = texture(mtl.specular, fs_in.fragTexCoord).g * 100.0f;
    vec3 lightDirection = normalize(light.direction);
    float shadowValue = getShadowValue(light, fs_in.fragPosLightSpace, normal);

    // ambient
    vec3 ambientLight = light.ambientStrength * diffuseAmbientColor;

    // diffuse
    float diffuseFactor = max(dot(lightDirection, normal), 0.0f);
    vec3 diffuseLight = light.diffuseStrength * diffuseFactor * diffuseAmbientColor;

    // specular
    vec3 specularLight = vec3(0.0f);
    if(diffuseFactor > 0.0f)
    {
        vec3 halfwayVec = normalize(lightDirection + viewVec);
        float specularFactor = pow(max(dot(halfwayVec, normal), 0.0f), specularHighlight);
        specularLight = light.specularStrength * specularFactor * specularColor;
    }

    return (ambientLight + shadowValue * (diffuseLight + specularLight));
}

vec3 spotLight(SpotlightSource light, Material mtl, vec3 normal, vec3 viewVec)
{
    vec3 diffuseAmbientColor = light.color * texture(mtl.diffuse, fs_in.fragTexCoord).rgb;
    vec3 specularColor = light.color * texture(mtl.specular, fs_in.fragTexCoord).r;
    float specularHighlight = texture(mtl.specular, fs_in.fragTexCoord).g * 100.0f;
    vec3 lightrayDirection = normalize(light.position - fs_in.fragPos);

    float epsilon = light.innerAngle - light.outherAngle;
    float theta = dot(normalize(light.direction), -lightrayDirection);
    float spotIntensity = clamp((theta - light.outherAngle) / epsilon, 0.0f, 1.0f);

    float distance = length(light.position - fs_in.fragPos);
    float attenutation = 1.0f / (distance * (light.quadratic * distance + light.linear) + light.constant);

    // ambient
    vec3 ambientLight = attenutation * light.ambientStrength * diffuseAmbientColor;

    // diffuse
    float diffuseFactor = max(dot(lightrayDirection, normal), 0.0f);
    vec3 diffuseLight =  attenutation * spotIntensity * light.diffuseStrength * diffuseFactor * diffuseAmbientColor;

    // specular
    vec3 specularLight = vec3(0.0f);
    if(diffuseFactor > 0.0f)
    {
        vec3 halfwayVec = normalize(lightrayDirection + viewVec);
        float specularFactor = pow(max(dot(halfwayVec, normal), 0.0f), specularHighlight);
        specularLight = attenutation * spotIntensity * light.specularStrength * specularFactor * specularColor;
    }

    return (ambientLight + diffuseLight + specularLight);
}

void main()
{
    if(material.isSource == 0)
    {
        float opacity = texture(material.diffuse, fs_in.fragTexCoord).a;
        vec3 viewVec = normalize(camPos - fs_in.fragPos);

        vec3 resColor = directionalLight(worldLight, material, fs_in.normalVec, viewVec);
        resultColor = vec4(resColor, opacity);
    }
    else
    {
        resultColor = texture(material.diffuse, fs_in.fragTexCoord) * vec4(worldLight.color, 1.0f);
    }
}
