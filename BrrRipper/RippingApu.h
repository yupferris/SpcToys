#ifndef __RIPPINGAPU_H__
#define __RIPPINGAPU_H__

#include "../Common.h"
#include "../Sample.h"

class RippingApu : public Apu
{
public:
	virtual ~RippingApu();

	List<Sample *> Samples;

	virtual void WriteByte(unsigned int address, unsigned char value);

	bool Contains(Sample *sample) const;
};

#endif
