#pragma once

#include <array>
#include <RHIAdapter.h>
#include <RHI/Direct3D12RHI/export.h>
#include <Math/Vector.h>
#include <Mesh.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class D3D12RHIPrivate;

		class DIRECT3D_API D3D12RHI final : public RHIAdapter
		{
		public:
			using Ptr = std::shared_ptr<RHIAdapter>;

		public:
			D3D12RHI();

		public:
			virtual void Init() override;
			virtual void Update(Mesh::Ptr mesh, Material::Ptr material) override;
			virtual Mesh::Ptr CreateBoxMesh() override;
<<<<<<< HEAD
			virtual Mesh::Ptr CreateSpiderMesh() override;
=======
			virtual Mesh::Ptr CreatePyramidMesh() override;
>>>>>>> bc7185de889fae0a1f6726b52a3dd4e2d783c1aa
			virtual Material::Ptr GetMaterial(const std::string& name) override;
			virtual void ExecuteCommandLists() override;
			virtual void Flush() override;
			virtual void OnResize() override;

		private:
			std::shared_ptr<D3D12RHIPrivate> m_d3d12Private = nullptr;

			Core::Math::Vector3f FindKnee(Core::Math::Vector3f shoulder, Core::Math::Vector3f foot, float elbow_length);
			std::array<Core::Math::Vector3f, 3> MakeKneeRing(Core::Math::Vector3f shoulder, Core::Math::Vector3f knee, Core::Math::Vector3f foot, float radius);
		};
	}
}