#include "RippingApu.h"

RippingApu::~RippingApu()
{
	for (int i = 0; i < Instruments.Count(); i++)
		delete Instruments[i];
}

void RippingApu::WriteByte(unsigned int address, unsigned char value)
{
	if ((address & 0xffff) == 0xf3 && GetDspRegAddress() == 0x4c && value)
	{
		auto dsp = GetDsp();
		int dirAddress = dsp->GetSourceDir() * 0x100;

		unsigned char voiceMask = value;
		for (int i = 0; i < Dsp::NumVoices; i++)
		{
			if (voiceMask & 1)
			{
				auto voice = dsp->GetVoice(i);
				auto envelope = voice->GetEnvelope();

				auto sample = new Sample();
				auto instrument = new Instrument(sample);

				int entryAddress = dirAddress + voice->GetSource() * 4;
				int sampleStartAddress = ReadByte(entryAddress);
				sampleStartAddress |= ReadByte(entryAddress + 1) << 8;
				int loopStartAddress = ReadByte(entryAddress + 2);
				loopStartAddress |= ReadByte(entryAddress + 3) << 8;

				int currentAddress = sampleStartAddress;
				while (true)
				{
					auto block = new Sample::Block();
					block->Header = ReadByte(currentAddress++);
					for (int i = 0; i < 8; i++)
						block->Data[i] = ReadByte(currentAddress++);
					sample->Blocks.Add(block);

					bool isEnd = (block->Header & 0x01) != 0;
					bool isLooping = (block->Header & 0x02) != 0;
					if (isEnd)
					{
						if (isLooping)
						{
							instrument->IsLooping = true;
							instrument->LoopOffset = loopStartAddress - sampleStartAddress;
						}
						break;
					}
				}

				instrument->Adsr0 = envelope->GetAdsr0();
				instrument->Adsr1 = envelope->GetAdsr1();
				instrument->Gain = envelope->GetGain();

				if (!Contains(instrument))
				{
					Instruments.Add(instrument);
				}
				else
				{
					delete sample;
				}
			}
			voiceMask >>= 1;
		}
	}

	Apu::WriteByte(address, value);
}

bool RippingApu::Contains(Instrument *instrument) const
{
	for (int i = 0; i < Instruments.Count(); i++)
	{
		if (*Instruments[i] == *instrument)
			return true;
	}
	return false;
}
