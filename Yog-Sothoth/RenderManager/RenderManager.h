#pragma once

#ifdef RENDERMANAGER_EXPORTS
#define RENDERMANAGER_API __declspec(dllexport)
#else
#define RENDERMANAGER_API __declspec(dllimport)
#endif

namespace RenderManager
{

	class Shader
	{
	public:
		RENDERMANAGER_API static int Suma(int a, int b);
	};
}
