#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include "Common.h"

class Sample
{
public:
	class Block
	{
	public:
		bool operator ==(const Block& b) const;
		bool operator !=(const Block& b) const;

		unsigned char Header;
		unsigned char Data[8];
	};

	~Sample();

	bool operator ==(const Sample& s) const;
	bool operator !=(const Sample& s) const;

	List<Block *> Blocks;
};

#endif
