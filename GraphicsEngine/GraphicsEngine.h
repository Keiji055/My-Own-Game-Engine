#pragma once
#include <wrl.h>
#include <memory>
#include "InterOp/RHI.h"
#include <Objects/Mesh.h>
#include <GraphicsCommand.h>
#include <GraphicsEngine/Rendering/ConstantBuffer.h>
#include "Shaders/Shader.h"
#include "Objects/Default_C.h"
#include "Objects/Default_N.h"
#include "Objects/Default_M.h"
#include "Objects/Default_FX.h"
#include "../CommonUtilities/AABB3D.hpp"
#include "../CommonUtilities/Vector.hpp"
#include "Rendering/GBuffer.h"
#include <vector>
using namespace Microsoft::WRL;
#include "Rendering/Texture.h"

enum class BlendState 
{
	eAlpha,
	eAddative,
	eNone
};

enum DepthStencilState
{
	DDS_ReadWrite,
	DDS_ReadOnly,
	DDS_COUNT
};

class GraphicsEngine
{
	friend class GraphicsCommand;

	FrameBuffer			myFrameBuffer;
	ObjectBuffer		myObjectBuffer;
	LightBuffer			myLightBuffer;
	MaterialBuffer		myMaterialBuffer;
	GBufferData			myGBuffer;
	PointLightBuffer	myPointLightBuffer;
	SpotLightBuffer		mySpotLightBuffer;
	
	std::vector<std::shared_ptr<GraphicsCommand>> myCommandList;

	SIZE myWindowSize{0,0};
	HWND myWindowHandle{};

	std::vector<std::shared_ptr<Texture>> myGBufferTextures;

	std::shared_ptr<Texture>		myBackBuffer;
	std::shared_ptr<Texture>		mySceneBuffer;
	std::shared_ptr<Texture>		myDepthBuffer;

	std::shared_ptr<Texture>		myDefaultAlbedoTexture;
	std::shared_ptr<Texture>		myEmptyTexture;
	std::shared_ptr<Texture>		myDefaultNormalTexture;
	std::shared_ptr<Texture>		myDefaultMaterialTexture;
	std::shared_ptr<Texture>		myDefaultFXTexture;
	std::shared_ptr<Texture>		myBRDFLookupTable;
	std::shared_ptr<Texture>		myShadowMap;
	std::shared_ptr<Texture>		myIntermediateTexture;
	std::shared_ptr<Texture>		myIntermediateBTexture;
	std::shared_ptr<Texture>		myHalfSizeTexture;
	std::shared_ptr<Texture>		myQuaterSizeTexture;
	std::shared_ptr<Texture>		mySecondQuaterSizeTexture;

	// TODO: TEMP: These will live elsewhere later on.
	std::shared_ptr<Shader>			myDefaultVS;
	std::shared_ptr<Shader>			myDefaultPS;

	std::shared_ptr<Material>		myDefaultMaterial;

	MaterialBufferData				myMaterialBufferData;
	ComPtr<ID3D11Buffer>			myVertexBuffer;
	ComPtr<ID3D11Buffer>			myIndexBuffer;

	ComPtr<ID3D11SamplerState>		myDefaultSampler;
	ComPtr<ID3D11SamplerState>		myLUTSampler;
	ComPtr<ID3D11SamplerState>		myShadowSampler;
	ComPtr<ID3D11SamplerState>		myPointSampler;

	ComPtr<ID3D11PixelShader>		myPixelShader;
	ComPtr<ID3D11PixelShader>		myLuminencePS;
	ComPtr<ID3D11PixelShader>		myCopyPS;
	ComPtr<ID3D11PixelShader>		myGaussianBlurPS;
	ComPtr<ID3D11PixelShader>		myDeferredPS;
	ComPtr<ID3D11PixelShader>		myBloomPS;
	ComPtr<ID3D11PixelShader>		myGammaPS;
	ComPtr<ID3D11PixelShader>		myUpsamplePS;
	ComPtr<ID3D11PixelShader>		myEnvironmentPS;
	ComPtr<ID3D11PixelShader>		myToneMapPS;
	ComPtr<ID3D11PixelShader>		mySSAOPS;

	ComPtr<ID3D11VertexShader>		myVertexShader;
	ComPtr<ID3D11VertexShader>		myScreenSpaceQuadVS;

	ComPtr<ID3D11BlendState>		myAlphaBlendState;
	ComPtr<ID3D11BlendState>		myAddativeBlendState;

	CommonUtilities::Vector3f		myWorldCenter;
	CommonUtilities::Vector3f		myWorldMin;
	CommonUtilities::Vector3f		myWorldMax;

	std::vector<ID3D11DepthStencilState*> myDepthStencilStates;

	float							myWorldRadius;
	//bool							myShouldCastShadows;

	unsigned myTopology = 0;
	unsigned myVertexStride = 0;
	unsigned myNumIndices = 0;
	// We're a container singleton, no instancing this outside the class.
	GraphicsEngine() = default;

public:

	static GraphicsEngine& Get() { static GraphicsEngine myInstance; return myInstance; }

	/**
	 * Initializes the Graphics Engine with the specified settings.
	 * @param windowHandle The window that will contain this Graphics Engine.
	 * @param enableDeviceDebug If DirectX should write debug output in the Output.
	 */
	bool Initialize(HWND windowHandle, bool enableDeviceDebug);
	
	template<typename T, typename... Types>
	void AddCommand(Types... args);

	const std::shared_ptr<Material>& GetDefaultMaterial() const;
	const std::shared_ptr<Shader>& GetVertexShader() const;
	const std::shared_ptr<Shader>& GetPixelShader() const;

	const ComPtr<ID3D11PixelShader>& GetLuminencePixelShader() const;
	const ComPtr<ID3D11PixelShader>& GetCopyPixelShader() const;
	const ComPtr<ID3D11PixelShader>& GetGaussianBlurPixelShader() const;
	const ComPtr<ID3D11PixelShader>& GetGammaPixelShader() const;
	const ComPtr<ID3D11PixelShader>& GetUpsamplePixelShader() const;
	const ComPtr<ID3D11PixelShader>& GetBloomPixelShader() const;
	const ComPtr<ID3D11PixelShader>& GetEnvironmentPixelShader() const;
	const ComPtr<ID3D11PixelShader>& GetDeferredPixelShader() const;
	const ComPtr<ID3D11PixelShader>& GetToneMapPixelShader() const;
	const ComPtr<ID3D11PixelShader>& GetSSAOPixelShader() const;
	const ComPtr<ID3D11VertexShader>& GetScreenSpaceQuadVertexShader() const;

	const std::vector< ID3D11DepthStencilState*>& GetDepthStencilStates() const;

	const MaterialBuffer& GetMaterialBuffer() const;
	const GBufferData& GetGBuffer() const;
	const std::vector<std::shared_ptr<Texture>>& GetGBufferTextures() const;

	const std::shared_ptr<Texture>& GetRenderTarget() const;
	const std::shared_ptr<Texture>& GetEmptyTexture() const;
	const std::shared_ptr<Texture>& GetSceneBuffer() const;
	const std::shared_ptr<Texture>& GetIntermediateTexture() const;
	const std::shared_ptr<Texture>& GetIntermediateBTexture() const;
	const std::shared_ptr<Texture>& GetDepthStencil() const;
	const std::shared_ptr<Texture>& GetShadowMap() const;
	const std::shared_ptr<Texture>& GetHalfSizeTexture() const;
	const std::shared_ptr<Texture>& GetQuaterSizeTexture() const;
	const std::shared_ptr<Texture>& GetSecondQuaterSizeTexture() const;
	bool CreateDepthStencilStates();
	CommonUtilities::Vector3f& GetWorldCenter();
	float& GetWorldRadius();

	void SetWorldBounds(CommonUtilities::AABB3D<float>& aBoundingBox);
	void CalculatWorldCenterAndRadius();
	void CreateGBufferTextures();

	void SetBlendStateToAlpha();
	void SetBlendStateToAddative();
	void SetBlendStateToNone();

	void SetLoggingWindow(HANDLE aHandle);

	/**
	 * Prepares the next frame for rendering by resetting states and clearing all render targets.
	 */
	void BeginFrame();

	/**
	 * Finishes a rendered frame and swaps the screen buffers to display it on screen.
	 */
	void EndFrame();

	/**
	 * Renders the current scene to the BackBuffer.
	 */
	void RenderFrame(float aDeltaTime, double aTotalTime);

	[[nodiscard]] HWND FORCEINLINE GetWindowHandle() const { return myWindowHandle; }
	[[nodiscard]] SIZE FORCEINLINE GetWindowSize() const { return myWindowSize; }

};

template<typename T, typename ...Types>
inline void GraphicsEngine::AddCommand(Types ...args)
{
	std::shared_ptr<T> command = std::make_shared<T>(args...);
	myCommandList.emplace_back(command);
}
