#include "../Common.h"
#include "../Instrument.h"
#include "../InstrumentSerializer.h"

#include <Windows.h>

#include <functional>
using namespace std;

class Basher : IEmulator8
{
public:
	Basher(IAudioDriver *driver)
	{
		mutex = Mutex::Create();

		ram = new unsigned char[0x10000];
		for (int i = 0; i < 0x10000; i++)
			ram[i] = 0xff;

		dsp = new Dsp(this);

		dsp->SetRegister(0x0c, 0x7f); // Vol l
		dsp->SetRegister(0x1c, 0x7f); // Vol r
		dsp->SetRegister(0x2c, 0x00); // Echo vol l
		dsp->SetRegister(0x3c, 0x00); // Echo vol r
		dsp->SetRegister(0x6c, 0x20); // Flg
		dsp->SetRegister(0x5d, 0x00); // Source dir

		const int bufferSize = driver->GetSampleRate() * 2;
		left = new short[bufferSize];
		right = new short[bufferSize];
		driver->SetRenderCallback(renderCallbackRouter, this);

		currentSampleAddress = 0x100;
	}

	~Basher()
	{
		delete ram;
		delete dsp;
		delete [] left;
		delete [] right;

		delete mutex;
	}

	void LoadInstrument(Instrument *instrument, int voiceIndex)
	{
		mutex->Lock();

		auto sample = instrument->Sample;

		const int sampleAddress = currentSampleAddress;
		const int loopAddress = sampleAddress + sample->LoopStartBlock * 9;
		int dirAddress = voiceIndex * 4;
		WriteByte(dirAddress, sampleAddress & 0xff);
		WriteByte(dirAddress + 1, (sampleAddress >> 8) & 0xff);
		WriteByte(dirAddress + 2, loopAddress & 0xff);
		WriteByte(dirAddress + 3, (loopAddress >> 8) & 0xff);

		int address = sampleAddress;
		for (int i = 0; i < sample->Blocks.Count(); i++)
		{
			auto block = sample->Blocks[i];
			WriteByte(address++, block->Header);
			for (int j = 0; j < 8; j++)
				WriteByte(address++, block->Data[j]);
		}

		currentSampleAddress = address;

		dsp->SetRegister(voiceIndex * 0x10 + 0x00, 0x7f); // Voice vol l
		dsp->SetRegister(voiceIndex * 0x10 + 0x01, 0x7f); // Voice vol r
		dsp->SetRegister(voiceIndex * 0x10 + 0x02, 0x00); // Voice pitch low
		dsp->SetRegister(voiceIndex * 0x10 + 0x03, 0x10); // Voice pitch high
		dsp->SetRegister(voiceIndex * 0x10 + 0x04, voiceIndex); // Voice source
		dsp->SetRegister(voiceIndex * 0x10 + 0x05, instrument->Adsr0);
		dsp->SetRegister(voiceIndex * 0x10 + 0x06, instrument->Adsr1);
		dsp->SetRegister(voiceIndex * 0x10 + 0x07, instrument->Gain);

		mutex->Unlock();
	}

	void Bash(unsigned char voiceMask)
	{
		mutex->Lock();
		dsp->SetRegister(0x4c, voiceMask);
		mutex->Unlock();
	}

	void Unbash(unsigned char voiceMask)
	{
		mutex->Lock();
		dsp->SetRegister(0x5c, voiceMask);
		mutex->Unlock();
	}

	virtual void Reset()
	{
	}

	virtual void CpuCyclesCallback(int numCycles)
	{
	}

	virtual unsigned char ReadByte(unsigned int address)
	{
		return ram[address & 0xffff];
	}

	virtual void WriteByte(unsigned int address, unsigned char value)
	{
		ram[address & 0xffff] = value;
	}

private:
	static void renderCallbackRouter(float *leftBuffer, float *rightBuffer, int numSamples, void *userData)
	{
		if (userData)
			((Basher *)userData)->renderCallback(leftBuffer, rightBuffer, numSamples);
	}

	void renderCallback(float *leftBuffer, float *rightBuffer, int numSamples)
	{
		mutex->Lock();

		dsp->SetOutputBuffers(left, right);
		dsp->CyclesCallback(numSamples * 64);
		dsp->Flush();

		for (int i = 0; i < numSamples; i++)
		{
			leftBuffer[i] = (float)left[i] / 32768.0f;
			rightBuffer[i] = (float)right[i] / 32768.0f;
		}

		mutex->Unlock();
	}

	Mutex *mutex;
	unsigned char *ram;
	Dsp *dsp;
	short *left, *right;
	int currentSampleAddress;
};

class KeyTrigger
{
public:
	KeyTrigger(int vKey, function<void()> downCallback, function<void()> upCallback)
	{
		this->vKey = vKey;
		this->downCallback = downCallback;
		this->upCallback = upCallback;

		isTriggered = false;
	}

	void Update()
	{
		if (GetAsyncKeyState(vKey))
		{
			if (!isTriggered)
			{
				downCallback();
				isTriggered = true;
			}
		}
		else
		{
			if (isTriggered)
			{
				upCallback();
				isTriggered = false;
			}
		}
	}

private:
	int vKey;
	function<void()> downCallback;
	function<void()> upCallback;

	bool isTriggered;
};

int Main(const List<String>& arguments)
{
	try
	{
		if (!arguments.Count())
			throw FSL_EXCEPTION("Need some ins', fucktard");

		List<Instrument *> instruments;

		Console::WriteLine("Loading instruments...");
		for (int i = 0; i < arguments.Count(); i++)
		{
			auto obj = BsonSerializer::Deserialize(arguments[i]);
			auto instrument = InstrumentSerializer::Deserialize(obj);
			instruments.Add(instrument);
			delete obj;
		}

		Console::WriteLine("Initializing audio driver...");
		auto driver = AudioDriverFactory::CreateDefault();
		driver->SetSampleRate(32000);

		Console::WriteLine("Initializing basher...");
		Basher basher(driver);
		for (int i = 0; i < instruments.Count(); i++)
			basher.LoadInstrument(instruments[i], i);

		Console::WriteLine("And now we blast like there's no tomorrow :)");
		Console::WriteLine("ESC quits.");
		const char chars[] = { '-', '/', '|', '\\' };
		const int numChars = 4;
		int charIndex = 0;
		List<KeyTrigger *> keyTriggers;
		keyTriggers.Add(new KeyTrigger(VK_F1, [&] { basher.Bash(0x01); }, [&] { basher.Unbash(0x01); }));
		keyTriggers.Add(new KeyTrigger(VK_F2, [&] { basher.Bash(0x02); }, [&] { basher.Unbash(0x02); }));
		keyTriggers.Add(new KeyTrigger(VK_F3, [&] { basher.Bash(0x04); }, [&] { basher.Unbash(0x04); }));
		keyTriggers.Add(new KeyTrigger(VK_F4, [&] { basher.Bash(0x08); }, [&] { basher.Unbash(0x08); }));
		keyTriggers.Add(new KeyTrigger(VK_F5, [&] { basher.Bash(0x10); }, [&] { basher.Unbash(0x10); }));
		keyTriggers.Add(new KeyTrigger(VK_F6, [&] { basher.Bash(0x20); }, [&] { basher.Unbash(0x20); }));
		keyTriggers.Add(new KeyTrigger(VK_F7, [&] { basher.Bash(0x40); }, [&] { basher.Unbash(0x40); }));
		keyTriggers.Add(new KeyTrigger(VK_F8, [&] { basher.Bash(0x80); }, [&] { basher.Unbash(0x80); }));
		while (!GetAsyncKeyState(VK_ESCAPE))
		{
			Console::Write(String("\r[") + chars[charIndex = (charIndex + 1) % numChars] + "]");

			for (int i = 0; i < keyTriggers.Count(); i++)
				keyTriggers[i]->Update();

			Threading::Sleep(3);
		}
		Console::Write("\r   \r");

		for (int i = 0; i < keyTriggers.Count(); i++)
			delete keyTriggers[i];

		delete driver;

		for (int i = 0; i < instruments.Count(); i++)
			delete instruments[i];
	}
	catch (const Exception& e)
	{
		Console::WriteLine("ERROR: " + e.GetMsg());
		return 1;
	}

	return 0;
}
