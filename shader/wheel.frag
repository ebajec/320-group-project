#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragPos;

out vec4 finalColor;

uniform sampler2D texture0; // bound texture
uniform vec4 colDiffuse;    // material tint
uniform mat4 matView;       // view matrix

vec4 lighting(vec4 base, vec3 N, vec3 P)
{

	vec3 O = vec3(0,0,2);

	vec3 r = O - P;

	vec3 L = normalize(r);
	float f_diffuse = clamp(dot(L, N),0,1);

	vec3 V = matView[2].xyz;
	vec3 R = reflect(-L,N);
	float f_spec = 0.4*pow(max(dot(V,R),0),64);

	vec3 color = mix(base.xyz * f_diffuse,vec3(1),f_spec); 

	return vec4(color, 1);
}

void main()
{
	const int N = 36;
	float x = fragTexCoord.x * N;

	vec3 del_x = dFdx(fragPos);
	vec3 del_y = dFdy(fragPos);
	vec3 n = normalize(cross(del_x, del_y));

	vec2 uv = vec2(fract(x),fragTexCoord.y); 

    vec4 tex = texture(texture0, fragTexCoord);

	int ix = int(floor(x)); 

	if (ix == 0) { 
		finalColor = vec4(0,0.5,0,1);
	} else if ((ix % 2) == 0) {
		finalColor = vec4(0.5,0,0,1);
	} else {
		finalColor = vec4(0,0,0,1);
	}
	finalColor = lighting(finalColor, n, fragPos);
}
