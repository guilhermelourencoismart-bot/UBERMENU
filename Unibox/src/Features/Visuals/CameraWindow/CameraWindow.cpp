#include "CameraWindow.h"

#include "../../Visuals/Materials/Materials.h"
#include "../EnemyCam/EnemyCam.h"

using CViewRenderRenderViewFn = void(__fastcall*)(void*, const CViewSetup&, ClearFlags_t, RenderViewInfo_t);

// Draws camera to the screen
void CCameraWindow::Draw()
{
	if (!m_pCameraMaterial || !m_pCameraTexture || !(m_bShouldDraw || m_bEnemyView) || !I::EngineClient->IsInGame())
		return;

	const auto enemy = F::EnemyCam.GetView();
	if (m_bEnemyView && !enemy.active && !m_bShouldDraw)
		return;
	const auto tWindowBox = m_bEnemyView ? enemy.window : Vars::Visuals::Simulation::ProjectileWindow.Value;

	// Draw to screen
	auto pRenderContext = I::MaterialSystem->GetRenderContext();
	if (!pRenderContext)
		return;
	pRenderContext->DrawScreenSpaceRectangle(
		m_pCameraMaterial,
		tWindowBox.x - tWindowBox.w / 2, tWindowBox.y, tWindowBox.w, tWindowBox.h,
		0, 0, std::clamp(tWindowBox.w,1,m_pCameraTexture->GetActualWidth())-1, std::clamp(tWindowBox.h,1,m_pCameraTexture->GetActualHeight())-1,
		m_pCameraTexture->GetActualWidth(), m_pCameraTexture->GetActualHeight(),
		nullptr, 1, 1
	);
	pRenderContext->Release();
}

// Renders another view onto a texture
void CCameraWindow::RenderView(void* rcx, const CViewSetup& pViewSetup)
{
	const auto enemy = F::EnemyCam.GetView();
	m_bEnemyView = !m_bShouldDraw && enemy.active;
	if (!(m_bShouldDraw || m_bEnemyView) || !m_pCameraTexture || m_bDrawing)
		return;

	m_bDrawing = true;

	const auto tWindowBox = m_bEnemyView ? enemy.window : Vars::Visuals::Simulation::ProjectileWindow.Value;

	CViewSetup tViewSetup = pViewSetup;
	tViewSetup.x = 0;
	tViewSetup.y = 0;

	tViewSetup.origin = m_bEnemyView ? enemy.origin : m_vCameraOrigin;
	tViewSetup.angles = m_bEnemyView ? enemy.angles : m_vCameraAngles;

	tViewSetup.width = std::clamp(tWindowBox.w + 1, 1, m_pCameraTexture->GetActualWidth());
	tViewSetup.height = std::clamp(tWindowBox.h + 1, 1, m_pCameraTexture->GetActualHeight());
	tViewSetup.m_flAspectRatio = float(tViewSetup.width) / float(tViewSetup.height);
	tViewSetup.fov = 90;

	RenderCustomView(rcx, tViewSetup, m_pCameraTexture);

	m_bDrawing = false;
}

void CCameraWindow::RenderCustomView(void* rcx, const CViewSetup& pViewSetup, ITexture* pTexture)
{
	auto pRenderContext = I::MaterialSystem->GetRenderContext();
	if (!pRenderContext)
		return;
	const auto hook = U::Hooks.m_mHooks.find("CViewRender_RenderView");
	if (hook == U::Hooks.m_mHooks.end() || !hook->second)
	{
		pRenderContext->Release();
		return;
	}

	pRenderContext->PushRenderTargetAndViewport();
	pRenderContext->SetRenderTarget(pTexture);

	hook->second->As<CViewRenderRenderViewFn>()(
		rcx, pViewSetup, static_cast<ClearFlags_t>(VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH), RENDERVIEW_UNSPECIFIED);

	pRenderContext->PopRenderTargetAndViewport();
	pRenderContext->Release();
}

void CCameraWindow::Initialize()
{
	SDK::CInitTimingScope tInit("CameraWindow.Initialize");

	if (!m_pCameraMaterial)
	{
		m_pCameraMaterial = F::Materials.create_from_vmt("CameraMaterial",
			"\"UnlitGeneric\""
			"\n{"
			"\n\t$basetexture \"m_pCameraTexture\""
			"\n}");
	}

	if (!m_pCameraTexture && SDK::SupportsDX9Shaders())
	{
		m_pCameraTexture = I::MaterialSystem->CreateNamedRenderTargetTextureEx(
			"m_pCameraTexture",
			1,
			1,
			RT_SIZE_FULL_FRAME_BUFFER,
			IMAGE_FORMAT_RGB888,
			MATERIAL_RT_DEPTH_SHARED,
			TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
			CREATERENDERTARGETFLAGS_HDR
		);
		if (m_pCameraTexture)
			m_pCameraTexture->IncrementReferenceCount();
	}
}

void CCameraWindow::Unload()
{
	m_bEnemyView = m_bDrawing = m_bShouldDraw = false;
	if (m_pCameraMaterial)
	{
		m_pCameraMaterial->DecrementReferenceCount();
		m_pCameraMaterial->DeleteIfUnreferenced();
		m_pCameraMaterial = nullptr;
	}

	if (m_pCameraTexture)
	{
		m_pCameraTexture->DecrementReferenceCount();
		m_pCameraTexture->DeleteIfUnreferenced();
		m_pCameraTexture = nullptr;
	}
}
