#include <RenderEngine.h>
#include <RHIHelper.h>
#include <Mesh.h>

namespace GameEngine::Render
{
	RenderEngine::RenderEngine()
	{
		m_rhi = HAL::RHIHelper::CreateRHI("D3D12");
		m_rhi->Init();

<<<<<<< HEAD
		RenderObject::Ptr box = std::make_shared<RenderObject>();
		m_RenderObjects.push_back(box);
		box->m_mesh = m_rhi->CreateSpiderMesh();
		box->m_material = m_rhi->GetMaterial(box->m_mesh->GetName());
=======
		RenderObject::Ptr pyr = std::make_shared<RenderObject>();
		m_RenderObjects.push_back(pyr);
		// box->m_mesh = m_rhi->CreateBoxMesh();
		pyr->m_mesh = m_rhi->CreatePyramidMesh();
		pyr->m_material = m_rhi->GetMaterial(pyr->m_mesh->GetName());
>>>>>>> bc7185de889fae0a1f6726b52a3dd4e2d783c1aa
		
		m_rhi->ExecuteCommandLists();
		m_rhi->Flush();
	}

	void RenderEngine::Update()
	{
		m_rhi->Update(m_RenderObjects[0]->m_mesh, m_RenderObjects[0]->m_material);

		m_rhi->Flush();
	}

	void RenderEngine::OnResize(uint16_t width, uint16_t height)
	{
		m_swapChainWidth = width;
		m_swapChainHeight = height;
		m_rhi->OnResize();
	}
}