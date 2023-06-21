
#include "Geometry.h"

void GeometryGenerator::generatorCylinder(MeshData cylinder, float topRadius, float bottomRadius, float height, uint32 sliceCount, uint32 stackCount)
{
	
	for (uint32 i = 0; i < stackCount + 1; i++)
	{
		float dh = height / stackCount;
		float y = -height * 0.5 + dh * i;
		float dr = (topRadius - bottomRadius) / stackCount;
		float radius = bottomRadius + dr * i;
		for (uint32 j = 0; j < sliceCount; j++)
		{
			float radian = 2 * MathHelper::Pi * j / sliceCount;
			float c = cosf(radian);
			float s = sinf(radian);
			DirectX::XMFLOAT3 position(radius * c, y, radius * s);
			DirectX::XMFLOAT3 tangentU(-radius / s)
		}
	}
}

