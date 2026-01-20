
#   ifdef DX11RENDERER_EXPORTS
#       define THIS_IS_ENGINE_MODULE
#       define DX11RENDERER_API __declspec(dllexport)
#   elif defined(DX11RENDERER_IMPORTS)
#       define DX11RENDERER_API __declspec(dllimport)
#   else
#       define DX11RENDERER_API
#   endif

#ifndef CORE_API
#define CORE_API __declspec(dllimport)
#endif

#ifndef GAMEMODULE_API
#define GAMEMODULE_API __declspec(dllimport)
#endif

#include "SSCore.h"

