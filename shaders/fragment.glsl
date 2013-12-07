#version 430 core

// enable early z culling
layout (early_fragment_tests) in;

// output color
layout (location = 0) out vec4 color;

// texture
layout (binding = 0) uniform sampler2D tex;

// input from vertex shader
in vec2 fTexcoord;
in vec3 fPosition;
in vec3 fNormal;

// lighting parameters
uniform vec3 lightpos;
uniform vec3 spotdir;
uniform float spotexponent;
uniform float lightintensity;

void main (void)
{
	// lighting calculations
	
	// obtain light direction and distance
	vec3 lightdir = lightpos - fPosition;
	float lightdist = length (lightdir);
	lightdir /= lightdist;
	
	// compute light intensity as the cosine of the angle
	// between light direction and normal direction
	float intensity = max (dot (lightdir, fNormal), 0);
	
	// apply distance attenuation and light intensity
	intensity /= lightdist * lightdist;
	intensity *= lightintensity;
	
	// spot light effect	
	float angle = dot (spotdir, - lightdir);
	intensity *= pow (angle, spotexponent);
	
	// ambient light1
	intensity += 0.1;

	// fetch texture value and output resulting color
	color = clamp (intensity, 0, 1) * texture (tex, fTexcoord);
}
