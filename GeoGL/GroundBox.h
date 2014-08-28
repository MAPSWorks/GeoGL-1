#pragma once

//this builds and maintains a set of lat lon based boxes of geojson content that contain the buildings
//it follows the clipmap terrain idea of moving around the geometry that the viewpoint is over

#include "object3d.h"
#include "mesh2.h"

//forward references
namespace gengine {
	class Camera;
	class SceneDataObject;
	class GraphicsContext;
	class Shader;
}

struct BoxContent {
	bool IsEmpty;
	int TileX,TileY,TileZ;
	Object3D* mesh;
	BoxContent() { TileX=0; TileY=0; TileZ=0; mesh=nullptr; IsEmpty=true; };
};

class GroundBox : public Object3D {
private:
	BoxContent _gndboxes[9];
	Mesh2* DebugMesh(glm::dvec3 RadGeodetic3D);
protected:
	void ShuffleBoxes(const int TileZ, const int TileX, const int TileY);
	void UpdateData(const gengine::SceneDataObject& sdo);
public:
	gengine::Shader* _Shader; //shader used for rendering
	GroundBox();
	virtual ~GroundBox();

	void LonLatToTileXY(glm::dvec3 RadGeodetic3D,int& TileX,int& TileY);
	virtual void Render(gengine::GraphicsContext* GC,const gengine::SceneDataObject& sdo);
	virtual bool HasGeometry() { return true; } //yes, this object has geometry associated with it and can be rendered
};
