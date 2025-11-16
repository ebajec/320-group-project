#version 330

// Input vertex attributes from raylib
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;

// MVP matrix from raylib (Model * View * Projection)
uniform mat4 mvp;
uniform mat4 matModel;

out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragPos;

void main()
{
    // Pass through attributes
    fragTexCoord = vertexTexCoord;
    fragColor    = vertexColor;
	fragPos = vec3(matModel * vec4(vertexPosition,1));

    // Transform vertex to clip space
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
