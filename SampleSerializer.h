#ifndef __SAMPLESERIALIZER_H__
#define __SAMPLESERIALIZER_H__

#include "Sample.h"

class SampleSerializer
{
public:
	static Sample *Deserialize(const BsonObject *object);
	static BsonObject *Serialize(const Sample *sample);
};

#endif
