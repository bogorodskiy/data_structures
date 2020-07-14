#pragma once
template<typename K, typename V>
class Dictionary
{

public:
	void insert(K* key, V* value);
	void remove(K* key);
	V* get(K* key);
	bool isEmpty();
	void clear();

private:
	template<K, V>
	class DictionaryNode
	{
	public:
		K* key;
		V* value;

		DictionaryNode* parent;
		DictionaryNode* leftChild;
		DictionaryNode* rightChild;
	};



};

