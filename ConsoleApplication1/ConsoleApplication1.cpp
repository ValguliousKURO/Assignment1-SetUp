#include<glad/glad.h>
#include<string>
#include<iostream>
#include <GLFW/glfw3.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "tiny_obj_loader.h"

//uniform mat4 transform;
//Modifier for the model's x Position
float x_mod = 0;
float y_mod = 0;
float theta_mod = 0;
float scale_mod = 1;

void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    /* //Template for action
    if (key == GLFW_KEY_D &&
        action == GLFW_PRESS) {
        //Move bunny to right
        x_mod += 0.01f;
        //std::cout << x_mod; check the values
    */
    if (key == GLFW_KEY_D) {
        //Move bunny to right
        x_mod += 0.01f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_A) {
        //Move bunny left
        x_mod -= 0.01f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_W) {
        //Move bunny up
        y_mod += 0.01f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_S) {
        //Move bunny down
        y_mod -= 0.01f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_Q) {
        //rotate
        theta_mod += 3.0f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_E) {
        //rotate
        theta_mod -= 3.0f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_R) {
        //skew
        scale_mod += 0.1f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_F) {
        //skew
        scale_mod -= 0.1f;
        //std::cout << x_mod; check the values
    }



}

int main(void)
{
    //GL_POSITION = transform * vec4(aPos, 1.0);
    glm::mat4 identity_matrix = glm::mat4(1.0f);
    float scale_x=1, scale_y=1, scale_z=0;
    float theta=90;
    float axis_x=0, axis_y=1, axis_z=0;
    float x=0, y=0, z=0;
    

    /*glm::mat4 translation =
        glm::translate(identity_matrix4,
            glm::vec3(x,
                y,
                z);*/

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Arvin Lawrence B. Dacanay", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();
    
    glfwSetKeyCallback(window, Key_Callback); //callback func to window 

    //Load the shader file into a string stream
    std::fstream vertSrc("Shaders/sample.vert"); 
    std::stringstream vertBuff; 
    //Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf(); 
    //Convert the stream to a character array 
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();  

    //Load the shader file into a string stream
    std::fstream fragSrc("Shaders/sample.frag"); 
    std::stringstream fragBuff; 
    //Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf(); 
    //Convert the stream to a character array
    std::string fragS = fragBuff.str(); 
    const char* f = fragS.c_str(); 

    //Create a Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); 
    glShaderSource(vertexShader, 1, &v, NULL); 
    glCompileShader(vertexShader); 
    GLint isCompiled = 0; 
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled); 
    if (isCompiled == GL_FALSE) 
    {
        GLint maxLength = 0; 
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength); 

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength); 
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]); 

        std::cout << &errorLog[0];  //show the error

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(vertexShader);  // Don't leak the shader.

    } 

    //Create a Fragment Shader 
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(fragShader, 1, &f, NULL);  
    glCompileShader(fragShader); 
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled); 
    if (isCompiled == GL_FALSE) 
    {
        GLint maxLength = 0; 
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength); 

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength); 
        glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]); 

        std::cout << &errorLog[0];  //show the error

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fragShader);  // Don't leak the shader.
        //return;
    }

    //Create the Shader Program
    GLuint shaderProg = glCreateProgram(); 
    //Attach the compiled Vertex Shader
    glAttachShader(shaderProg, vertexShader); 
    //Attach the compiled Fragment Shader
    glAttachShader(shaderProg, fragShader); 

    //Finalize the compilation process
    glLinkProgram(shaderProg); 


     
    std::string path = "3D/bunny.obj"; //path of model
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLfloat vertices[]
    {
        //x   y      z
        0.f, 0.5f, 0.f, //0
        -0.5f, -0.5f, 0.f, //1
        0.5f, - 0.5f, 0.f //2
        

       /*-1, 0, 0, //0
        0, 1, 0, //1
        1, 0, 0, //2
        2, 1 ,0 //3
        */
      
    };

    GLuint indices[]
    {
        0, 1, 2
    };

    //initialization of arrays and buffers
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO); 
    glGenBuffers(1, &EBO); 

    //Currently editing VAO = VAO
    glBindVertexArray(VAO);
    //Currentlu edotomh VBO = null 
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Binds VBO to VAO then 
        //Currently editing VBO = VBO
     
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(GL_FLOAT) * attributes.vertices.size(), //size of array
            attributes.vertices.data(), //array == &attributes.vertices[0]
            GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, //0 pos, 1 tex, 2 norms
        3, // xyz data
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // binds EBO to VAO, EBO = EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint)*mesh_indices.size(),
        mesh_indices.data(), 
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        x = x_mod; 
        y = y_mod; 
        theta = theta_mod; 
        scale_x = scale_mod;
        
        
        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix,
            glm::vec3(x, y, z)
        );

        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z)
        )
       
        );
        



        //Get the location of the transform variable in the shader
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        //Assign the matrix
        glUniformMatrix4fv(transformLoc,
            1,
            GL_FALSE, //transpose?
            glm::value_ptr(transformation_matrix)); //Pointer to the matrix

        //Tell open GL to use this foler
        //for the VAO/s below
        glUseProgram(shaderProg); 
        glBindVertexArray(VAO); 

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

/*
        glBegin(GL_POLYGON);
 
        glVertex2f(0.5f, 0.0f);
        glVertex2f(0.1548f, 0.4754f);
        glVertex2f(-0.4041f, 0.2944f);
        glVertex2f(-0.4051f, -0.2931f);
        glVertex2f(0.1533f, -0.4759f);
        
        glEnd();
*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    //De-initialization of variables
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file



/*

*/