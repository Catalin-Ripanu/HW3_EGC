#version 330
#define MAX 50
// Input
in vec3 world_position;
in vec3 world_normal;
in vec2 textCOORD;

uniform vec3 lightDirection;
uniform vec3 object_color;
uniform int spotLights;
uniform vec3 pointLightPositions[MAX];
uniform vec3 pointLightColors[MAX];
uniform vec3 lightPositions[MAX];
uniform vec3 lightDirections[MAX];

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform sampler2D textureObj;
uniform int pointLights;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float slope;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 N = normalize(world_normal);

    float directionalLight = material_ks * max(dot(N, lightDirection), 0);

    float ambient_light = -0.5 * material_kd;

    vec3 L_spot[MAX];
    vec3 V_spot = normalize( eye_position - world_position);
    vec3 H_spot[MAX];

    float diffuse_light_spot[MAX];
    float specular_light_spot[MAX];

    float factor_atenuare_spot[MAX];

    float alpha = 30;
    float cut_off = radians(alpha);
    
    float spot_light_limit;
    float linear_att;
    float light_att_factor[MAX];

    vec3 L[MAX];
    vec3 V = normalize(eye_position - world_position);
    vec3 H[MAX];

    vec3 diffuse_light[MAX];
    vec3 specular_light[MAX];

    float factor_atenuare[MAX];

    float sum = 0;

    for (int i = 0; i < spotLights; i++) {
        L_spot[i] = normalize( lightPositions[i] - world_position);
        H_spot[i] = normalize( L_spot[i] + V_spot );
    }

    for (int i = 0; i < spotLights; i++) {
        diffuse_light_spot[i] = 0;
        specular_light_spot[i] = 0;
    }
    
    for (int i = 0; i < spotLights; i++) {
        factor_atenuare_spot[i] = 1/(1 + 0 + 0.004 * pow(distance(lightPositions[i], world_position),2));
    }

    for (int i = 0; i < spotLights; i++) {
        float spot_light = dot(-L_spot[i], normalize(lightDirections[i]));
        if (spot_light > cos(cut_off))
        {
            diffuse_light_spot[i] = material_kd * max (dot(N, L_spot[i]), slope);

            if (diffuse_light_spot[i] > slope)
            {
               specular_light_spot[i] = material_ks * pow(max(dot(N, H_spot[i]), 0), material_shininess);
            }

            spot_light_limit = cos(cut_off);
            linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            light_att_factor[i] = pow(linear_att, 2);
        }
    }

    for(int i = 0; i < pointLights; i++) {
        L[i] = normalize(pointLightPositions[i] - world_position);
        H[i] = normalize(L[i] + V );
    }

    for(int i = 0; i < pointLights; i++) {
        diffuse_light[i] = vec3(0);
        specular_light[i] = vec3(0);
    }

    for(int i = 0; i < pointLights; i++) {
        diffuse_light[i] = material_kd * pointLightColors[i] * max (dot(N, L[i]), slope);
    }


    for(int i = 0; i < pointLights; i++) {
        float specular_factor = max (dot(N, L[i]), slope);
        if (specular_factor > slope)
        {
            specular_light[i] = material_ks * pointLightColors[i] * pow(max(dot(N, H[i]), 0), material_shininess);
        }
    }

    for(int i = 0; i < pointLights; i++) {
        factor_atenuare[i] = 1/(1 + 0 + 0.1 * pow(distance(pointLightPositions[i], world_position),2));
    }

    for (int i = 0; i < spotLights; i++) {
        sum += factor_atenuare_spot[i] * (diffuse_light_spot[i] + specular_light_spot[i]) * light_att_factor[i];
    }

    sum += directionalLight;

    vec3 color = object_color * (ambient_light + sum);

    for(int i = 0; i < pointLights; i++) {
        color += factor_atenuare[i] * (diffuse_light[i] + specular_light[i]);
    }

    out_color = vec4(color, 1);
    out_color += texture(textureObj, textCOORD);
  
    if(out_color.a < 0.5f)
        discard;

}