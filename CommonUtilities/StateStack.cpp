#include "StateStack.h"
namespace CommonUtilities
{
	StateStack::StateStack()
	{
	}

	StateStack::~StateStack()
	{

	}

	void StateStack::Init()
	{
		PushState(GameState::Menu);
	}

	void StateStack::Update()
	{
		if (GetCurrentState()->Update() == GameState::Unchanged){ }
		else if (GetCurrentState()->Update() == myCurrentStateID)
		{
			Pop();
			if (myStates.size() <= 0)
			{
				PostQuitMessage(0);
			}
			else
			{
				myCurrentStateID = GetCurrentState()->GetGameState();
			}
		}
		else if(GetCurrentState()->Update() != myCurrentStateID)
		{
			PushState(GetCurrentState()->Update());
		}
	}

	void StateStack::Render(Tga::SpriteDrawer& aSpriteDrawer)
	{
		if (myStates.size() <= 0)
		{
			return;
		}

		if (GetCurrentState()->LetThroughRender() == true)
		{
			myStates[myStates.size() - 2]->Render(aSpriteDrawer);
		}

		GetCurrentState()->Render(aSpriteDrawer);
	}

	void StateStack::PushState(GameState aState)
	{
		myStates.push_back(myCachedStates[aState]);
		myCurrentStateID = aState;
		myStates.back()->Init();
	}

	void StateStack::Pop()
	{
		myStates.pop_back();
	}

	std::shared_ptr<State>& StateStack::GetCurrentState()
	{
		return myStates.back();
	}

	void StateStack::AddState(GameState anID, std::shared_ptr<State> aState)
	{
		myCachedStates.insert(std::make_pair(anID, aState));
	}
}
