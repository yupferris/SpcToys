#include "InstrumentSerializer.h"
#include "SampleSerializer.h"

Instrument *InstrumentSerializer::Deserialize(const BsonObject *object)
{
	auto sample = SampleSerializer::Deserialize(object->GetObjectValue("sample"));

	auto ret = new Instrument(sample);

	ret->IsLooping = object->GetBoolValue("islooping");
	ret->LoopOffset = object->GetInt32Value("loopoffset");
	ret->Adsr0 = object->GetInt32Value("adsr0");
	ret->Adsr1 = object->GetInt32Value("adsr1");
	ret->Gain = object->GetInt32Value("gain");

	return ret;
}

BsonObject *InstrumentSerializer::Serialize(const Instrument *instrument)
{
	auto ret = new BsonObject();

	auto sample = instrument->GetSample();
	ret->AddObject("sample", SampleSerializer::Serialize(sample));

	ret->AddBool("islooping", instrument->IsLooping);
	ret->AddInt32("loopoffset", instrument->LoopOffset);
	ret->AddInt32("adsr0", instrument->Adsr0);
	ret->AddInt32("adsr1", instrument->Adsr1);
	ret->AddInt32("gain", instrument->Gain);

	return ret;
}
