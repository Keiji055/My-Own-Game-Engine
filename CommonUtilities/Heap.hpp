#pragma once
#include <vector>

namespace CommonUtilities
{
	template <class T>
	class Heap
	{
	public:
		Heap();
		~Heap();

		int GetSize() const;

		void Enqueue(const T& anElement);

		const T& GetTop() const;

		T Dequeue();

	private:
		void BubbleUp();
		void BubbleDown();
		
		std::vector<T> myHeap;
	};

	template <class T>
	Heap<T>::Heap()
	{
		myHeap = std::vector<T>();
	}

	template <class T>
	Heap<T>::~Heap()
	{
		myHeap.clear();
	}

	template <class T>
	int Heap<T>::GetSize() const
	{
		return myHeap.size();
	}

	template <class T>
	void Heap<T>::Enqueue(const T& anElement)
	{
		myHeap.push_back(anElement);
		
		if (!(myHeap.size() < 1) && !(1 < myHeap.size()))
		{
			return;
		}
		else if (myHeap.size() < 4)
		{
			for (int i = myHeap.size() - 1; 0 < i; i--)
			{
				if (myHeap[0] < myHeap[i])
				{
					std::swap(myHeap[myHeap.size() - 1], myHeap[0]);
				}
			}
			return;
		}
		BubbleUp();
	}

	template <class T>
	const T& Heap<T>::GetTop() const
	{
		return myHeap[0];
	}

	template <class T>
	T Heap<T>::Dequeue()
	{
		T value = myHeap[0];
		std::swap(myHeap[0], myHeap[myHeap.size() - 1]);
		myHeap.pop_back();

		BubbleDown();
		return value;
	}

	template<class T>
	void Heap<T>::BubbleUp()
	{
		int index = myHeap.size() - 1;
		while (index != 0)
		{
			if (myHeap[static_cast<unsigned int>((index - 1) * 0.5f)] < myHeap[index])
			{
				std::swap(myHeap[static_cast<unsigned int>((index - 1) * 0.5f)], myHeap[index]);
				index = static_cast<unsigned int>((index - 1) * 0.5f);
			}
			else
			{
				return;
			}
		}
	}

	template<class T>
	void Heap<T>::BubbleDown()
	{
		int index = 0;
		while (index != myHeap.size()-1)
		{
			if (static_cast<unsigned int>(2 * index + 1) < myHeap.size() && myHeap[index] < myHeap[static_cast<unsigned int>(2 * index + 1)])
			{
				if (static_cast<unsigned int>(2 * index + 2) < myHeap.size() && myHeap[index] < myHeap[static_cast<unsigned int>(2 * index + 2)])
				{
					if (myHeap[static_cast<unsigned int>(2 * index + 1)] < myHeap[static_cast<unsigned int>(2 * index + 2)])
					{
						std::swap(myHeap[static_cast<unsigned int>(2 * index + 2)], myHeap[index]);
						index = static_cast<unsigned int>(2 * index + 2);
					}
					else
					{
						std::swap(myHeap[static_cast<unsigned int>(2 * index + 1)], myHeap[index]);
						index = static_cast<unsigned int>(2 * index + 1);
					}
				}
				else
				{
					std::swap(myHeap[static_cast<unsigned int>(2 * index + 1)], myHeap[index]);
					return;
				}
			}
			else
			{
				return;
			}
		}
	}
}