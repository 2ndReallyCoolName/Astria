#pragma once

#include "AstriaMath.h"
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class Cylinder
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv, int latDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3 && longDiv >= 2);

		const auto l_base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto u_base = dx::XMVectorSet(1.0f, 0.0f,  1.0f, 0.0f);

		const float longitudeAngle = 2.0f * PI / longDiv;

		// l_base vertices
		std::vector<V> vertices;
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				l_base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
		}


		// center rings
		auto base = l_base;

		for (int iLat = 0; iLat < latDiv - 2; iLat++) {
			base = dx::XMVector3Transform(base, dx::XMMatrixTranslation(0.0f, 0.0f, 2.0f / (latDiv - 1 )));
			for (int iLong = 0; iLong < longDiv; iLong++) {
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
		}


		// u_base vertices
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				u_base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
		}
		
		// the l center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,-1.0f };
		const auto ilCenter = (unsigned short)(vertices.size() - 1);

		// the u center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,1.0f };
		const auto iuCenter = (unsigned short)(vertices.size() - 1);


		// l base indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(ilCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}

		unsigned short end = longDiv + (latDiv-2)*longDiv;

		unsigned short n = 0;

		// cylinder indices
		for (unsigned short iLong = 0; iLong < end; iLong++)
		{
			if (iLong != 0 && iLong % longDiv == 0) {
				n += 1;
			}

			unsigned short factor = n * longDiv;

			indices.push_back(iLong);
			indices.push_back(((iLong + 1) % longDiv) + factor);
			indices.push_back(iLong+longDiv);

			indices.push_back(((iLong + 1) % longDiv) + factor);
			indices.push_back(((iLong + 1) % longDiv)+longDiv + factor);
			indices.push_back(iLong + longDiv);

		}

		// u base indices
		for (unsigned short iLong = end; iLong < longDiv+end; iLong++)
		{
			indices.push_back(iuCenter);
			indices.push_back(iLong);
			indices.push_back(((iLong + 1) % longDiv)+ end);
		}

		return { std::move(vertices),std::move(indices) };
	
	}

	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentCapNormals(int longDiv, int latDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3 && longDiv >= 2);

		const auto l_base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto u_base = dx::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);

		const float longitudeAngle = 2.0f * PI / longDiv;

		// l_base vertices
		std::vector<V> vertices;
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				l_base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
			vertices.back().n = { 0.0f,0.0f,-1.0f };
		}


		// center rings
		auto base = l_base;

		for (int iLat = 0; iLat < latDiv - 2; iLat++) {
			base = dx::XMVector3Transform(base, dx::XMMatrixTranslation(0.0f, 0.0f, 2.0f / (latDiv - 1)));
			for (int iLong = 0; iLong < longDiv; iLong++) {
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
				vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,0.0f };
			}
		}


		// u_base vertices
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				u_base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
			vertices.back().n = { 0.0f,0.0f, 1.0f };
		}

		// the l center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,-1.0f };
		vertices.back().n = { 0.0f,0.0f,-1.0f };
		const auto ilCenter = (unsigned short)(vertices.size() - 1);

		// the u center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,1.0f };
		vertices.back().n = { 0.0f,0.0f,1.0f };
		const auto iuCenter = (unsigned short)(vertices.size() - 1);


		// l base indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(ilCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}

		unsigned short end = longDiv + (latDiv - 2) * longDiv;

		unsigned short n = 0;

		// cylinder indices
		for (unsigned short iLong = 0; iLong < end; iLong++)
		{
			if (iLong != 0 && iLong % longDiv == 0) {
				n += 1;
			}

			unsigned short factor = n * longDiv;

			indices.push_back(iLong);
			indices.push_back(((iLong + 1) % longDiv) + factor);
			indices.push_back(iLong + longDiv);

			indices.push_back(((iLong + 1) % longDiv) + factor);
			indices.push_back(((iLong + 1) % longDiv) + longDiv + factor);
			indices.push_back(iLong + longDiv);

		}

		// u base indices
		for (unsigned short iLong = end; iLong < longDiv + end; iLong++)
		{
			indices.push_back(iuCenter);
			indices.push_back(iLong);
			indices.push_back(((iLong + 1) % longDiv) + end);
		}

		return { std::move(vertices),std::move(indices) };

	}


	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24, 4);
	}
};