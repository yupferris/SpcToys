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

	static Sample *Load(const String& fileName);

	Sample();
	~Sample();

	bool operator ==(const Sample& s) const;
	bool operator !=(const Sample& s) const;

	void Save(const String& fileName);

	List<Block *> Blocks;
	bool IsLooping;
	int LoopOffset;

	// TODO: This is really instrument data, not sample data
	unsigned char Adsr0, Adsr1, Gain;
};

#endif
