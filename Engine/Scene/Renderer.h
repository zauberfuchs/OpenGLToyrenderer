#pragma once


class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void ShadowPrepath();
	static void GeometryPath();

	static void DrawMesh();

private:

	static void RenderScreenQuad();
};

