#ifndef __INSTRUMENTSERIALIZER_H__
#define __INSTRUMENTSERIALIZER_H__

#include "Common.h"
#include "Instrument.h"

class InstrumentSerializer
{
public:
	static Instrument *Deserialize(const BsonDocument *document);
	static BsonDocument *Serialize(const Instrument *instrument);
};

#endif
