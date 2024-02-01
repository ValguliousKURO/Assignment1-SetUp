#version 330 core
//Version

//Gets the data at Attrib Index 0
//Converts it and stores it into a Vec3

layout(location = 0) in vec3 aPos; //if other index just change the location instead the 0 it will be you preferred index

//Creates a transform variable 
uniform mat4 transform;

void main()
{
    //Multiply the transformation matrix to the
    //vec4 version of aPos to get the final position
    gl_Position = transform * vec4(aPos, 1.0); 
}