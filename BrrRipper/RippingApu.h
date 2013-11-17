#ifndef __RIPPINGAPU_H__
#define __RIPPINGAPU_H__

#include "../Common.h"
#include "../Instrument.h"

class RippingApu : public Apu
{
public:
	virtual ~RippingApu();

	List<Instrument *> Instruments;

	virtual void WriteByte(unsigned int address, unsigned char value);

	bool Contains(Instrument *instrument) const;
};

#endif
