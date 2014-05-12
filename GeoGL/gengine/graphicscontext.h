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

#include "events/EventListener.h"

//forward references
struct GLFWwindow;

namespace gengine {

	//forward references
	//class RenderState;
	class DrawObject;
	class SceneDataObject;

	/// <summary>
	/// A graphics context is something we can render to and includes a window
	/// </summary>
	class GraphicsContext : public events::EventListener
	{
	public:
		GLFWwindow *window;

		GraphicsContext(GLFWwindow* pWindow);
		~GraphicsContext(void);

		void Clear(); //clear before rendering
		void Render(const DrawObject& obj, const SceneDataObject& sceneobj);
		void RenderFallback(const DrawObject& obj, const SceneDataObject& sceneobj);
		void SwapBuffers(void);

		virtual void WindowSizeCallback(GLFWwindow* window, int w, int h);
	};

} //namespace gengine
