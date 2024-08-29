#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec2 textCoord;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 textCOORD;


void main()
{
    // TODO(student): Compute world space vertex position and normal,
    // and send them to the fragment shader
    textCOORD.x = v_texture_coord.x - textCoord.x;
    textCOORD.y = v_texture_coord.y - textCoord.y;

    world_position = (Model * vec4(v_position,1)).xyz;
    world_normal = normalize( mat3(Model) * v_normal );

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
