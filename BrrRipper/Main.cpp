#include "../Common.h"
#include "RippingApu.h"

int Main(const List<String>& arguments)
{
	try
	{
		if (arguments.Count() != 2)
			throw FSL_EXCEPTION("Need an spc and an output dir, fucktard");

		auto spcFileName = arguments[0];
		auto outDir = arguments[1];

		Console::WriteLine("Loading SPC...");
		auto spc = Spc::Load(spcFileName);
		auto title = spc->GetId666Tag()->GetSongTitle();

		Console::WriteLine("Initializing APU...");
		RippingApu apu;
		apu.SetState(*spc);

		const int sampleRate = 32000;
		const int runLengthSeconds = 180;

		const int bufferSize = sampleRate * 2;
		auto leftBuffer = new short[bufferSize];
		auto rightBuffer = new short[bufferSize];

		Console::WriteLine("Running...");
		const char chars[] = { '-', '/', '|', '\\' };
		const int numChars = 4;
		int charIndex = 0;
		for (int i = 0; i < runLengthSeconds * 2; i++)
		{
			apu.Render(leftBuffer, rightBuffer, bufferSize / 2);
			Console::Write(String("\r[") + chars[charIndex = (charIndex + 1) % numChars] + "]");
		}
		Console::Write("\r   \r");

		Console::WriteLine("Dumping samples...");
		for (int i = 0; i < apu.Instruments.Count(); i++)
		{
			auto doc = apu.Instruments[i]->Serialize();
			BsonSerializer::Serialize(outDir + title + " " + i + ".ins", doc);
			delete doc;
		}

		delete [] leftBuffer;
		delete [] rightBuffer;

		delete spc;
	}
	catch (const Exception& e)
	{
		Console::WriteLine("ERROR: " + e.GetMsg());
		return 1;
	}

	return 0;
}
