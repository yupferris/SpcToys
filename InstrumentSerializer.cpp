#include "InstrumentSerializer.h"
#include "SampleSerializer.h"

Instrument *InstrumentSerializer::Deserialize(const BsonDocument *document)
{
	auto sample = SampleSerializer::Deserialize(document->GetDocumentValue("sample"));

	auto ret = new Instrument(sample);

	ret->IsLooping = document->GetBoolValue("islooping");
	ret->LoopOffset = document->GetInt32Value("loopoffset");
	ret->Adsr0 = document->GetInt32Value("adsr0");
	ret->Adsr1 = document->GetInt32Value("adsr1");
	ret->Gain = document->GetInt32Value("gain");

	return ret;
}

BsonDocument *InstrumentSerializer::Serialize(const Instrument *instrument)
{
	auto ret = new BsonDocument();

	auto sample = instrument->GetSample();
	ret->AddDocument("sample", SampleSerializer::Serialize(sample));

	ret->AddBool("islooping", instrument->IsLooping);
	ret->AddInt32("loopoffset", instrument->LoopOffset);
	ret->AddInt32("adsr0", instrument->Adsr0);
	ret->AddInt32("adsr1", instrument->Adsr1);
	ret->AddInt32("gain", instrument->Gain);

	return ret;
}
