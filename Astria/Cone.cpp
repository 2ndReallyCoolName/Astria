#include "Cone.h"
#include "ConeVertices.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include <array>

Cone::Cone(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist, std::uniform_int_distribution<int>& longDist)
	: 
	ObjectBase(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;
	if (!IsStaticInitialized()) {

		auto pvs = std::make_unique<VertexShader>(gfx, L"BlendedPhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"BlendedPhongPS.cso"));

		
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant {
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} colorConst;

		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
	}

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
		std::array<char, 4> color;
		char padding;
	};

	int tessalations = longDist(rng);
	auto model = ConeVertices::MakeTesselatedIndependentFaces<Vertex>(tessalations);

	for (auto& vertex : model.vertices) {
		vertex.color = { (char)10,(char)10,(char)255 };
	}


	model.vertices[model.vertices.size()-1].color = { (char)255,(char)10,(char)10 };


	/*model.SetNormalsIndependentFlat();*/

	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}
