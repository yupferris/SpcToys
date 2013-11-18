#ifndef __INSTRUMENTSERIALIZER_H__
#define __INSTRUMENTSERIALIZER_H__

#include "Common.h"
#include "Instrument.h"

class InstrumentSerializer
{
public:
	static Instrument *Deserialize(const BsonObject *object);
	static BsonObject *Serialize(const Instrument *instrument);
};

#endif
