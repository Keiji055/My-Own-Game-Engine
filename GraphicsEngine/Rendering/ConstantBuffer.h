#pragma once

struct ID3D11Buffer;
#include "FrameBuffer.h"
#include "ObjectBuffer.h"
#include "LightBuffer.h"
#include "PointLightBuffer.h"
#include "SpotLightBuffer.h"
#include <wrl.h>
using namespace Microsoft::WRL;

/**
 * Provides the base implementation of a Constant Buffer
 * POD object on the CPU.
 */
struct ConstantBufferBase
{
	friend class RHI;
private:
	const size_t myDataSize;
	const void* myDataPtr = nullptr;
	ComPtr<ID3D11Buffer> myBuffer;

protected:
	ConstantBufferBase(const void* aPtr, size_t aDataSize);

public:
	bool Initialize();
	virtual ~ConstantBufferBase();
};

/**
 * Templated base for Constant Buffers, allowing unified access
 * to contained data via the Data property regardless of type.
 */
template<typename DataStruct>
struct ConstantBuffer : public ConstantBufferBase
{

	DataStruct Data;

	ConstantBuffer()
		: ConstantBufferBase(&Data, sizeof(DataStruct))
	{  }

	ConstantBuffer(const ConstantBuffer& aConstantBuffer)
		: ConstantBufferBase(&Data, sizeof(DataStruct)), Data(aConstantBuffer.Data)
	{  }
};

typedef ConstantBuffer<FrameBufferData>		FrameBuffer;
typedef ConstantBuffer<ObjectBufferData>	ObjectBuffer;
typedef ConstantBuffer<DirectionalLightData>LightBuffer;
typedef ConstantBuffer<PointLightData>		PointLightBuffer;
typedef ConstantBuffer<SpotLightData>		SpotLightBuffer;