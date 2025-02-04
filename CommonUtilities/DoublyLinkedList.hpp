#pragma once

namespace CommonUtilities
{
	template<class T>
	class DoublyLinkedList;

	template <class T>
	class DoublyLinkedListNode
	{
	public:
		DoublyLinkedListNode(const DoublyLinkedListNode<T>&) = delete;
		DoublyLinkedListNode<T>& operator=(const DoublyLinkedListNode<T>&) = delete;

		const T& GetValue() const;
		T& GetValue();
		DoublyLinkedListNode<T>* GetNext() const;
		DoublyLinkedListNode<T>* GetPrevious() const;
		void SetNext(DoublyLinkedListNode<T>& aNode);
		void SetPrevious(DoublyLinkedListNode<T>& aNode);
	private:
		friend class DoublyLinkedList<T>;
		DoublyLinkedListNode(const T& aValue);
		~DoublyLinkedListNode() {}

		DoublyLinkedListNode<T>* myPrevious;
		DoublyLinkedListNode<T>* myNext;
		T myValue;
	};

	template <class T>
	DoublyLinkedListNode<T>::DoublyLinkedListNode(const T& aValue)
	{
		myValue = aValue;
		myNext = nullptr;
		myPrevious = nullptr;
	}

	template <class T>
	const T& DoublyLinkedListNode<T>::GetValue() const
	{
		return myValue;
	}

	template <class T>
	T& DoublyLinkedListNode<T>::GetValue()
	{
		return myValue;
	}

	template <class T>
	DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetNext() const
	{
		return myNext;
	}

	template <class T>
	DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetPrevious() const
	{
		return myPrevious;
	}

	template <class T>
	void DoublyLinkedListNode<T>::SetNext(DoublyLinkedListNode<T>& aNode)
	{
		myNext = &aNode;
	}

	template <class T>
	void DoublyLinkedListNode<T>::SetPrevious(DoublyLinkedListNode<T>& aNode)
	{
		myPrevious = &aNode;
	}

	template <class T>
	class DoublyLinkedList
	{
	public:
		DoublyLinkedList();	
		~DoublyLinkedList();

		int GetSize() const;

		DoublyLinkedListNode<T>* GetFirst();
		DoublyLinkedListNode<T>* GetLast();
		
		void InsertFirst(const T& aValue);
		
		void InsertLast(const T& aValue);

		void InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue);

		void InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue);

		void Remove(DoublyLinkedListNode<T>* aNode);

		DoublyLinkedListNode<T>* FindFirst(const T& aValue);
		
		DoublyLinkedListNode<T>* FindLast(const T& aValue);
		
		bool RemoveFirst(const T& aValue);
		
		bool RemoveLast(const T& aValue);

	private:
		DoublyLinkedListNode<T>* myFirst;
		DoublyLinkedListNode<T>* myLast;
		int mySize;
	};

	template<class T>
	DoublyLinkedList<T>::DoublyLinkedList()
	{
		mySize = 0;
		myFirst = nullptr;
		myLast = nullptr;
	}

	template<class T>
	DoublyLinkedList<T>::~DoublyLinkedList()
	{
		DoublyLinkedListNode<T>* nodeToDelete = myLast;
		while (myLast != nullptr)
		{
			myLast = myLast->myPrevious;
			delete nodeToDelete;
			nodeToDelete = myLast;
		}
	}

	template <class T>
	int DoublyLinkedList<T>::GetSize() const 
	{
		return mySize;
	}

	template<class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetFirst()
	{
		return myFirst;
	}

	template <class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetLast()
	{
		return myLast;
	}

	template <class T>
	void DoublyLinkedList<T>::InsertFirst(const T& aValue)
	{
		mySize++;
		DoublyLinkedListNode<T>* temp = new DoublyLinkedListNode<T>(aValue);
		if (mySize == 1)
		{
			myFirst = temp;
			myLast = temp;
		}
		else
		{
			temp->myNext = myFirst;
			myFirst->myPrevious = temp;
			myFirst = temp;
		}
	}

	template <class T>
	void DoublyLinkedList<T>::InsertLast(const T& aValue)
	{
		mySize++;
		DoublyLinkedListNode<T>* temp = new DoublyLinkedListNode<T>(aValue);
		if (mySize == 1)
		{
			myFirst = temp;
			myLast = temp;
		}
		else
		{
			temp->myPrevious = myLast;
			myLast->myNext = temp;
			myLast = temp;
		}
	}

	template <class T>
	void DoublyLinkedList<T>::InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		mySize++;
		DoublyLinkedListNode<T>* temp = new DoublyLinkedListNode<T>(aValue);
		if (aNode == myFirst)
		{
			temp->myNext = aNode;
			aNode->myPrevious = temp;
			temp->myPrevious = nullptr;
			myFirst = temp;
		}
		else
		{
			temp->myPrevious = aNode->myPrevious;
			temp->myNext = aNode;
			aNode->myPrevious->myNext = temp;
			aNode->myPrevious = temp;
		}
	}

	template <class T>
	void DoublyLinkedList<T>::InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		mySize++;
		DoublyLinkedListNode<T>* temp = new DoublyLinkedListNode<T>(aValue);
		if (aNode == myLast)
		{
			temp->myPrevious = aNode;
			temp->myNext = nullptr;
			aNode->myNext = temp;
			myLast = temp;
		}
		else
		{
			temp->myNext = aNode->myNext;
			temp->myPrevious = aNode;
			aNode->myNext->myPrevious = temp;
			aNode->myNext = temp;
		}
	}

	template <class T>
	void DoublyLinkedList<T>::Remove(DoublyLinkedListNode<T>* aNode)
	{
		if (mySize > 1)
		{
			if (aNode == myFirst)
			{
				aNode->myNext->myPrevious = nullptr;
				myFirst = aNode->myNext;
			}
			else if (aNode == myLast)
			{
				aNode->myPrevious->myNext = nullptr;
				myLast = aNode->myPrevious;
			}
			else
			{
				aNode->myPrevious->myNext = aNode->myNext;
				aNode->myNext->myPrevious = aNode->myPrevious;
			}
		}
		else
		{
			myFirst = nullptr;
			myLast = nullptr;
		}
		mySize--;
		delete aNode;
	}

	template <class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* temp = myFirst;
		while (temp != nullptr)
		{
			if (temp->myValue == aValue)
			{
				return temp;
			}
			temp = temp->myNext;
		}
		return nullptr;
	}

	template <class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindLast(const T& aValue)
	{
		DoublyLinkedListNode<T>* temp = myLast;
		while (temp != nullptr)
		{
			if (temp->myValue == aValue)
			{
				return temp;
			}
			temp = temp->myPrevious;
		}
		return nullptr;
	}

	template <class T>
	bool DoublyLinkedList<T>::RemoveFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* temp = FindFirst(aValue);

		if (temp != nullptr)
		{
			Remove(temp);
			return true;
		}
		return false;
	}

	template <class T>
	bool DoublyLinkedList<T>::RemoveLast(const T& aValue)
	{
		DoublyLinkedListNode<T>* temp = FindLast(aValue);

		if (temp != nullptr)
		{
			Remove(temp);
			return true;
		}
		return false;
	}
}