#include "Instrument.h"

Instrument::Instrument(Sample *sample)
{
	this->sample = sample;

	IsLooping = false;
	LoopOffset = 0;
	Adsr0 = Adsr1 = Gain = 0;
}

Instrument::~Instrument()
{
	delete sample;
}

bool Instrument::operator ==(const Instrument& i) const
{
	if (*sample != *i.sample ||
		IsLooping != i.IsLooping ||
		LoopOffset != i.LoopOffset ||
		Adsr0 != i.Adsr0 ||
		Adsr1 != i.Adsr1 ||
		Gain != i.Gain)
		return false;
	return true;
}

bool Instrument::operator !=(const Instrument& i) const
{
	return !(*this == i);
}

Sample *Instrument::GetSample() const
{
	return sample;
}
