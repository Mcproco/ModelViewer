typedef struct {
    unsigned int programId;
} ShaderProgram;

void ShaderUseProgram(ShaderProgram s);
void ShaderGenProgram(const char *vertexSrc, const char *fragSrc, ShaderProgram *output);
int  ShaderUniformLocation(const char* name, ShaderProgram target);
