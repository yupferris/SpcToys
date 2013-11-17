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

Sample::Sample()
{
	IsLooping = false;
	LoopOffset = 0;
	Adsr0 = Adsr1 = Gain = 0;
}

Sample::~Sample()
{
	for (int i = 0; i < Blocks.Count(); i++)
		delete Blocks[i];
}

Sample *Sample::Load(const String& fileName)
{
	auto ret = new Sample();

	FileStream input(fileName, FileStream::FileMode::OpenRead);
	BinaryReader reader(&input);

	int numBlocks = reader.ReadInt32();
	for (int i = 0; i < numBlocks; i++)
	{
		auto block = new Block();
		ret->Blocks.Add(block);
		block->Header = reader.ReadByte();
		for (int j = 0; j < 8; j++)
			block->Data[j] = reader.ReadByte();
	}
	ret->IsLooping = reader.ReadInt32() != 0;
	ret->LoopOffset = reader.ReadInt32();
	ret->Adsr0 = reader.ReadByte();
	ret->Adsr1 = reader.ReadByte();
	ret->Gain = reader.ReadByte();

	return ret;
}

bool Sample::operator ==(const Sample& s) const
{
	if (IsLooping != s.IsLooping ||
		LoopOffset != s.LoopOffset ||
		Adsr0 != s.Adsr0 ||
		Adsr1 != s.Adsr1 ||
		Gain != s.Gain)
		return false;
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

void Sample::Save(const String& fileName)
{
	FileStream output(fileName, FileStream::FileMode::OpenWrite);
	BinaryWriter writer(&output);

	writer.WriteInt32(Blocks.Count());
	for (int i = 0; i < Blocks.Count(); i++)
	{
		auto block = Blocks[i];
		writer.WriteByte(block->Header);
		for (int j = 0; j < 8; j++)
			writer.WriteByte(block->Data[j]);
	}
	writer.WriteInt32(IsLooping ? 1 : 0);
	writer.WriteInt32(LoopOffset);
	writer.WriteByte(Adsr0);
	writer.WriteByte(Adsr1);
	writer.WriteByte(Gain);
}
