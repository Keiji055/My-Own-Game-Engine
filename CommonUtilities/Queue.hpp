#pragma once
#include <cassert>
#include <array>

namespace CommonUtilities
{
	template <class T>
	class Queue
	{
	public:
		Queue();
		
		~Queue();

		int GetSize() const;

		const T& GetFront() const;

		T& GetFront();

		void Enqueue(const T& aValue);

		T Dequeue();

	private:
		unsigned int myQueueSize;
		int myFilledSize;
		int myFirstElement;
		int myLastElement; 
		T* myQueue;
	};

	template <class T>
	Queue<T>::Queue()
	{
		myFirstElement = -1;
		myLastElement = -1;
		myQueueSize = 8;
		myFilledSize = 0;
		myQueue = new T[myQueueSize];
	}

	template <class T>
	Queue<T>::~Queue()
	{
		delete[] myQueue;
	}

	template <class T>
	int Queue<T>::GetSize() const
	{
		return myFilledSize;
	}

	template <class T>
	const T& Queue<T>::GetFront() const
	{
		assert(myFilledSize != 0);
		return myQueue[myFirstElement];
	}

	template <class T>
	T& Queue<T>::GetFront()
	{
		assert(myFilledSize != 0);
		return myQueue[myFirstElement];
	}

	template <class T>
	void Queue<T>::Enqueue(const T& aValue)
	{
		if (myFilledSize == 0)
		{
			myQueue[0] = aValue;
			myFirstElement = 0;
			myLastElement = 0;
		}
		else if (myFilledSize == myQueueSize)
		{
			int oldSize = myQueueSize;
			myQueueSize *= 2;
			T* newQueue = new T[myQueueSize];

			for (int index = 0; index < oldSize; index++)
			{
				newQueue[index] = Dequeue();
			}
			newQueue[oldSize] = aValue;
			myFilledSize = oldSize;
			T* oldQueue = myQueue;
			myQueue = newQueue;
			myFirstElement = 0;
			myLastElement = oldSize;
			delete[] oldQueue;
		}
		else
		{
			myLastElement = (myLastElement + 1) % myQueueSize;
			myQueue[myLastElement] = aValue;
		}
		myFilledSize++;
	}

	template <class T>
	T Queue<T>::Dequeue()
	{
		assert(myFilledSize != 0);
		int deQueuedIndex = myFirstElement;
		myFirstElement = (myFirstElement + 1) % myQueueSize;
		myFilledSize--;
		return myQueue[deQueuedIndex];
	}
}