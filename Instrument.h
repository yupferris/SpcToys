#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include "Common.h"
#include "Sample.h"

class Instrument
{
public:
	Instrument();
	~Instrument();

	bool operator ==(const Instrument& s) const;
	bool operator !=(const Instrument& s) const;

	Sample *Sample;

	bool IsLooping;
	int LoopOffset;
	unsigned char Adsr0, Adsr1, Gain;
};

#endif
