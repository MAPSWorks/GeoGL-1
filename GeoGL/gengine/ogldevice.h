#pragma once

/**
 *	@program		GeoGL: Geographic Graphics Engine, part of MapTube (http://www.maptube.org)
 *	@description	A tool for building 3D representations from geographic data, animations over time and a 3D GIS.
 *	@author			Richard Milton
 *	@organisation	Centre for Advanced Spatial Analysis (CASA), University College London
 *	@website		http://www.casa.ucl.ac.uk
 *	@date			9 May 2014
 *	@licence		Free for all commercial and academic use (including teaching). Not for commercial use. All rights are held by the author.
 */

#include "gengine.h"
#include "renderstate.h"

namespace gengine {

	//forward declarations
	class Shader;
	class GraphicsContext;
	class VertexArrayObject;
	class VertexBuffer;
	class IndexBuffer;
	enum class BufferTarget;
	enum class BufferUsage;

	/// <summary>
	/// All functions which are part of the GPU Hardware
	/// </summary>
	class OGLDevice
	{
	public:
		//OGLDevice(void);
		//~OGLDevice(void);

		static bool hasProgrammableShaders; //set to false for OpenGL1 fallback
		static int VersionMajor; //OpenGL version numbers
		static int VersionMinor;

		static void Initialise(); //initialises glfw
		static void Destroy();

		static GraphicsContext* XCreateWindow(int Width,int Height);
		static void CreateStereoWindow();
		static Shader* CreateShaderProgram(std::string VertexFilename, std::string FragmentFilename);
		//and buffer creation
		static VertexBuffer* CreateVertexBuffer(const std::string& AttributeName, BufferTarget Target, BufferUsage Usage, unsigned int NumBytes);
		static IndexBuffer* CreateIndexBuffer(BufferTarget Target, BufferUsage Usage, unsigned int NumBytes);

		static void SetRenderState(const RenderState& rs);
	private:
		static RenderState CurrentRenderState; //current state of device globals
	};

} //namespace gengine
