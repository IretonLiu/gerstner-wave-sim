#include "gl_helper.hpp"

GLFWwindow* window;
GLuint VAO, vertexPositionBuffer, vertexPositionIndexBuffer;
int loadShaders(const char* vertexshader_path, const char* fragmentshader_path) {
    // Create the shaders
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(vertexshader_path, std::ios::in);
    if (vertexShaderStream.is_open()) {  // Check if the file can be opened
        std::stringstream sstr;
        sstr << vertexShaderStream.rdbuf();
        vertexShaderCode = sstr.str();
        vertexShaderStream.close();
    } else {
        printf("Could not open %s.?\n", vertexshader_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(fragmentshader_path, std::ios::in);
    if (fragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << fragmentShaderStream.rdbuf();
        fragmentShaderCode = sstr.str();
        fragmentShaderStream.close();
    } else {
        printf("Could not open %s.?\n", fragmentshader_path);
        getchar();
        return 0;
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    printf("Compiling shader : %s\n", vertexshader_path);
    char const* VertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(vertexShaderID);

    // Check for vertex shader errors
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
        printf("%s\n", &vertexShaderErrorMessage[0]);
    }

    // Compile fragment fhader
    printf("Compiling shader : %s\n", fragmentshader_path);
    char const* fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
    glCompileShader(fragmentShaderID);

    // Check for fragment shader errors
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
        printf("%s\n", &fragmentShaderErrorMessage[0]);
    }
    // Link the program
    printf("Linking program\n");
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    std::cout << "Shaders attached; Program created" << std::endl;
    return programID;
};

int initGLProgram(const char* programName) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open the glfw window and create the OpenGL context
    window = glfwCreateWindow(1024, 768, programName, NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW.\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    std::cout << "GLFW initialized" << std::endl;
    return 0;
}

// create the vertex buffer objects
void createVBOs(Geometry* geometry) {
    // array buffer for vertex position
    glGenBuffers(1, &vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(GLfloat), &geometry->vertices[0], GL_STATIC_DRAW);

    // array buffer for indices
    glGenBuffers(1, &vertexPositionIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexPositionIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indices.size() * sizeof(GLuint), &geometry->indices[0], GL_STATIC_DRAW);
    // error checking
    GLenum errorValue = glGetError();
    if (errorValue != GL_NO_ERROR) {
        fprintf(
            stderr,
            "ERROR: Could not create a VBO: %s \n",
            gluErrorString(errorValue));

        exit(-1);
    }
}

void createVAO() {
    // generate the vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void getWaveUniform(GLuint programID, GLuint& uWaveDirection, GLuint& uWaveLength, GLuint& uWaveSteepness) {
    uWaveLength = glGetUniformLocation(programID, "wave.waveLength");
    // uWaveSpeed = glGetUniformLocation(programID, "wave.speed");
    uWaveSteepness = glGetUniformLocation(programID, "wave.steepness");
    uWaveDirection = glGetUniformLocation(programID, "wave.direction");
    // uWaveAmplitude = glGetUniformLocation(programID, "wave.amplitude");
}

void bindWaveData(GLuint programID, std::vector<std::vector<GLfloat>> waves) {
    for (long unsigned int i = 0; i < waves.size(); i++) {
        GLuint uWaveDirection, uWaveLength, uWaveSteepness;

        std::string lengthLocName = "wave[" + std::to_string(i) + "].waveLength";  // name of the wave variables in the vertex shader
        std::string steepnessLocName = "wave[" + std::to_string(i) + "].steepness";
        std::string directionLocName = "wave[" + std::to_string(i) + "].direction";
        uWaveLength = glGetUniformLocation(programID, lengthLocName.c_str());
        uWaveSteepness = glGetUniformLocation(programID, steepnessLocName.c_str());
        uWaveDirection = glGetUniformLocation(programID, directionLocName.c_str());

        glUniform1f(uWaveLength, waves[i][0]);
        glUniform1f(uWaveSteepness, waves[i][1]);
        glUniform2fv(uWaveDirection, 1, &waves[i][2]);
    }
}

std::vector<std::vector<GLfloat>> createWaves() {
    // the number of waves here must match the number in the vertex shader
    std::vector<std::vector<GLfloat>> waves = std::vector<std::vector<GLfloat>>();
    std::vector<GLfloat> wave1 = {1.0f, 0.5f, 1.0f, 1.0f};  //(waveLength, steepness, direction.x, direction.z);
    waves.push_back(wave1);
    std::vector<GLfloat> wave2 = {3.0f, 0.2f, 1.0f, 0.0f};  //(waveLength, steepness, direction.x, direction.z);
    waves.push_back(wave2);
    std::vector<GLfloat> wave3 = {2.0f, 0.4f, 0.3f, 0.2f};  //(waveLength, steepness, direction.x, direction.z);
    waves.push_back(wave3);
    return waves;
}
// main render function
void render(PerspectiveCamera* camera, Geometry* geometry) {
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // changes to wireframe mode
    // white background
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    //std::cout << "Current working directory: " << tmp << std::endl;

    GLuint programID = loadShaders("../src/shaders/VertexShader.vs", "../src/shaders/FragmentShader.fs");
    // Use our shader
    glUseProgram(programID);

    std::vector<std::vector<GLfloat>> waves = createWaves();
    bindWaveData(programID, waves);
    GLuint uModelView = glGetUniformLocation(programID, "modelView");
    GLuint uProjection = glGetUniformLocation(programID, "projection");
    GLuint uNormalMatrix = glGetUniformLocation(programID, "normalMatrix");

    GLuint uTime = glGetUniformLocation(programID, "time");

    // uniform variables for wave calculations
    // GLuint uWaveAmplitude;
    // GLuint uWaveDirection, uWaveLength, uWaveSteepness;
    // getWaveUniform(programID, uWaveDirection, uWaveLength, uWaveSteepness);

    int frameNumber = 0;

    createVAO();
    createVBOs(geometry);
    do {
        frameNumber++;
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Send our transformation to the currently bound shader,
        glUniformMatrix4fv(uModelView, 1, GL_FALSE, &camera->getModelViewMat()[0][0]);  // TODO: potentially optimise this step
        glUniformMatrix4fv(uProjection, 1, GL_FALSE, &camera->getProjectionMatrix()[0][0]);
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, &camera->inverseTranspose()[0][0]);

        // Send time to the shaders
        glUniform1f(uTime, frameNumber * 0.01);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
        glVertexAttribPointer(
            0,         // attribute. No particular reason for 0, but must match the layout in the shader.
            3,         // size
            GL_FLOAT,  // type
            GL_FALSE,  // normalized?
            0,         // stride
            (void*)0   // array buffer offset
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexPositionIndexBuffer);
        //glDrawArrays(GL_TRIANGLES, 0, 12 * 3);  // 12*3 indices starting at 0 -> 12 triangles
        glDrawElements(GL_TRIANGLES, geometry->indices.size(), GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        GLenum errorValue = glGetError();
        if (errorValue != GL_NO_ERROR) {
            fprintf(
                stderr,
                "SHADER ERROR: %s \n",
                gluErrorString(errorValue));

            exit(-1);
        }
    }  // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexPositionBuffer);
    glDeleteBuffers(1, &vertexPositionIndexBuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VAO);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}
