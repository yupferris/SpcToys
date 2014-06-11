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
	LoopStartBlock = LoopEndBlock = 0;
}

Sample::~Sample()
{
	for (int i = 0; i < Blocks.Count(); i++)
		delete Blocks[i];
}

bool Sample::operator ==(const Sample& s) const
{
	if (IsLooping != s.IsLooping ||
		LoopStartBlock != s.LoopStartBlock ||
		LoopEndBlock != s.LoopEndBlock ||
		Blocks.Count() != s.Blocks.Count())
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
