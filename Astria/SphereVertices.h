#pragma once

#include "AstriaMath.h"
#include "IndexedTriangleList.h"
#include <DirectXMath.h>

class SphereVertices
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv, int latDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 2 && longDiv >= 1);

		const auto l_tip = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto u_tip = dx::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);

		const float longitudeAngle = 2.0f * PI / longDiv;
		const float latitudeAngle = PI / latDiv;

		auto base = dx::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

		std::vector<V> vertices;

		for (int iLat = 1; iLat < latDiv; iLat ++) {

			base = dx::XMVector3Transform(base, dx::XMMatrixRotationY(latitudeAngle));

			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
		}

		// the l tip
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f};
		const auto ilCenter = (unsigned short)(vertices.size() - 1);

		// the u tip
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f, 1.0f };
		const auto iuCenter = (unsigned short)(vertices.size() - 1);


		// l tip indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(ilCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}


		unsigned short end = longDiv * (latDiv - 2);
		unsigned short n = 0;

		// Sphere indices
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

		// u tip indices
		for (unsigned short iLong = end; iLong < longDiv + end; iLong++)
		{
			indices.push_back(iuCenter);
			indices.push_back(iLong);
			indices.push_back(((iLong + 1) % longDiv) + end);
		}

		return { std::move(vertices),std::move(indices) };

	}


	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentCapNormals(int longDiv, int latDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 2 && longDiv >= 1);

		const auto l_tip = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto u_tip = dx::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);

		const float longitudeAngle = 2.0f * PI / longDiv;
		const float latitudeAngle = PI / latDiv;

		auto base = dx::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

		std::vector<V> vertices;

		for (int iLat = 1; iLat < latDiv; iLat++) {

			base = dx::XMVector3Transform(base, dx::XMMatrixRotationY(latitudeAngle));

			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
				vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,vertices.back().pos.z };
			}
		}

		// the l tip
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		vertices.back().n = { 0.0f,0.0f, -1.0f };
		const auto ilCenter = (unsigned short)(vertices.size() - 1);

		// the u tip
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f, 1.0f };
		vertices.back().n = { 0.0f,0.0f, 1.0f };
		const auto iuCenter = (unsigned short)(vertices.size() - 1);


		// l tip indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(ilCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}


		unsigned short end = longDiv * (latDiv - 2);
		unsigned short n = 0;

		// Sphere indices
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

		// u tip indices
		for (unsigned short iLong = end; iLong < longDiv + end; iLong++)
		{
			indices.push_back(iuCenter);
			indices.push_back(iLong);
			indices.push_back(((iLong + 1) % longDiv) + end);
		}

		return { std::move(vertices),std::move(indices) };

	}

	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentTextureCapNormals(int longDiv, int latDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 2 && longDiv >= 1);

		const auto l_tip = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto u_tip = dx::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);

		const float longitudeAngle = 2.0f * PI / longDiv;
		const float latitudeAngle = PI / latDiv;

		auto base = dx::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

		const auto txc_center = dx::XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f);
		const auto txc_base = dx::XMVectorSet(1, 0.5f, 0.0f, 0.0f);

		std::vector<V> vertices;

		//l_base ring
		base = dx::XMVector3Transform(base, dx::XMMatrixRotationY(latitudeAngle));

		for (int iLong = 0; iLong < longDiv; iLong++)
		{

			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
			
			vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,vertices.back().pos.z };
		
			auto t = dx::XMVectorAdd(
				dx::XMVector2Transform(dx::XMVectorSubtract(txc_base, txc_center), dx::XMMatrixRotationZ(longitudeAngle * iLong)),
				txc_center);

			dx::XMStoreFloat2(&vertices.back().tc, t);
		}


		for (int iLat = 2; iLat < latDiv-1; iLat++) {

			base = dx::XMVector3Transform(base, dx::XMMatrixRotationY(latitudeAngle));

			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
				vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,vertices.back().pos.z };
				vertices.back().tc = { iLong / (longDiv * 1.0f), iLat / (longDiv * 1.0f) };
			}
		}

		//u_base ring
		base = dx::XMVector3Transform(base, dx::XMMatrixRotationY(latitudeAngle));

		for (int iLong = 0; iLong < longDiv; iLong++)
		{

			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);

			vertices.back().n = { vertices.back().pos.x,vertices.back().pos.y,vertices.back().pos.z };

			auto t = dx::XMVectorAdd(
				dx::XMVector2Transform(dx::XMVectorSubtract(txc_base, txc_center), dx::XMMatrixRotationZ(longitudeAngle * iLong)),
				txc_center);

			dx::XMStoreFloat2(&vertices.back().tc, t);
		}

		// the l tip
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		vertices.back().n = { 0.0f,0.0f, -1.0f };
		vertices.back().tc = { 0.5f, 0.5f };
		const auto ilCenter = (unsigned short)(vertices.size() - 1);

		// the u tip
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f, 1.0f };
		vertices.back().n = { 0.0f,0.0f, 1.0f };
		vertices.back().tc = { 0.5f, 0.5f };
		const auto iuCenter = (unsigned short)(vertices.size() - 1);


		// l tip indices
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			indices.push_back(ilCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}


		unsigned short end = longDiv * (latDiv - 2);
		unsigned short n = 0;

		// Sphere indices
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

		// u tip indices
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
		return MakeTesselated<V>(24, 24);
	}
};