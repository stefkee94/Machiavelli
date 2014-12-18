#pragma once
#include <vector>
#include <algorithm>
template<class T>
class CardStack
{
	public:
		void addCard(T card){
			cardVector.push_back(card);
		}
		void shuffleCards(){
			std::random_shuffle(cardVector.begin(), cardVector.end());
		}
		T getCardTop(){
			T card = cardVector.front();
			cardVector.erase(cardVector.begin());
			return card;
		}
		T getCardAtIndex(int index){
			T card = cardVector[index];
			cardVector.erase(cardVector.begin() + index);
			return card;
		}
	private:
		std::vector<T> cardVector;
};