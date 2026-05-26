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

	// test shapes
	Transform cubeTransform  = { { 3, 0,  3 }, { 0, 0,   0, 1 }, { 1, 1, 1 } };
	Transform cubeTransform2 = { { 3, 0, -1 }, { 0, 1, 0.5, 1 }, { 1, 1, 1 } };
	Transform cubeTransform3 = { { 3, 0, -2 }, { 0, 0, 0.5, 1 }, { 1, 5, 1 } };
	Transform cubeTransform4 = { { 3, 0, -6 }, { 0, 0, 0.5, 1 }, { 2, 2, 1 } };

	size_t cube1 = AddObject(cubeMesh, cubeTransform);
	size_t cube2 = AddObject(cubeMesh, cubeTransform2);
	size_t cube3 = AddObject(cubeMesh, cubeTransform3);
	size_t cube4 = AddObject(cubeMesh, cubeTransform4);

	// cube at light source
	Transform cubeTransform5 = { lightCB.data.position, { 0, 0, 0, 1 }, { 0.1, 0.1, 0.1 } };
	cube5 = AddObject(cubeMesh, cubeTransform5);

	objects[cube5].transform.position = lightCB.data.position;
}
void Scene::Draw(Graphics& gfx, float aspectRatio) {
	lightCB.data.cameraPosition = camera.GetPosition();
	lightCB.Upload(gfx);
	for (SceneObject& obj : objects) {
		transformCB.data.world     = dx::XMMatrixTranspose(obj.transform.GetMatrix());
		transformCB.data.transform = dx::XMMatrixTranspose(obj.transform.GetMatrix() * camera.GetMatrix(aspectRatio));
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
size_t Scene::AddObject(Mesh* mesh, Transform transform) {
	SceneObject obj;

	obj.mesh = mesh;
	obj.transform = transform;

	objects.push_back(std::move(obj));

	return objects.size() - 1;
}