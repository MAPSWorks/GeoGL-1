#pragma once
#include "main.h"
#include "gengine/graphicscontext.h"
#include "gengine/scenedataobject.h"
#include "gengine/drawobject.h"
#include "BBox.h"
#include <string>
#include <vector>

//forward references
namespace gengine {
	class GraphicsContext;
	class SceneDataObject;
	class DrawObject;
	class Shader;
}

//base class of all 3D objects in the world
//TODO: maybe pass the shader into the object so you can have different parts of the tree rendered with different shaders?
class Object3D {
public:
	std::string Name;
	BBox bounds; //bounding box

	//glm::mat4 modelMatrix; // Store the model matrix (promoted from protected)

	Object3D();
	~Object3D();
	void GetPos(double& X, double& Y, double& Z);
	void SetPos(double X, double Y, double Z);
	void Rotate(float Angle,glm::vec3 V);
	//void Render(unsigned int ShaderId, glm::mat4 ParentMat);
	//virtual void Render(glm::mat4 ParentMat);
	virtual void Render(gengine::GraphicsContext* GC, const gengine::SceneDataObject& sdo);
	virtual bool HasGeometry() { return false; } //no, this object has no geometry and cannot be rendered
	virtual const gengine::DrawObject& GetDrawObject(); //not implemented in this class
	void AddChild(Object3D* Child);
	void RemoveChild(Object3D* Child);
	void RemoveAllChildren();
	std::vector<Object3D*>::const_iterator BeginChild() { return Children.begin(); } //todo: this isn't exactly elegant!
	std::vector<Object3D*>::const_iterator EndChild() { return Children.end(); }
	virtual void AttachShader(gengine::Shader* pShader, bool Recursive);
	virtual BBox GetGeometryBounds();
	virtual void ComputeBounds();
	virtual void SetMatrix(const glm::mat4& matrix);

protected:
	glm::mat4 modelMatrix; // Store the model matrix

	//hierarchy
	std::vector<Object3D*> Children;

	gengine::NullDrawObject drawObject;
};
