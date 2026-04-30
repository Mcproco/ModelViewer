#version 330 core
/*
    
    ## VERTEX SHADER ##

    Ran on every single vertex. This step needs to be defined
    to pass on normalized device coordinates (NDC) to the next
    steps in the shader pipeline, to be converted to fragments

    You only get so many attributes per vertex. I believe its
    around 16. This is useful for defining properties such as
    the position, UV coordinates, etc. Just keep in mind that
    anything unique that needs to be attached to the verticies
    should be in the vertex attributes.

*/

layout (location = 0) in vec3 localPosition;
layout (location = 1) in vec2 aTex;

out vec2 TexCoord;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main() {

    /* In this step, transform the vertex's local position to the model's transform, thus translating it into global space */
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(localPosition, 1.0f); /* Since matrix operations are read right to left. */
    TexCoord = aTex;

}
