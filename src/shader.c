#include <glad/gl.h>
#include <stdio.h>
#include "shader.h"

/* Can't a white boy speak a little "mexicana"? */
void _parse(FILE *fp, unsigned int *shaderID, GLuint type, ShaderProgram program) {

    /*
     * Im just putting this here for future reference
     * This is sort-of bad practice since what im trying
     * to do here is create a shader program and return 
     * it. Although not by nature terrible, if you were to 
     * try to return a pointer created on the stack in 
     * this function scope then it would return a dangling
     * pointer.
     *
     * Kinda shit design mr bell labs
     *
     * Yet this happens since all local variables are stored
     * on the newly pushed stack frame's cache. Once the stack
     * frame is popped from the stack, all local variables allocated
     * by the stack are then freed. This can be circumvented by
     * using the heap.
     *
     * I guess its kinda intuitive in some ways but it does
     * get pretty funky
     *
     */

    *shaderID = glCreateShader(type);

    /* Why isnt there just a function to return the size of a file????? Or just return string contents or something stdio what are we doing */
    long bufferSize;
    if (fseek(fp, 0L, SEEK_END) == 0) {
        bufferSize = ftell(fp);
        if (bufferSize == -1) {
            *shaderID = -1;
            return;
        }

    }

    fseek(fp, 0L, SEEK_SET);

    char source[bufferSize + 1];
    size_t length = fread(source, sizeof(char), bufferSize, fp); /* Read <bufferSize> chars from fp into source */
    source[length++] = '\0'; /* Ensure that it is cut off... */

    const char *code = source; /* Can't let compiler yell at me for passing a non const string */
    glShaderSource(*shaderID, 1, &code, NULL);
    glCompileShader(*shaderID);
    glAttachShader(program.programId, *shaderID);

}

void ShaderGenProgram(const char *vertexSrcPath, const char *fragSrcPath, ShaderProgram *output) {

    output->programId = glCreateProgram();

    FILE *vShader = fopen(vertexSrcPath, "r");
    FILE *fShader = fopen(fragSrcPath, "r");
    unsigned int vertexShader, fragmentShader;

    if (vShader == NULL || fShader == NULL) { 
        return;
    }

    /* stupid notation since *output.programId would be refering to programId's pointer... (the field selection operator has higher operator precedence by design) */
    _parse(vShader, &vertexShader, GL_VERTEX_SHADER, *output);
    _parse(fShader, &fragmentShader, GL_FRAGMENT_SHADER, *output);

    /* Better... */
    glLinkProgram(output->programId);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void ShaderUseProgram(ShaderProgram s) {
    glUseProgram(s.programId);
}

int ShaderUniformLocation(const char *name, ShaderProgram target) {
    return glGetUniformLocation(target.programId, name);
}
