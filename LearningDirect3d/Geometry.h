#pragma once

#include "Common/d3dApp.h"

using uint32 = std::uint32_t;
using uint16 = std::uint16_t;

struct Vertex
{
    Vertex() {}
    Vertex(
        const DirectX::XMFLOAT3& p,
        const DirectX::XMFLOAT3& n,
        const DirectX::XMFLOAT3& t,
        const DirectX::XMFLOAT2& uv
    ) : Position(p),
        Normal(n),
        TangentU(t),
        TexC(uv) {}

    Vertex(
        float px, float py, float pz,
        float nx, float ny, float nz,
        float tx, float ty, float tz,
        float u, float v
    ) : Position(px, py, pz),
        Normal(nx, ny, nz),
        TangentU(tx, ty, tz),
        TexC(u, v) {}


    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT3 TangentU;
    DirectX::XMFLOAT2 TexC;
};

struct MeshData
{
    std::vector<Vertex> Vertices;
    std::vector<std::uint32_t> Indices32;
};


class GeometryGenerator
{
public:
    static void generatorCylinder(MeshData& cylinder, float topRadius, float bottomRadius, float height, uint32 sliceCount, uint32 stackCount);
    static void generatorCircle(MeshData& cylinder, float radius, float height, uint32 sliceCount, float normalY);
    static void generatorGeosphere(MeshData& geosphere, float radius, uint32 numSubdivisions);
    static void subdivide(MeshData& meshData);
};