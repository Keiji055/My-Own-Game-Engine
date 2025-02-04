#include "GraphicsEngine.pch.h"
#include "GraphicsEngine.h"

#include <filesystem>
#include <utility>
#include <Shaders/Include/DefaultMaterial_PS.h>
#include <Shaders/Include/DefaultMaterial_VS.h>
#include <Shaders/Include/brdfLUT_PS.h>
#include <Shaders/Include/brdfLUT_VS.h>
#include <Shaders/Include/GBufferPS.h>
#include <Shaders/Include/LuminencePS.h>
#include <Shaders/Include/UpsamplingPS.h>
#include <Shaders/Include/CopyPS.h>
#include <Shaders/Include/BloomPS.h>
#include <Shaders/Include/GammaPS.h>
#include <Shaders/Include/GaussianBlurPS.h>
#include <Shaders/Include/EnvironmentPS.h>
#include <Shaders/Include/ScreenSpaceQuad_VS.h>
#include <Shaders/Include/ToneMapPS.h>
#include <Shaders/Include/SSAO.h>


bool GraphicsEngine::Initialize(HWND windowHandle, bool enableDeviceDebug)
{
	GELogger = Logger::Create("GraphicsEngine");
#ifdef _DEBUG
	try
	{
#endif
		GELogger.SetPrintToVSOutput(false);

		//myShouldCastShadows = true;

		myWorldRadius = 0;
		myWorldCenter = CommonUtilities::Vector3f(0, 0, 0);

		myWindowHandle = windowHandle;

		myBackBuffer = std::make_shared<Texture>();
		myDepthBuffer = std::make_shared<Texture>();

		if (!RHI::Initialize(myWindowHandle,
			enableDeviceDebug,
			myBackBuffer.get(),
			myDepthBuffer.get()))
		{
			GELogger.Err("Failed to initialize the RHI!");
			return false;
		}
		myMaterialBuffer.Initialize();

		myWorldMin = CommonUtilities::Vector3f(0, 0, 0);
		myWorldMax = CommonUtilities::Vector3f(0, 0, 0);

		myDefaultPS = std::make_shared<Shader>();
		myDefaultVS = std::make_shared<Shader>();

		RHI::LoadShaderFromMemory(
			myDefaultVS.get(),
			L"DefaultVertexShader",
			BuiltIn_DefaultMaterial_VS_ByteCode,
			sizeof(BuiltIn_DefaultMaterial_VS_ByteCode)
		);

		RHI::LoadShaderFromMemory(
			myDefaultPS.get(),
			L"DefaultPixelShader",
			BuiltIn_DefaultMaterial_PS_ByteCode,
			sizeof(BuiltIn_DefaultMaterial_PS_ByteCode)
		);

		RHI::CreateInputLayout(
			Vertex::InputLayout,
			Vertex::InputLayoutDefinition,
			BuiltIn_DefaultMaterial_VS_ByteCode,
			sizeof(BuiltIn_DefaultMaterial_VS_ByteCode)
		);

		myMaterialBuffer.Data.NormalStrength = 1.0f;
		myMaterialBuffer.Data.Shininess = 1.0f;

		myDefaultMaterial = std::make_shared<Material>(myDefaultVS, myDefaultPS);
		myDefaultAlbedoTexture = std::make_shared<Texture>();
		RHI::LoadTextureFromMemory(
			myDefaultAlbedoTexture.get(),
			L"Default Albedo",
			BuiltIn_Default_C_ByteCode,
			sizeof(BuiltIn_Default_C_ByteCode)
		);

		myDefaultNormalTexture = std::make_shared<Texture>();
		RHI::LoadTextureFromMemory(
			myDefaultNormalTexture.get(),
			L"Default Normal",
			BuiltIn_Default_N_ByteCode,
			sizeof(BuiltIn_Default_N_ByteCode)
		);

		myDefaultMaterialTexture = std::make_shared<Texture>();
		RHI::LoadTextureFromMemory(
			myDefaultMaterialTexture.get(),
			L"Default Material",
			BuiltIn_Default_M_ByteCode,
			sizeof(BuiltIn_Default_M_ByteCode)
		);

		myDefaultFXTexture = std::make_shared<Texture>();
		RHI::LoadTextureFromMemory(
			myDefaultFXTexture.get(),
			L"Default FX",
			BuiltIn_Default_FX_ByteCode,
			sizeof(BuiltIn_Default_FX_ByteCode)
		);

		myDefaultMaterial->SetTexture(L"albedoTexture", myDefaultAlbedoTexture);
		myDefaultMaterial->SetTexture(L"normalTexture", myDefaultNormalTexture);
		myDefaultMaterial->SetTexture(L"materialTexture", myDefaultMaterialTexture);
		myDefaultMaterial->SetTexture(L"FXTexture", myDefaultFXTexture);


		myBRDFLookupTable = std::make_shared<Texture>();
		RHI::CreateTexture(myBRDFLookupTable.get(), L"brdfLUT", 512, 512,
			DXGI_FORMAT_R16G16_FLOAT,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET
		);

		CreateGBufferTextures();

		mySceneBuffer = std::make_shared<Texture>();
		RHI::CreateTexture(mySceneBuffer.get(), L"SceneBuffer", 
			RHI::GetDeviceSize().Width,
			RHI::GetDeviceSize().Height, 
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			D3D11_USAGE_DEFAULT, 
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

		myIntermediateTexture = std::make_shared<Texture>();
		RHI::CreateTexture(myIntermediateTexture.get(), L"IntermediateTexture",
			RHI::GetDeviceSize().Width,
			RHI::GetDeviceSize().Height,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

		myIntermediateBTexture = std::make_shared<Texture>();
		RHI::CreateTexture(myIntermediateBTexture.get(), L"IntermediateBTexture",
			RHI::GetDeviceSize().Width,
			RHI::GetDeviceSize().Height,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

		myHalfSizeTexture = std::make_shared<Texture>();
		RHI::CreateTexture(myHalfSizeTexture.get(), L"HalfTexture",
			RHI::GetDeviceSize().Width / 2,
			RHI::GetDeviceSize().Height / 2,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

		myQuaterSizeTexture = std::make_shared<Texture>();
		RHI::CreateTexture(myQuaterSizeTexture.get(), L"QuaterTexture",
			RHI::GetDeviceSize().Width / 4,
			RHI::GetDeviceSize().Height / 4,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

		mySecondQuaterSizeTexture = std::make_shared<Texture>();
		RHI::CreateTexture(mySecondQuaterSizeTexture.get(), L"SecondQuaterTexture",
			RHI::GetDeviceSize().Width / 4,
			RHI::GetDeviceSize().Height / 4,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

		myEmptyTexture = std::make_shared<Texture>();
		RHI::CreateTexture(myEmptyTexture.get(), L"EmptyTexture",
			RHI::GetDeviceSize().Width,
			RHI::GetDeviceSize().Height,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

		myShadowMap = std::make_shared<Texture>();
		RHI::CreateTexture(myShadowMap.get(), L"Shadow", 512, 512, 
			DXGI_FORMAT_R32_TYPELESS, D3D11_USAGE_DEFAULT,
			D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);

		RHI::ClearDepthStencil(myShadowMap.get());

		D3D11_SAMPLER_DESC defaultSamplerDesc = {};

		defaultSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		defaultSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		defaultSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		defaultSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		defaultSamplerDesc.MipLODBias = 0.0f;
		defaultSamplerDesc.MaxAnisotropy = 1;
		defaultSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		defaultSamplerDesc.BorderColor[0] = 1.0f;
		defaultSamplerDesc.BorderColor[1] = 1.0f;
		defaultSamplerDesc.BorderColor[2] = 1.0f;
		defaultSamplerDesc.BorderColor[3] = 1.0f;
		defaultSamplerDesc.MinLOD = -D3D11_FLOAT32_MAX;
		defaultSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (!RHI::CreateSamplerState(myDefaultSampler, defaultSamplerDesc))
		{
			assert(false);
		}

		RHI::SetSamplerState(myDefaultSampler, 0);

		D3D11_SAMPLER_DESC pointSampleDesc = {};

		pointSampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		pointSampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		pointSampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		pointSampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		pointSampleDesc.MipLODBias = 0.0f;
		pointSampleDesc.MaxAnisotropy = 1;
		pointSampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		pointSampleDesc.BorderColor[0] = 1.0f;
		pointSampleDesc.BorderColor[1] = 1.0f;
		pointSampleDesc.BorderColor[2] = 1.0f;
		pointSampleDesc.BorderColor[3] = 1.0f;
		pointSampleDesc.MinLOD = -D3D11_FLOAT32_MAX;
		pointSampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (!RHI::CreateSamplerState(myPointSampler, pointSampleDesc))
		{
			assert(false);
		}

		RHI::SetSamplerState(myPointSampler, 1);

		myObjectBuffer.Initialize();
		myFrameBuffer.Initialize();
		myLightBuffer.Initialize();
		myPointLightBuffer.Initialize();
		mySpotLightBuffer.Initialize();

		D3D11_SAMPLER_DESC shadowSamplerDesc = {};
		shadowSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSamplerDesc.MipLODBias = 0.0f;
		shadowSamplerDesc.MaxAnisotropy = 1;
		shadowSamplerDesc.BorderColor[0] = 0.0f;
		shadowSamplerDesc.BorderColor[1] = 0.0f;
		shadowSamplerDesc.BorderColor[2] = 0.0f;
		shadowSamplerDesc.BorderColor[3] = 0.0f;
		shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		shadowSamplerDesc.MinLOD = 0;
		shadowSamplerDesc.MaxLOD = 0;

		RHI::CreateSamplerState(myShadowSampler, shadowSamplerDesc);
		RHI::SetSamplerState(myShadowSampler, 14);

		D3D11_SAMPLER_DESC LUTSamplerDesc = {};
		LUTSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		LUTSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		LUTSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		LUTSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		LUTSamplerDesc.MipLODBias = 0.0f;
		LUTSamplerDesc.MaxAnisotropy = 1;
		LUTSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		LUTSamplerDesc.BorderColor[0] = 1.0f;
		LUTSamplerDesc.BorderColor[1] = 1.0f;
		LUTSamplerDesc.BorderColor[2] = 1.0f;
		LUTSamplerDesc.BorderColor[3] = 1.0f;
		LUTSamplerDesc.MinLOD = 0;
		LUTSamplerDesc.MaxLOD = 0;

		RHI::CreateSamplerState(myLUTSampler, LUTSamplerDesc);
		RHI::SetSamplerState(myLUTSampler, 15);

		ComPtr<ID3D11VertexShader> brdfVS;
		RHI::CreateVertexShader(brdfVS,
			BuiltIn_brdfLUT_VS_ByteCode,
			sizeof(BuiltIn_brdfLUT_VS_ByteCode));

		ComPtr<ID3D11PixelShader> brdfPS;
		RHI::CreatePixelShader(brdfPS,
			BuiltIn_brdfLUT_PS_ByteCode,
			sizeof(BuiltIn_brdfLUT_PS_ByteCode));

		RHI::SetVertexShader(brdfVS);
		RHI::SetPixelShader(brdfPS);
		RHI::SetRenderTarget(myBRDFLookupTable.get(), nullptr);
		RHI::ConfigureInputAssembler(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			nullptr,
			nullptr,
			0,
			nullptr
		);
		RHI::Draw(4);

		RHI::SetRenderTarget(nullptr, nullptr);
		RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER,
			127, myBRDFLookupTable.get());

		D3D11_BLEND_DESC blendDesc{};
		D3D11_RENDER_TARGET_BLEND_DESC& targetBlendDesc = blendDesc.RenderTarget[0];

		targetBlendDesc.BlendEnable = TRUE;
		targetBlendDesc.SrcBlend = D3D11_BLEND_ONE;
		targetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		targetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		targetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		targetBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
		targetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		targetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		if (!RHI::CreateBlendState(myAlphaBlendState, blendDesc)) 
		{
			assert(false);
		}

		targetBlendDesc.BlendEnable = TRUE;
		targetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		targetBlendDesc.DestBlend = D3D11_BLEND_ONE;
		targetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		targetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
		targetBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
		targetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		targetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		if (!RHI::CreateBlendState(myAddativeBlendState, blendDesc))
		{
			assert(false);
		}

		RHI::CreatePixelShader(myLuminencePS,
			BuiltIn_LuminencePS_ByteCode,
			sizeof(BuiltIn_LuminencePS_ByteCode));

		RHI::CreatePixelShader(myEnvironmentPS,
			BuiltIn_EnvironmentPS_ByteCode,
			sizeof(BuiltIn_EnvironmentPS_ByteCode));

		RHI::CreatePixelShader(myCopyPS,
			BuiltIn_CopyPS_ByteCode,
			sizeof(BuiltIn_CopyPS_ByteCode));

		RHI::CreatePixelShader(myGaussianBlurPS,
			BuiltIn_GaussianBlurPS_ByteCode,
			sizeof(BuiltIn_GaussianBlurPS_ByteCode));
		
		RHI::CreatePixelShader(myUpsamplePS,
			BuiltIn_UpsamplingPS_ByteCode,
			sizeof(BuiltIn_UpsamplingPS_ByteCode));
		
		RHI::CreatePixelShader(myBloomPS,
			BuiltIn_BloomPS_ByteCode,
			sizeof(BuiltIn_BloomPS_ByteCode));

		RHI::CreatePixelShader(myToneMapPS,
			BuiltIn_ToneMapPS_ByteCode,
			sizeof(BuiltIn_ToneMapPS_ByteCode));

		RHI::CreatePixelShader(myGammaPS,
			BuiltIn_GammaPS_ByteCode,
			sizeof(BuiltIn_GammaPS_ByteCode));

		RHI::CreatePixelShader(myDeferredPS,
			BuiltIn_GBufferPS_ByteCode,
			sizeof(BuiltIn_GBufferPS_ByteCode));

		RHI::CreatePixelShader(mySSAOPS,
			BuiltIn_SSAO_ByteCode,
			sizeof(BuiltIn_SSAO_ByteCode));
		
		RHI::CreateVertexShader(myScreenSpaceQuadVS,
			BuiltIn_ScreenspaceQuad_VS_ByteCode,
			sizeof(BuiltIn_ScreenspaceQuad_VS_ByteCode));


		D3D11_DEPTH_STENCIL_DESC readOnlyDepthDesc = {};
		readOnlyDepthDesc.DepthEnable = true;
		readOnlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		readOnlyDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
		readOnlyDepthDesc.StencilEnable = false;

		CreateDepthStencilStates();
#ifdef _DEBUG
	}
	catch (const std::exception& e)
	{
		GELogger.LogException(e);
		exit(-1);
	}
#endif

	return true;
}

const std::shared_ptr<Material>& GraphicsEngine::GetDefaultMaterial() const
{
	return myDefaultMaterial;
}

const std::shared_ptr<Shader>& GraphicsEngine::GetVertexShader() const
{
	return myDefaultVS;
}

const std::shared_ptr<Shader>& GraphicsEngine::GetPixelShader() const
{
	return myDefaultPS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetLuminencePixelShader() const
{
	return myLuminencePS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetCopyPixelShader() const
{
	return myCopyPS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetGaussianBlurPixelShader() const
{
	return myGaussianBlurPS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetGammaPixelShader() const
{
	return myGammaPS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetUpsamplePixelShader() const
{
	return myUpsamplePS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetBloomPixelShader() const
{
	return myBloomPS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetEnvironmentPixelShader() const
{
	return myEnvironmentPS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetDeferredPixelShader() const
{
	return myDeferredPS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetToneMapPixelShader() const
{
	return myToneMapPS;
}

const ComPtr<ID3D11PixelShader>& GraphicsEngine::GetSSAOPixelShader() const
{
	return mySSAOPS;
}

const ComPtr<ID3D11VertexShader>& GraphicsEngine::GetScreenSpaceQuadVertexShader() const
{
	return myScreenSpaceQuadVS;
}

const std::vector<ID3D11DepthStencilState*>& GraphicsEngine::GetDepthStencilStates() const
{
	return myDepthStencilStates;
}

const MaterialBuffer& GraphicsEngine::GetMaterialBuffer() const
{
	return myMaterialBuffer;
}

const GBufferData& GraphicsEngine::GetGBuffer() const
{
	return myGBuffer;
}

const std::vector<std::shared_ptr<Texture>>& GraphicsEngine::GetGBufferTextures() const
{
	return myGBufferTextures;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetRenderTarget() const
{
	return myBackBuffer;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetEmptyTexture() const
{
	return myEmptyTexture;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetSceneBuffer() const
{
	return mySceneBuffer;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetIntermediateTexture() const
{
	return myIntermediateTexture;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetIntermediateBTexture() const
{
	return myIntermediateBTexture;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetHalfSizeTexture() const
{
	return myHalfSizeTexture;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetQuaterSizeTexture() const
{
	return myQuaterSizeTexture;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetSecondQuaterSizeTexture() const
{
	return mySecondQuaterSizeTexture;
}

bool GraphicsEngine::CreateDepthStencilStates()
{
	ID3D11DepthStencilState* state;
	myDepthStencilStates.push_back(state);
	myDepthStencilStates.push_back(state);
	HRESULT result;
	D3D11_DEPTH_STENCIL_DESC readOnlyDepthDesc = {};
	readOnlyDepthDesc.DepthEnable = true;
	readOnlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readOnlyDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	readOnlyDepthDesc.StencilEnable = false;
	result = RHI::Device->CreateDepthStencilState(&readOnlyDepthDesc, &myDepthStencilStates[DepthStencilState::DDS_ReadOnly]);
	if (FAILED(result))
	{
		return false;
	}
	myDepthStencilStates[DepthStencilState::DDS_ReadWrite] = nullptr;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetDepthStencil() const
{
	return myDepthBuffer;
}

const std::shared_ptr<Texture>& GraphicsEngine::GetShadowMap() const
{
	return myShadowMap;
}

CommonUtilities::Vector3f& GraphicsEngine::GetWorldCenter()
{
	return myWorldCenter;
}

float& GraphicsEngine::GetWorldRadius()
{
	return myWorldRadius;
}

void GraphicsEngine::SetWorldBounds(CommonUtilities::AABB3D<float>& aBoundingBox)
{
	if (aBoundingBox.GetMin().x < myWorldMin.x)
	{
		myWorldMin.x = aBoundingBox.GetMin().x;
	}
	if (aBoundingBox.GetMin().y < myWorldMin.y)
	{
		myWorldMin.y = aBoundingBox.GetMin().y;
	}
	if (aBoundingBox.GetMin().z < myWorldMin.z)
	{
		myWorldMin.z = aBoundingBox.GetMin().z;
	}

	if (aBoundingBox.GetMax().x > myWorldMax.x)
	{
		myWorldMax.x = aBoundingBox.GetMax().x;
	}
	if (aBoundingBox.GetMax().y > myWorldMax.y)
	{
		myWorldMax.y = aBoundingBox.GetMax().y;
	}
	if (aBoundingBox.GetMax().z > myWorldMax.z)
	{
		myWorldMax.z = aBoundingBox.GetMax().z;
	}
}

void GraphicsEngine::CalculatWorldCenterAndRadius()
{
	myWorldCenter = (myWorldMin + myWorldMax);
	myWorldCenter = 0.5f * myWorldCenter;

	CommonUtilities::Vector3f temp = myWorldCenter - myWorldMax;
	myWorldRadius = temp.Length();
}

void GraphicsEngine::CreateGBufferTextures()
{
	myGBuffer.Albedo = std::make_shared<Texture>();
	RHI::CreateTexture(myGBuffer.Albedo.get(), 
		L"Albedo", 
		RHI::GetDeviceSize().Width, 
		RHI::GetDeviceSize().Height, 
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_USAGE_DEFAULT, 
		D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE);

	myGBuffer.PixelNormal = std::make_shared<Texture>();
	RHI::CreateTexture(myGBuffer.PixelNormal.get(),
		L"PixelNormal",
		RHI::GetDeviceSize().Width,
		RHI::GetDeviceSize().Height,
		DXGI_FORMAT_R16G16B16A16_SNORM,
		D3D11_USAGE_DEFAULT,
		D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE);

	myGBuffer.Material = std::make_shared<Texture>();
	RHI::CreateTexture(myGBuffer.Material.get(),
		L"Material",
		RHI::GetDeviceSize().Width,
		RHI::GetDeviceSize().Height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_USAGE_DEFAULT,
		D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE);

	myGBuffer.VertexNormal = std::make_shared<Texture>();
	RHI::CreateTexture(myGBuffer.VertexNormal.get(),
		L"VertexNormal",
		RHI::GetDeviceSize().Width,
		RHI::GetDeviceSize().Height,
		DXGI_FORMAT_R16G16B16A16_SNORM,
		D3D11_USAGE_DEFAULT,
		D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE);

	myGBuffer.WorldPosition = std::make_shared<Texture>();
	RHI::CreateTexture(myGBuffer.WorldPosition.get(),
		L"Position",
		RHI::GetDeviceSize().Width,
		RHI::GetDeviceSize().Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		D3D11_USAGE_DEFAULT,
		D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE);

	myGBuffer.FXTexture = std::make_shared<Texture>();
	RHI::CreateTexture(myGBuffer.FXTexture.get(),
		L"FX",
		RHI::GetDeviceSize().Width,
		RHI::GetDeviceSize().Height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_USAGE_DEFAULT,
		D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE);

	myGBufferTextures.push_back(myGBuffer.Albedo);
	myGBufferTextures.push_back(myGBuffer.PixelNormal);
	myGBufferTextures.push_back(myGBuffer.Material);
	myGBufferTextures.push_back(myGBuffer.VertexNormal);
	myGBufferTextures.push_back(myGBuffer.WorldPosition);
	myGBufferTextures.push_back(myGBuffer.FXTexture);
}

void GraphicsEngine::SetBlendStateToAlpha()
{
	RHI::SetBlendState(myAlphaBlendState);
}

void GraphicsEngine::SetBlendStateToAddative()
{
	RHI::SetBlendState(myAddativeBlendState);
}

void GraphicsEngine::SetBlendStateToNone()
{
	RHI::SetBlendState(nullptr);
}

void GraphicsEngine::SetLoggingWindow(HANDLE aHandle)
{
	GELogger.SetConsoleHandle(aHandle);
}

void GraphicsEngine::BeginFrame()
{
	RHI::ClearRenderTarget(myBackBuffer.get());
	RHI::ClearRenderTarget(mySceneBuffer.get());
	RHI::ClearDepthStencil(myDepthBuffer.get());
	RHI::ClearDepthStencil(myShadowMap.get());
}

void GraphicsEngine::RenderFrame(float aDeltaTime, double aTotalTime)
{
	for (int index = 0; index < myCommandList.size(); index++)
	{
		myCommandList[index]->Execute();
	}
}

void GraphicsEngine::EndFrame()
{
	// We finish our frame here and present it on screen.
	RHI::Present();
	myCommandList.clear();
}
