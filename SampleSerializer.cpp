#include "SampleSerializer.h"

Sample *SampleSerializer::Deserialize(const BsonObject *object)
{
	auto ret = new Sample();

	auto blockData = object->GetBinaryValue("blockdata");
	for (int i = 0; i < blockData.Count(); i += 9)
	{
		auto block = new Sample::Block();
		ret->Blocks.Add(block);
		block->Header = blockData[i];
		for (int j = 0; j < 8; j++)
			block->Data[j] = blockData[i + j + 1];
	}

	object->TryGetElementValue("islooping", ret->IsLooping);
	object->TryGetElementValue("loopstartblock", ret->LoopStartBlock);
	object->TryGetElementValue("loopendblock", ret->LoopEndBlock);

	return ret;
}

BsonObject *SampleSerializer::Serialize(const Sample *sample)
{
	auto ret = new BsonObject();
	
	List<unsigned char> blockData;
	for (int i = 0; i < sample->Blocks.Count(); i++)
	{
		auto block = sample->Blocks[i];
		blockData.Add(block->Header);
		for (int j = 0; j < 8; j++)
			blockData.Add(block->Data[j]);
	}
	ret->AddBinary("blockdata", blockData);

	ret->AddBool("islooping", sample->IsLooping);
	ret->AddInt32("loopstartblock", sample->LoopStartBlock);
	ret->AddInt32("loopendblock", sample->LoopEndBlock);

	return ret;
}
