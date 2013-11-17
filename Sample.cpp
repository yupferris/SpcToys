#include "Sample.h"

bool Sample::Block::operator ==(const Block& b) const
{
	if (Header != b.Header)
		return false;
	for (int i = 0; i < 8; i++)
	{
		if (Data[i] != b.Data[i])
			return false;
	}
	return true;
}

bool Sample::Block::operator !=(const Block& b) const
{
	return !(*this == b);
}

Sample::~Sample()
{
	for (int i = 0; i < Blocks.Count(); i++)
		delete Blocks[i];
}

bool Sample::operator ==(const Sample& s) const
{
	if (Blocks.Count() != s.Blocks.Count())
		return false;
	for (int i = 0; i < Blocks.Count(); i++)
	{
		if (*Blocks[i] != *s.Blocks[i])
			return false;
	}
	return true;
}

bool Sample::operator !=(const Sample& s) const
{
	return !(*this == s);
}

Sample *Sample::Deserialize(const BsonDocument *document)
{
	auto ret = new Sample();

	auto blocksArray = document->GetArrayValue("blocks");
	for (int i = 0; i < blocksArray.Count(); i++)
	{
		auto blockBytes = ((BsonBinaryElement *)blocksArray[i])->GetValue();
		auto block = new Block();
		ret->Blocks.Add(block);
		block->Header = blockBytes[0];
		for (int j = 0; j < 8; j++)
			block->Data[j] = blockBytes[j + 1];
	}

	return ret;
}

BsonDocument *Sample::Serialize()
{
	auto ret = new BsonDocument();

	auto blockArray = new BsonArrayElement("blocks");
	ret->AddElement(blockArray);
	for (int i = 0; i < Blocks.Count(); i++)
	{
		List<unsigned char> blockBytes;

		auto block = Blocks[i];
		blockBytes.Add(block->Header);
		for (int j = 0; j < 8; j++)
			blockBytes.Add(block->Data[j]);

		blockArray->AddBinary(blockBytes);
	}

	return ret;
}
