
#include "Geometry.h"

void GeometryGenerator::generatorCylinder(MeshData& cylinder, float topRadius, float bottomRadius, float height, uint32 sliceCount, uint32 stackCount)
{
	for (uint32 i = 0; i < stackCount + 1; i++)
	{
		float dh = height / stackCount;
		float y = -height * 0.5f + dh * i;
		float dr = (topRadius - bottomRadius) / stackCount;
		float radius = bottomRadius + dr * i;
		float dTheta = 2 * MathHelper::Pi / sliceCount;
		for (uint32 j = 0; j <= sliceCount; j++)
		{
			float radian = dTheta * j;
			float c = cosf(radian);
			float s = sinf(radian);

			Vertex vert = Vertex();

			vert.Position = { radius * c, y, radius * s };
			vert.TangentU = { -s, 0, c };
			DirectX::XMFLOAT3 tangent = { radius * c, height / stackCount, radius * s };

			DirectX::XMVECTOR t1 = DirectX::XMLoadFloat3(&vert.TangentU);
			DirectX::XMVECTOR t2 = DirectX::XMLoadFloat3(&tangent);
			DirectX::XMStoreFloat3(&vert.Normal, DirectX::XMVector3Cross(t1, t2));

			vert.TexC = { float(j) / sliceCount, 1 - float(i) / stackCount };

			cylinder.Vertices.push_back(std::move(vert));
		}
	}

	for (uint32 i = 0; i < stackCount; i++)
	{
		for (uint32 j = 0; j < sliceCount; j++)
		{
			cylinder.Indices32.push_back(i * sliceCount + j);
			cylinder.Indices32.push_back(i * sliceCount + j + 1);
			cylinder.Indices32.push_back((i + 1) * sliceCount + j);

			cylinder.Indices32.push_back(i * sliceCount + j + 1);
			cylinder.Indices32.push_back((i + 1) * sliceCount + j);
			cylinder.Indices32.push_back((i + 1) * sliceCount + j + 1);
		}
	}

	generatorCircle(cylinder, bottomRadius, height, sliceCount, -1.0f);
	generatorCircle(cylinder, topRadius, height, sliceCount, 1.0f);
}

void GeometryGenerator::generatorCircle(MeshData& cylinder, float radius, float height, uint32 sliceCount, float normalY)
{
	uint32 base = uint32(cylinder.Vertices.size());
	float dTheta = 2 * MathHelper::Pi / sliceCount;
	float y = normalY * 0.5f * height;
	for (uint32 j = 0; j <= sliceCount; j++)
	{
		Vertex vert = Vertex();

		float radian = j * dTheta;
		float c = cosf(radian);
		float s = sinf(radian);

		vert.Position = { radius * c, y, radius * s };
		vert.TangentU = { -radius / s, y, radius / c };
		vert.Normal = { 0, normalY, 0 };
		vert.TexC = { radius * c / height + 0.5f, radius * s / height + 0.5f };
		cylinder.Vertices.push_back(std::move(vert));
	}
	Vertex vert = Vertex(0, y, 0, 0, 1, 0, 1, 0, 0, 0.5f, 0.5f);
	cylinder.Vertices.push_back(std::move(vert));
	uint32 center = uint32(cylinder.Vertices.size());

	for (uint32 j = 0; j < sliceCount; j++)
	{
		cylinder.Indices32.push_back(base + j);
		cylinder.Indices32.push_back(center);
		cylinder.Indices32.push_back(base + j + 1);
	}
}

void GeometryGenerator::subdivide(MeshData& meshData)
{
	auto indices = std::move(meshData.Indices32);
	meshData.Indices32 = std::vector<uint32>();
	uint32 size = indices.size();
	for (uint32 i = 0; i < size; i += 3) 
	{
		DirectX::XMVECTOR v0 = DirectX::XMLoadFloat3(&(meshData.Vertices.at(indices[i]).Position));
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&(meshData.Vertices.at(indices[i + 1]).Position));
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&(meshData.Vertices.at(indices[i + 2]).Position));

		DirectX::XMVECTOR m01 = DirectX::XMVectorScale(DirectX::XMVectorAdd(v0, v1), 0.5);
		DirectX::XMVECTOR m02 = DirectX::XMVectorScale(DirectX::XMVectorAdd(v0, v2), 0.5);
		DirectX::XMVECTOR m12 = DirectX::XMVectorScale(DirectX::XMVectorAdd(v1, v2), 0.5);

		Vertex vert01;
		DirectX::XMStoreFloat3(&vert01.Position, m01);
		meshData.Vertices.push_back(std::move(vert01));

		Vertex vert02;
		DirectX::XMStoreFloat3(&vert02.Position, m02);
		meshData.Vertices.push_back(std::move(vert02));

		Vertex vert12;
		DirectX::XMStoreFloat3(&vert12.Position, m12);
		meshData.Vertices.push_back(std::move(vert12));

		meshData.Indices32.push_back(indices[i]);
		meshData.Indices32.push_back(size + i + 1);
		meshData.Indices32.push_back(size + i);

		meshData.Indices32.push_back(size + i + 1);
		meshData.Indices32.push_back(indices[i + 2]);
		meshData.Indices32.push_back(size + i + 2);

		meshData.Indices32.push_back(size + i);
		meshData.Indices32.push_back(size + i + 2);
		meshData.Indices32.push_back(indices[i + 1]);

		meshData.Indices32.push_back(size + i);
		meshData.Indices32.push_back(size + i + 1);
		meshData.Indices32.push_back(size + i + 2);
	}

}

void GeometryGenerator::generatorGeosphere(MeshData& geosphere, float radius, uint32 numSubdivisions)
{
	const float X = 0.525731f;
	const float Z = 0.850651f;

	DirectX::XMFLOAT3 pos[12] =
	{
		DirectX::XMFLOAT3(-X, 0.0f, Z),  DirectX::XMFLOAT3(-X, 0.0f, Z),
		DirectX::XMFLOAT3(-X, 0.0f, -Z), DirectX::XMFLOAT3(X, 0.0f, -Z),
		DirectX::XMFLOAT3(0.0f, Z, X),   DirectX::XMFLOAT3(0.0f, Z, -X),
		DirectX::XMFLOAT3(0.0f, -Z, X),  DirectX::XMFLOAT3(0.0f, -Z, -X),
		DirectX::XMFLOAT3(Z, X, 0.0f),   DirectX::XMFLOAT3(-Z, X, 0.0f),
		DirectX::XMFLOAT3(Z, -X, 0.0f),  DirectX::XMFLOAT3(-Z, -X, 0.0f),
	};

	geosphere.Indices32 =
	{
		1, 4, 0,	4, 9, 0,	4, 5, 9,	8, 5, 4,	1, 8, 4,
		1, 10, 8,	10, 3, 8,	8, 3, 5,	3, 2, 5,	3, 7, 2,
		3, 10, 7,	10, 6, 7,	6, 11, 7,	6, 0, 11,	6, 1, 0,
		10, 1, 6,	11, 0, 9,	2, 11, 9,	5, 2, 9,	11, 2, 7
	};

	for (auto p : pos)
	{
		Vertex vect;
		vect.Position = p;
		geosphere.Vertices.push_back(std::move(vect));
	}

	for (uint32 i = 0; i < numSubdivisions; i++)
	{
		subdivide(geosphere);
	}
}