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

Instrument *Instrument::Deserialize(const BsonDocument *document)
{
	auto sample = Sample::Deserialize(document->GetDocumentValue("sample"));

	auto ret = new Instrument(sample);

	ret->IsLooping = document->GetBoolValue("islooping");
	ret->LoopOffset = document->GetInt32Value("loopoffset");
	ret->Adsr0 = document->GetInt32Value("adsr0");
	ret->Adsr1 = document->GetInt32Value("adsr1");
	ret->Gain = document->GetInt32Value("gain");

	return ret;
}

BsonDocument *Instrument::Serialize()
{
	auto ret = new BsonDocument();

	ret->AddDocument("sample", sample->Serialize());

	ret->AddBool("islooping", IsLooping);
	ret->AddInt32("loopoffset", LoopOffset);
	ret->AddInt32("adsr0", Adsr0);
	ret->AddInt32("adsr1", Adsr1);
	ret->AddInt32("gain", Gain);

	return ret;
}

Sample *Instrument::GetSample() const
{
	return sample;
}
