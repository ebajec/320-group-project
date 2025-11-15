#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

// Final fragment color
out vec4 finalColor;

// raylib's default material uniforms:
uniform sampler2D texture0; // bound texture
uniform vec4 colDiffuse;    // material tint

void main()
{
	const int N = 36;
	float x = fragTexCoord.x * N;


	vec2 uv = vec2(fract(x),fragTexCoord.y); 

    vec4 texel = texture(texture0, fragTexCoord);
	//finalColor = texel * colDiffuse * fragColor;

	int ix = int(floor(x)); 

	if (ix == 0) { 
		finalColor = vec4(0,0.5,0,1);
	} else if ((ix % 2) == 0) {
		finalColor = vec4(0.5,0,0,1);
	} else {
		finalColor = vec4(0,0,0,1);
	}
	//finalColor = vec4(uv,0,1);
}
