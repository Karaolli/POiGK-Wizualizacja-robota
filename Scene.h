#pragma once

#include <vector>

#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "ConstantBuffer.h"

struct SceneObject {
	Mesh*     mesh;
	Transform transform;
	void Draw(Graphics& gfx) { // Podlączenie i rysowanie
		mesh->Bind(gfx);
		mesh->Draw(gfx);
	}
};

class Scene {
public:
	Scene(Graphics& gfx);
	void Draw(Graphics& gfx, Camera& camera); // Aktualizacja buforów stałych, rysowanie wszystkich obiektów
	size_t AddObject(Mesh* mesh, const Transform& transform);// Dodanie obiektu do sceny, zwracanie jego indeksu w wektorze wszystkich obiektów

	std::vector<SceneObject> objects;
	Mesh* cubeMesh;
private:
	Mesh* LoadMesh(ID3D11Device* device, // Ładowanie siatki do wektora wszystkich siatek, zwracanie wskaźnika do dodanej siatki
		Vertex* vertices, UINT vertexCount,
		unsigned short* indices, UINT indexCount);

	ConstantBuffer<TransformData> transformCB;
	ConstantBuffer<LightData>     lightCB;
	std::vector<Mesh> meshes;
	size_t cube5;
};