#pragma once

#include <vector>

#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "ConstantBuffer.h"

struct SceneObject {
	Mesh*     mesh;
	Transform transform;
	void Draw(Graphics& gfx) {
		mesh->Bind(gfx);
		mesh->Draw(gfx);
	}
};

class Scene {
public:
	Scene(Graphics& gfx);
	void Draw(Graphics& gfx, float aspectRatio);
	size_t AddObject(Mesh* mesh,
		Transform transform);

	std::vector<SceneObject> objects;
	Camera camera;
	Mesh* cubeMesh;
private:
	Mesh* LoadMesh(ID3D11Device* device,
		Vertex* vertices, UINT vertexCount,
		unsigned short* indices, UINT indexCount);

	ConstantBuffer<TransformData> transformCB;
	ConstantBuffer<LightData>     lightCB;
	std::vector<Mesh> meshes;
	size_t cube5;
};