#include "Instrument.h"

Instrument::Instrument()
{
	Sample = nullptr;
	Adsr0 = Adsr1 = Gain = 0;
}

Instrument::~Instrument()
{
	if (Sample)
		delete Sample;
}

bool Instrument::operator ==(const Instrument& i) const
{
	if (*Sample != *i.Sample ||
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
