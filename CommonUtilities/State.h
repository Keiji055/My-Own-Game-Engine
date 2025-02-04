#pragma once
#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/sprite/sprite.h>
#include <InputHandler.h>

namespace CommonUtilities
{
	class State
	{
	public:
		State() = default;
		~State() = default;
		virtual void Init() = 0;
		virtual const GameState& Update() = 0;
		virtual void Render(Tga::SpriteDrawer& aSpriteDrawer) = 0;
		virtual bool LetThroughRender() = 0; 
		virtual const GameState& GetGameState() const = 0;
	};
}