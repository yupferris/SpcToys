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
	int Adsr0, Adsr1, Gain;
};

#endif
