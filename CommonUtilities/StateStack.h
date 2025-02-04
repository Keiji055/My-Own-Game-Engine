#pragma once
#include "State.h"
#include <vector>
#include "Enums.h"
#include <memory>
#include <InputHandler.h>
namespace CommonUtilities
{
	class StateStack
	{
	public:
		StateStack();
		~StateStack();
		void Init();
		void Update();
		void Render(Tga::SpriteDrawer& aSpriteDrawer);
		void PushState(GameState aState);
		void Pop();
		std::shared_ptr<State>& GetCurrentState();
		void AddState(GameState anID, std::shared_ptr<State> aState);

	private:
		std::vector<std::shared_ptr<State>> myStates;
		std::unordered_map<GameState, std::shared_ptr<State>> myCachedStates;
		GameState myCurrentStateID;
	};
}