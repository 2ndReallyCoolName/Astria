#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "AstriaMath.h"


class ConeVertices
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		// base vertices
		std::vector<V> vertices;
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
		}
		// the center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,-1.0f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);
		// the tip :darkness:
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,1.0f };
		const auto iTip = (unsigned short)(vertices.size() - 1);


		// base indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(iCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}

		// cone indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(iLong);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iTip);
		}

		return { std::move(vertices),std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentFaces(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		// base vertices
		std::vector<V> vertices;
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
			vertices.back().n = { 0.0f,0.0f,-1.0f };
		}

		////duplicate base for outwards normals
		const auto iBase2 = (unsigned short)(vertices.size());
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
			vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,0.0f };
		}

		// the center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,-1.0f };
		vertices.back().n = { 0.0f,0.0f,-1.0f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);
		// the tip :darkness:
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,1.0f };
		vertices.back().n = { 0.0f,0.0f,1.0f };
		const auto iTip = (unsigned short)(vertices.size() - 1);


		// base indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(iCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}

		// cone indices
		for (unsigned short iLong = iBase2; iLong < longDiv+ iBase2; iLong++)
		{
			indices.push_back(iLong);
			indices.push_back(((iLong + 1) % longDiv)+iBase2);
			indices.push_back(iTip);
		}

		return { std::move(vertices),std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentTextureFaces(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		const auto txc_center = dx::XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f);
		const auto txc_base = dx::XMVectorSet(1, 0.5f, 0.0f, 0.0f);

		// base vertices
		std::vector<V> vertices;
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);

			auto t = dx::XMVectorAdd(
				dx::XMVector2Transform(dx::XMVectorSubtract(txc_base, txc_center), dx::XMMatrixRotationZ(longitudeAngle * iLong)),
				txc_center);

			vertices.back().n = { 0.0f,0.0f,-1.0f };
			dx::XMStoreFloat2(&vertices.back().tc, t);
		}

		////duplicate base for outwards normals
		const auto iBase2 = (unsigned short)(vertices.size());
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);

			auto t = dx::XMVectorAdd(
				dx::XMVector2Transform(dx::XMVectorSubtract(txc_base, txc_center), dx::XMMatrixRotationZ(longitudeAngle * iLong)),
				txc_center);

			vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,0.0f };
			dx::XMStoreFloat2(&vertices.back().tc, t);
		}

		// the center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,-1.0f };
		vertices.back().n = { 0.0f,0.0f,-1.0f };
		vertices.back().tc = { 0.5f, 0.5f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);
		// the tip :darkness:
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,1.0f };
		vertices.back().n = { 0.0f,0.0f,1.0f };
		vertices.back().tc = { 0.5f, 0.5f };
		const auto iTip = (unsigned short)(vertices.size() - 1);


		// base indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(iCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}

		// cone indices
		for (unsigned short iLong = iBase2; iLong < longDiv + iBase2; iLong++)
		{
			indices.push_back(iLong);
			indices.push_back(((iLong + 1) % longDiv) + iBase2);
			indices.push_back(iTip);
		}

		return { std::move(vertices),std::move(indices) };
	}


	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};