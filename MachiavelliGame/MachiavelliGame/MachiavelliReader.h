#pragma once
#include <string>
#include <vector>

class MachiavelliReader
{
public:
	MachiavelliReader();
	virtual ~MachiavelliReader();

	void read_character_cards();
	void read_building_cards();
};

