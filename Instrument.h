#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include "Common.h"
#include "Sample.h"

class Instrument
{
public:
	Instrument(Sample *sample);
	~Instrument();

	bool operator ==(const Instrument& s) const;
	bool operator !=(const Instrument& s) const;

	static Instrument *Deserialize(const BsonDocument *document);
	BsonDocument *Serialize();

	Sample *GetSample() const;

	bool IsLooping;
	int LoopOffset;
	unsigned char Adsr0, Adsr1, Gain;

private:
	Sample *sample;
};

#endif
