#version 150

/* texture fragment shader for globe */
/* globe texture frag 4.11 */

in vec3 worldPosition;
out vec4 fragmentColor;

uniform sampler2D u_texture0;
uniform vec3 u_globeOneOverRadiiSquared;

uniform float u_oneOverPi;
uniform float u_oneOverTwoPi;

vec3 GeodeticSurfaceNormal(vec3 positionOnEllipsoid, vec3 oneOverEllipsoidRadiiSquared)
{
	return normalize(positionOnEllipsoid*oneOverEllipsoidRadiiSquared);
}

vec2 ComputeTextureCoordinates(vec3 normal)
{
	return vec2(
		atan(normal.y, normal.x)*u_oneOverTwoPi + 0.5,
		asin(normal.z)*u_oneOverPi + 0.5);
	//new formula with poles in the right place
	//return vec2(
	//	atan(normal.x, normal.z)*u_oneOverTwoPi + 0.5,
	//	asin(normal.y)*u_oneOverPi + 0.5);
}

void main()
{
	vec3 normal = GeodeticSurfaceNormal(worldPosition, u_globeOneOverRadiiSquared);
	vec2 textureCoordinate = ComputeTextureCoordinates(normal);
	fragmentColor = vec4(texture(u_texture0, textureCoordinate).rgb,1.0);
}
