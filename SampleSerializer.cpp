#include "SampleSerializer.h"

Sample *SampleSerializer::Deserialize(const BsonObject *object)
{
	auto ret = new Sample();

	auto blocksArray = object->GetArrayValue("blocks");
	for (int i = 0; i < blocksArray.Count(); i++)
	{
		auto blockBytes = ((BsonBinaryElement *)blocksArray[i])->GetValue();
		auto block = new Sample::Block();
		ret->Blocks.Add(block);
		block->Header = blockBytes[0];
		for (int j = 0; j < 8; j++)
			block->Data[j] = blockBytes[j + 1];
	}

	return ret;
}

BsonObject *SampleSerializer::Serialize(const Sample *sample)
{
	auto ret = new BsonObject();

	auto blockArray = new BsonArrayElement("blocks");
	ret->AddElement(blockArray);
	for (int i = 0; i < sample->Blocks.Count(); i++)
	{
		List<unsigned char> blockBytes;

		auto block = sample->Blocks[i];
		blockBytes.Add(block->Header);
		for (int j = 0; j < 8; j++)
			blockBytes.Add(block->Data[j]);

		blockArray->AddBinary(blockBytes);
	}

	return ret;
}
