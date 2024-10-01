#shader vertex
#version 330

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iTexture;

out vec3 vFragmentPosition;
out vec3 vNormal;
out vec2 vTexture;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;

void main()
{
    vNormal = mat3(uNormalMatrix) * iNormal;
    vTexture = iTexture;
    vFragmentPosition = vec3(uModel * vec4(iPosition, 1.0));

    gl_Position = uProjection * uView * vec4(vFragmentPosition, 1.0);
}

#shader fragment
#version 330

in vec3 vFragmentPosition;
in vec3 vNormal;
in vec2 vTexture;

out vec4 oFragment;

uniform float uSpeed;
uniform vec3 uViewPosition;
uniform sampler2D uBathy2D;
uniform sampler2D uShallow2D;
uniform sampler2D uClouds2D;

struct DirecionalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

vec4 CalculateDirectionalLighting(DirecionalLight light, vec3 normal, vec3 viewDirection);

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 viewDirection = normalize(uViewPosition - vFragmentPosition);

    // Sun
    DirecionalLight light;
    light.ambient = vec3(0.0);
    light.diffuse = vec3(1.0);
    light.specular = vec3(0.4);
    light.direction = vec3(0.8, 0.0, 1.0);
    
    // Output
    vec4 result = CalculateDirectionalLighting(light, normal, viewDirection);
    oFragment = result;
}

vec4 CalculateDirectionalLighting(DirecionalLight light, vec3 normal, vec3 viewDirection)
{
    // Material
    vec2 cloudsSpeed = vec2(0.004, 0.0);
    vec4 bathy = texture(uBathy2D, vTexture);
    vec4 shallow = texture(uShallow2D, vTexture);
    vec4 clouds = texture(uClouds2D, vTexture + uSpeed * cloudsSpeed);
    vec4 material = mix(bathy, shallow, 0.6) + clouds;

    // Ambient
    vec4 ambient = vec4(light.ambient, 1.0) * material;

    // Diffuse
    vec3 direction = normalize(light.direction);
    float diffuseFactor = max(dot(normal, direction), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diffuseFactor * material;

    // Specular
    vec4 specular = vec4(0.0);

    if (diffuseFactor > 0.0)
    {
        vec3 halfWayDirection = normalize(viewDirection + direction);
        float specularFactor = pow(max(dot(normal, halfWayDirection), 0.0), 128.0);
        specular = vec4(light.specular, 1.0) * specularFactor * material.a;
    }

    // Output
    return ambient + diffuse + specular;
}

