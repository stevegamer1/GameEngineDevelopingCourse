#include <array.h>
#include <D3D12Mesh.h>
#include <D3D12RHI.h>
#include <D3D12RHIPrivate.h>
#include <D3D12Material.h>
#include <Math/Vector.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		using namespace Core;

		D3D12RHI::D3D12RHI() :
			m_d3d12Private(std::make_shared<D3D12RHIPrivate>())
		{
			m_d3d12Private->EnableDebugLayer();
		}

		void D3D12RHI::Init()
		{
			m_d3d12Private->Init();
		}

		void D3D12RHI::Update(Mesh::Ptr mesh, Material::Ptr material)
		{
			m_d3d12Private->Update(mesh, material);
		}

		Mesh::Ptr D3D12RHI::CreateBoxMesh()
		{
			array<Vertex, 8> vertices =
			{
				Vertex({ Math::Vector3f(-1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::White) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Black) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Green) }),
				Vertex({ Math::Vector3f(-1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Blue) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Yellow) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Cyan) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Magenta) })
			};

			array<uint16_t, 36> indices =
			{
				// front face
				0, 1, 2,
				0, 2, 3,

				// back face
				4, 6, 5,
				4, 7, 6,

				// left face
				4, 5, 1,
				4, 1, 0,

				// right face
				3, 2, 6,
				3, 6, 7,

				// top face
				1, 5, 6,
				1, 6, 2,

				// bottom face
				4, 0, 3,
				4, 3, 7
			};

			return m_d3d12Private->CreateMesh(vertices.begin(), vertices.size(), sizeof(Vertex), indices.begin(), indices.size(), sizeof(uint16_t));
		}

<<<<<<< HEAD
		Math::Vector3f D3D12RHI::FindKnee(Math::Vector3f shoulder, Math::Vector3f foot, float elbow_length) {
		  Math::Vector3f leg_vec = foot - shoulder;
		  Math::Vector3f flat_leg_vec{leg_vec.x, 0.0f, leg_vec.z};
		  Math::Vector3f shoulder_axis = flat_leg_vec.CrossProduct(Math::Vector3f{ 0.0f, 1.0f, 0.0f });
		  Math::Vector3f knee_normal = shoulder_axis.CrossProduct(leg_vec).Normalized();
		  if (knee_normal * Math::Vector3f(0.0f, 1.0f, 0.0f) < 0.0f) {
			knee_normal = -knee_normal;
		  }

		  float half_leg_length = leg_vec.GetLength() * 0.5f;
		  float knee_normal_scalar_sqr = elbow_length * elbow_length - half_leg_length * half_leg_length;
		  knee_normal_scalar_sqr = max(knee_normal_scalar_sqr, 0.0f);
		  return shoulder + leg_vec * 0.5f + knee_normal * sqrt(knee_normal_scalar_sqr);
		}

		std::array<Math::Vector3f, 3> D3D12RHI::MakeKneeRing(Math::Vector3f shoulder, Math::Vector3f knee, Math::Vector3f foot, float radius) {
		  Math::Vector3f leg_vec = foot - shoulder;
		  Math::Vector3f leg_vec_center = leg_vec * 0.5f;
		  Math::Vector3f v1 = (knee - leg_vec_center).Normalized();
		  Math::Vector3f v2 = leg_vec.CrossProduct(v1).Normalized();

		  return {
			knee + v1 * radius,
			knee + -v2 * radius * 0.75f - v1 * radius * 0.5f,
			knee + v2 * radius * 0.75f - v1 * radius * 0.5f
		  };
		}

		Mesh::Ptr D3D12RHI::CreateSpiderMesh()
		{
		  float body_size = 1.0f;
		  float knee_size = 1.0f;
		  float knee_thickness = 0.1f;
		  Math::Vector3f center{ 0.0f, 0.0f, 0.0f };

		  Math::Vector3f tip = center + Math::Vector3f{ 0.0f, 1.0f, 0.0f } * body_size;
		  Math::Vector3f front_left_shoulder = center + Math::Vector3f{-0.75f, -0.5f, 0.5f} * body_size;
		  Math::Vector3f front_right_shoulder = center + Math::Vector3f{ +0.75f, -0.5f, 0.5f } * body_size;
		  Math::Vector3f back_shoulder = center + Math::Vector3f{ 0.0f, -0.5f, -0.75f } * body_size;

		  Math::Vector3f front_left_foot = center + Math::Vector3f{ -1.25f, -1.5f, 0.75f } * body_size;
		  Math::Vector3f front_right_foot = center + Math::Vector3f{ +1.25f, -1.5f, 0.75f } * body_size;
		  Math::Vector3f back_foot = center + Math::Vector3f{ 0.0f, -1.5f, -1.25f } * body_size;

		  Math::Vector3f front_left_knee = FindKnee(front_left_shoulder, front_left_foot, knee_size);
		  Math::Vector3f front_right_knee = FindKnee(front_right_shoulder, front_right_foot, knee_size);
		  Math::Vector3f back_knee = FindKnee(back_shoulder, back_foot, knee_size);

		  std::array<Math::Vector3f, 3> front_left_knee_ring = MakeKneeRing(front_left_shoulder, front_left_knee, front_left_foot, knee_thickness);
		  std::array<Math::Vector3f, 3> front_right_knee_ring = MakeKneeRing(front_right_shoulder, front_right_knee, front_right_foot, knee_thickness);
		  std::array<Math::Vector3f, 3> back_knee_ring = MakeKneeRing(back_shoulder, back_knee, back_foot, knee_thickness);

		  array<Vertex, 16> vertices =
		  {
			  Vertex({ tip, Math::Vector4f((float*)&DirectX::Colors::Yellow)}),

			  Vertex({ front_left_shoulder, Math::Vector4f((float*)&DirectX::Colors::Red)}),
			  Vertex({ front_right_shoulder, Math::Vector4f((float*)&DirectX::Colors::Green)}),
			  Vertex({ back_shoulder, Math::Vector4f((float*)&DirectX::Colors::Blue)}),

			  Vertex({ front_left_foot, Math::Vector4f((float*)&DirectX::Colors::White)}),
			  Vertex({ front_right_foot, Math::Vector4f((float*)&DirectX::Colors::White)}),
			  Vertex({ back_foot, Math::Vector4f((float*)&DirectX::Colors::White)}),

			  Vertex({ front_left_knee_ring[0], Math::Vector4f((float*)&DirectX::Colors::Magenta)}),
			  Vertex({ front_left_knee_ring[1], Math::Vector4f((float*)&DirectX::Colors::Black)}),
			  Vertex({ front_left_knee_ring[2], Math::Vector4f((float*)&DirectX::Colors::Cyan)}),

			  Vertex({ front_right_knee_ring[0], Math::Vector4f((float*)&DirectX::Colors::Magenta)}),
			  Vertex({ front_right_knee_ring[1], Math::Vector4f((float*)&DirectX::Colors::Black)}),
			  Vertex({ front_right_knee_ring[2], Math::Vector4f((float*)&DirectX::Colors::Cyan)}),

			  Vertex({ back_knee_ring[0], Math::Vector4f((float*)&DirectX::Colors::Magenta)}),
			  Vertex({ back_knee_ring[1], Math::Vector4f((float*)&DirectX::Colors::Black)}),
			  Vertex({ back_knee_ring[2], Math::Vector4f((float*)&DirectX::Colors::Cyan)})
		  };

		  array<uint16_t, 66> indices =
		  {
			// body
			// front face
			0, 1, 2,
			// back left face
			0, 2, 3,
			// back right face
			0, 3, 1,
			// bottom face
			3, 2, 1,

			// front left leg
			// first part
			1, 7, 8,
			1, 8, 9,
			1, 9, 7,
			// second part
			4, 8, 7,
			4, 9, 8,
			4, 7, 9,

			// front right leg
			// first part
			2, 10, 11,
			2, 11, 12,
			2, 12, 10,
			// second part
			5, 11, 10,
			5, 12, 11,
			5, 10, 12,

			// back right leg
			// first part
			3, 13, 14,
			3, 14, 15,
			3, 15, 13,
			// second part
			6, 14, 13,
			6, 15, 14,
			6, 13, 15
		  };

		  return m_d3d12Private->CreateMesh(vertices.begin(), vertices.size(), sizeof(Vertex), indices.begin(), indices.size(), sizeof(uint16_t));
=======
		Mesh::Ptr D3D12RHI::CreatePyramidMesh()
		{
			array<Vertex, 5> vertices =
			{
				Vertex({ Math::Vector3f( 0.0f, +1.0f,  0.0f), Math::Vector4f((float*)&DirectX::Colors::White) }),
				Vertex({ Math::Vector3f(-1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Yellow) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Green) }),
				Vertex({ Math::Vector3f(-1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Blue) })
			};

			array<uint16_t, 18> indices =
			{
				// front face
				0, 2, 1,

				// right face
				0, 3, 2,

				// back face
				0, 4, 3,

				// left face
				0, 1, 4,

				// bottom face
				1, 2, 4,
				2, 3, 4
			};

			return m_d3d12Private->CreateMesh(vertices.begin(), vertices.size(), sizeof(Vertex), indices.begin(), indices.size(), sizeof(uint16_t));
>>>>>>> bc7185de889fae0a1f6726b52a3dd4e2d783c1aa
		}

		Material::Ptr D3D12RHI::GetMaterial(const std::string& name)
		{
			return m_d3d12Private->GetMaterial(name);
		}

		void D3D12RHI::ExecuteCommandLists()
		{
			m_d3d12Private->ExecuteCommandLists();
		}

		void D3D12RHI::Flush()
		{
			m_d3d12Private->FlushCommandQueue();
		}

		void D3D12RHI::OnResize()
		{
			m_d3d12Private->OnResize();
		}
	}
}