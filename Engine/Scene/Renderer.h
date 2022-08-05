#pragma once


class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void SkyboxPath();
	static void DepthPrePath();
	static void GeometryPath();
	static void PostFxPath();

	static void DrawMesh();

	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	static void UpdateViewport();
	static void SetClearColor(const glm::vec4& color);
	static void Clear();
	static void SetCullMode(const uint8_t& cullMode);

	static void RenderQuad();

private:
	static uint32_t m_viewport[4];
};
