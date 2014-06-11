#include "InstrumentSerializer.h"
#include "SampleSerializer.h"

Instrument *InstrumentSerializer::Deserialize(const BsonObject *object)
{
	auto ret = new Instrument();

	ret->Sample = SampleSerializer::Deserialize(object->GetObjectValue("sample"));
	ret->Adsr0 = object->GetInt32Value("adsr0");
	ret->Adsr1 = object->GetInt32Value("adsr1");
	ret->Gain = object->GetInt32Value("gain");

	return ret;
}

BsonObject *InstrumentSerializer::Serialize(const Instrument *instrument)
{
	auto ret = new BsonObject();

	ret->AddObject("sample", SampleSerializer::Serialize(instrument->Sample));
	ret->AddInt32("adsr0", instrument->Adsr0);
	ret->AddInt32("adsr1", instrument->Adsr1);
	ret->AddInt32("gain", instrument->Gain);

	return ret;
}
