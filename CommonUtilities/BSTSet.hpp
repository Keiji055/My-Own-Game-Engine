#pragma once
namespace CommonUtilities
{
	template <class T>
	class Node
	{
	public:
		Node();
		~Node();
		T myValue;
		Node* myLeft;
		Node* myRight;
	};

	template <class T>
	Node<T>::Node()
	{
		myValue = 0;
		myLeft = nullptr;
		myRight = nullptr;
	}

	template <class T>
	Node<T>::~Node()
	{
		delete myLeft;
		delete myRight;
		myLeft = nullptr;
		myRight = nullptr;
	}

	template <class T>
	class BSTSet
	{
	public:
		BSTSet();
		~BSTSet();
		bool HasElement(const T& aValue);
		void Insert(const T& aValue);
		void Remove(const T& aValue);
		bool TraverseTree(Node<T>*& aNode, const T& aValue);
		void TraverseAndRemove(Node<T>*& aNode, const T& aValue);
		void TraverseAndInsert(Node<T>*& aNode, const T& aValue);
		void RemoveLargestLeaf(Node<T>*& aNode, T& aValue);
	private:
		Node<T>* myRoot;
	};

	template <class T>
	BSTSet<T>::BSTSet()
	{
		myRoot = nullptr;
	}

	template <class T>
	BSTSet<T>::~BSTSet()
	{
		delete myRoot;
		myRoot = nullptr;
	}

	template <class T>
	bool BSTSet<T>::HasElement(const T& aValue)
	{
		if (TraverseTree(myRoot, aValue))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template <class T>
	void BSTSet<T>::Insert(const T& aValue)
	{
		if (myRoot == nullptr)
		{
			myRoot = new Node<T>();
			myRoot->myValue = aValue;
		}
		else if (HasElement(aValue))
		{
			return;
		}
		else
		{
			TraverseAndInsert(myRoot, aValue);
		}
	}

	template <class T>
	void BSTSet<T>::Remove(const T& aValue)
	{
		if (!HasElement(aValue))
		{
			return;
		}
		else
		{
			TraverseAndRemove(myRoot, aValue);
		}
	}

	template <class T>
	bool BSTSet<T>::TraverseTree(Node<T>*& aNode, const T& aValue)
	{
		if (aNode == nullptr)
		{
			return false;
		}
		else if (aValue < aNode->myValue)
		{
			if (aNode->myLeft == nullptr)
			{
				return false;
			}
			return TraverseTree(aNode->myLeft, aValue);
		}
		else if (aNode->myValue < aValue)
		{
			if (aNode->myRight == nullptr)
			{
				return false;
			}
			return TraverseTree(aNode->myRight, aValue);
		}
		return true;
	}

	template <class T>
	void BSTSet<T>::TraverseAndRemove(Node<T>*& aNode, const T& aValue)
	{
		if (aValue < aNode->myValue)
		{
			TraverseAndRemove(aNode->myLeft, aValue);
		}
		else if (aNode->myValue < aValue)
		{
			TraverseAndRemove(aNode->myRight, aValue);
		}
		else
		{
			if ((!(aNode->myValue < aValue) && !(aValue < aNode->myValue)) && aNode->myLeft == nullptr && aNode->myRight == nullptr)
			{
				aNode->myValue = 0;
				delete aNode;
				aNode = nullptr;
			}
			else
			{
				if (aNode->myLeft != nullptr && aNode->myRight != nullptr)
				{
					RemoveLargestLeaf(aNode->myLeft, aNode->myValue);
				}
				else if (aNode->myLeft != nullptr)
				{
					aNode = aNode->myLeft;
				}
				else if (aNode->myRight != nullptr)
				{
					aNode = aNode->myRight;
				}

			}

		}

	}

	template <class T>
	void BSTSet<T>::RemoveLargestLeaf(Node<T>*& aNode, T& aValue)
	{
		if (aNode->myRight == nullptr)
		{
			aValue = aNode->myValue;
			if (aNode->myLeft == nullptr)
			{
				delete aNode;
				aNode = nullptr;
			}
			else
			{
				aNode = aNode->myLeft;
			}
		}
		else
		{
			RemoveLargestLeaf(aNode->myRight, aValue);
		}
	}

	template <class T>
	void BSTSet<T>::TraverseAndInsert(Node<T>*& aNode, const T& aValue)
	{
		if (aValue < aNode->myValue)
		{
			if (aNode->myLeft == nullptr)
			{
				Node<T>* newNode = new Node<T>();
				aNode->myLeft = newNode;
				newNode->myValue = aValue;
				return;
			}
			TraverseAndInsert(aNode->myLeft, aValue);
		}
		else if (aNode->myValue < aValue)
		{
			if (aNode->myRight == nullptr)
			{
				Node<T>* newNode = new Node<T>();
				aNode->myRight = newNode;
				newNode->myValue = aValue;
				return;
			}
			TraverseAndInsert(aNode->myRight, aValue);
		}
	}
}
