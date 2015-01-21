#pragma once
#include <vector>
#include <algorithm>

template<typename T>
class CardStack
{
	public:
		void add_card(T card);
		void shuffle_cards();
		T get_card_at_top();
		T get_card_and_remove_at_index(int index);
		int size();
		T get_card_at(int i);
		void clear();
		bool is_empty();

	private:
		std::vector<T> card_vector;
};

//implementation of template class

template<typename T>
void CardStack<T>::add_card(T card)
{
	card_vector.push_back(card);
}


template<typename T>
bool CardStack<T>::is_empty()
{
	return card_vector.empty();
}

template<typename T>
void CardStack<T>::shuffle_cards()
{
	std::random_shuffle(card_vector.begin(), card_vector.end());
}

template<typename T>
int CardStack<T>::size()
{
	return card_vector.size();
}

template<typename T>
T CardStack<T>::get_card_at(int i)
{
	if (i < card_vector.size())
	{
		return card_vector[i];
	}
}

template<typename T>
T CardStack<T>::get_card_at_top()
{
	T card = card_vector.front();
	card_vector.erase(card_vector.begin());

	return card;
}

template<typename T>
T CardStack<T>::get_card_and_remove_at_index(int index)
{
	if (index < card_vector.size())
	{
		T card = card_vector[index];
		card_vector.erase(card_vector.begin() + index);

		return card;
	}
	return nullptr;
}

template<typename T>
void CardStack<T>::clear()
{
	card_vector.clear();
}