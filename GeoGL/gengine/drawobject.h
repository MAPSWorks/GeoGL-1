#pragma once

#include <map>

#include "primitivetypes.h"
//#include "textureunits.h"

namespace gengine {

	//forward declarations
	class RenderState;
	class Shader;
	class VertexData;
	//enum PrimitiveType : int;
	//enum class PrimitiveType;
	class Texture2D;

	/// <summary>
	/// We need to be able to return a null draw object for an Object3D which has no renderable geometry.
	/// This is a kludge to allow me to do that.
	/// Ideally, this would be removed.
	/// </summary>
	class NullDrawObject {
	};

	/// <summary>
	/// DrawObject wraps up everything needed to draw an object to a GraphicsContext. In other words, vertex buffers, shaders, attributes and render state
	/// Derives from a NullDrawObject for the reasons described above.
	/// </summary>
	class DrawObject : NullDrawObject {
	public:
		RenderState* _rs;
		Shader* _ShaderProgram;
		PrimitiveType _PrimType;
		VertexData* _vertexData;
		//TextureUnits* _texUnits;
		std::map<unsigned int,Texture2D*> _textures; //mapping between texture unit number and texture
		glm::mat4 _ModelMatrix;
	};

} //namespace gengine
