#pragma once

#include "../../Engine/OpenGL/VertexArray.h"
#include "../../Engine/OpenGL/VertexBufferLayout.h"
#include "../../Engine/OpenGL/IndexBuffer.h"
#include "../../Engine/OpenGL/Texture.h"
#include "../../Engine/Components/Transform.h"
#include "../../Engine/Components/Material.h"
#include "../../Engine/Components/MaterialPBR.h"


class IMesh
{
public:
	virtual ~IMesh() = default;

	virtual void SetupMesh() = 0;

	virtual void SetName(const std::string& name) = 0;
	virtual std::string GetName() = 0;

	virtual void SetMaterial(IMaterial* material) = 0;
	virtual IMaterial* GetMaterial() = 0;

	virtual VertexArray* GetVAO() = 0;
	virtual uint16_t GetRenderMode() = 0;
	virtual uint32_t GetIndicesSize() = 0;
};