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

		unsigned char voiceMask = value;
		for (int i = 0; i < Dsp::NumVoices; i++)
		{
			if (voiceMask & 1)
			{
				auto voice = dsp->GetVoice(i);
				auto envelope = voice->GetEnvelope();

				auto instrument = new Instrument();

				auto sample = new Sample();
				instrument->Sample = sample;

				int source = voice->GetSource();
				unsigned int sampleStartAddress = dsp->ReadSourceDirStartAddress(source);
				unsigned int loopStartAddress = dsp->ReadSourceDirLoopAddress(source);

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
							sample->IsLooping = true;
							sample->LoopStartBlock = (loopStartAddress - sampleStartAddress) / 9;
							sample->LoopEndBlock = (currentAddress - 9 - sampleStartAddress) / 9;
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
