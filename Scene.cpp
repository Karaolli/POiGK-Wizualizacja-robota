#include "Scene.h"

Vertex cubeVertices[] = {
	{ dx::XMFLOAT3(-0.5f, -0.5f, -0.5f), dx::XMFLOAT3(-1.0f,  0.0f,  0.0f) },
	{ dx::XMFLOAT3(-0.5f, -0.5f, -0.5f), dx::XMFLOAT3(0.0f, -1.0f,  0.0f) },
	{ dx::XMFLOAT3(-0.5f, -0.5f, -0.5f), dx::XMFLOAT3(0.0f,  0.0f, -1.0f) },

	{ dx::XMFLOAT3(-0.5f, -0.5f,  0.5f), dx::XMFLOAT3(-1.0f,  0.0f,  0.0f) },
	{ dx::XMFLOAT3(-0.5f, -0.5f,  0.5f), dx::XMFLOAT3(0.0f, -1.0f,  0.0f) },
	{ dx::XMFLOAT3(-0.5f, -0.5f,  0.5f), dx::XMFLOAT3(0.0f,  0.0f,  1.0f) },

	{ dx::XMFLOAT3(-0.5f,  0.5f, -0.5f), dx::XMFLOAT3(-1.0f,  0.0f,  0.0f) },
	{ dx::XMFLOAT3(-0.5f,  0.5f, -0.5f), dx::XMFLOAT3(0.0f,  1.0f,  0.0f) },
	{ dx::XMFLOAT3(-0.5f,  0.5f, -0.5f), dx::XMFLOAT3(0.0f,  0.0f, -1.0f) },

	{ dx::XMFLOAT3(-0.5f,  0.5f,  0.5f), dx::XMFLOAT3(-1.0f,  0.0f,  0.0f) },
	{ dx::XMFLOAT3(-0.5f,  0.5f,  0.5f), dx::XMFLOAT3(0.0f,  1.0f,  0.0f) },
	{ dx::XMFLOAT3(-0.5f,  0.5f,  0.5f), dx::XMFLOAT3(0.0f,  0.0f,  1.0f) },

	{ dx::XMFLOAT3(0.5f, -0.5f, -0.5f), dx::XMFLOAT3(1.0f,  0.0f,  0.0f) },
	{ dx::XMFLOAT3(0.5f, -0.5f, -0.5f), dx::XMFLOAT3(0.0f, -1.0f,  0.0f) },
	{ dx::XMFLOAT3(0.5f, -0.5f, -0.5f), dx::XMFLOAT3(0.0f,  0.0f, -1.0f) },

	{ dx::XMFLOAT3(0.5f, -0.5f,  0.5f), dx::XMFLOAT3(1.0f,  0.0f,  0.0f) },
	{ dx::XMFLOAT3(0.5f, -0.5f,  0.5f), dx::XMFLOAT3(0.0f, -1.0f,  0.0f) },
	{ dx::XMFLOAT3(0.5f, -0.5f,  0.5f), dx::XMFLOAT3(0.0f,  0.0f,  1.0f) },

	{ dx::XMFLOAT3(0.5f,  0.5f, -0.5f), dx::XMFLOAT3(1.0f,  0.0f,  0.0f) },
	{ dx::XMFLOAT3(0.5f,  0.5f, -0.5f), dx::XMFLOAT3(0.0f,  1.0f,  0.0f) },
	{ dx::XMFLOAT3(0.5f,  0.5f, -0.5f), dx::XMFLOAT3(0.0f,  0.0f, -1.0f) },

	{ dx::XMFLOAT3(0.5f,  0.5f,  0.5f), dx::XMFLOAT3(1.0f,  0.0f,  0.0f) },
	{ dx::XMFLOAT3(0.5f,  0.5f,  0.5f), dx::XMFLOAT3(0.0f,  1.0f,  0.0f) },
	{ dx::XMFLOAT3(0.5f,  0.5f,  0.5f), dx::XMFLOAT3(0.0f,  0.0f,  1.0f) }
};

unsigned short cubeIndices[] =
{
	 2,  8, 14,
	 8, 20, 14,
	 7, 10, 19,
	10, 22, 19,
	11,  5, 23,
	 5, 17, 23,
	15, 18, 21,
	15, 12, 18,
	16,  4, 13,
	13,  4,  1,
	 3,  9,  0,
	 9,  6,  0
};


Scene::Scene(Graphics& gfx)
	: transformCB(gfx)
	, lightCB(gfx)
{
	meshes.reserve(64);
	transformCB.BindVS(gfx, 0u);
	lightCB.BindPS(gfx, 1u);

	ID3D11Device* device = gfx.GetDevice();
	cubeMesh = LoadMesh(device,
		cubeVertices, (UINT)std::size(cubeVertices),
		cubeIndices,  (UINT)std::size(cubeIndices));

	lightCB.data.position = { 5, 10, 5 };
}
void Scene::Draw(Graphics& gfx, Camera& camera) {
	gfx.SetViewport(camera.GetViewport());
	lightCB.data.cameraPosition = camera.GetPosition();
	lightCB.Upload(gfx);
	for (SceneObject& obj : objects) {
		transformCB.data.world     = dx::XMMatrixTranspose(obj.transform.GetMatrix());
		transformCB.data.transform = dx::XMMatrixTranspose(obj.transform.GetMatrix() * camera.GetMatrix(gfx));
		transformCB.Upload(gfx);
		obj.Draw(gfx);
	}
}
Mesh* Scene::LoadMesh(ID3D11Device* device,
	Vertex* vertices, UINT vertexCount,
	unsigned short* indices, UINT indexCount) {
	meshes.emplace_back();
	meshes.back().Load(device, vertices, vertexCount, indices, indexCount);
	return &meshes.back();
}
size_t Scene::AddObject(Mesh* mesh, const Transform& transform) {
	SceneObject obj;

	obj.mesh = mesh;
	obj.transform = transform;

	objects.push_back(std::move(obj));

	return objects.size() - 1;
}