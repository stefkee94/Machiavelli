#pragma once
#include <string>
#include <vector>

#include "MachiavelliReader.h"

template <typename T>
class CardStack
{
public:
	CardStack();
	virtual ~CardStack();

	void read_character_cards();
	void read_building_cards();

private:
	MachiavelliReader reader;
};

template <typename T>
CardStack<T>::CardStack()
{
	reader = MachiavelliReader();
}

template <typename T>
void CardStack<T>::read_character_cards()
{
	reader.read_building_cards();
}

template <typename T>
void CardStack<T>::read_building_cards()
{

}

template <typename T>
CardStack<T>::~CardStack()
{
}
