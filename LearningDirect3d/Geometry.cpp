
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