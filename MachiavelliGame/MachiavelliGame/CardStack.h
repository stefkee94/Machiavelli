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
		T get_card_at_index(int index);

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
void CardStack<T>::shuffle_cards()
{
	std::random_shuffle(card_vector.begin(), card_vector.end());
}

template<typename T>
T CardStack<T>::get_card_at_top()
{
	T card = card_vector.front();
	card_vector.erase(card_vector.begin());

	return card;
}

template<typename T>
T CardStack<T>::get_card_at_index(int index)
{
	T card = card_vector[index];
	card_vector.erase(card_vector.begin() + index);

	return card;
}