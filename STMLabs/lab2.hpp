#pragma once

#include "utils.hpp"

namespace STMLab2
{
	void Ex1n2()
	{
		std::string inputPath = "input/beep.wav";

		Utils::WaveStr soundFile = Utils::WaveStr::LoadFromFile(inputPath);

		std::cout << inputPath << std::endl;
		std::cout << "File size: " << Utils::bytesToHumanReadable(soundFile.m_chunkSize) << std::endl;
		std::cout << "Format: " << soundFile.m_format << std::endl;
		std::cout << "Audio format: " << soundFile.m_audioFormat << std::endl;
		std::cout << "Number of channels: " << soundFile.m_numChannels << std::endl;
		std::cout << "Sample rate: " << soundFile.m_sampleRate << std::endl;
		std::cout << "Bits per sample: " << soundFile.m_bitsPerSample << std::endl;
		std::cout << "Data: " << std::endl;

		for (int i = 0; i < 100; ++i)
		{
			std::cout << soundFile.m_dataLong[i] << " ";
		}

		std::cout << std::endl << "Interrupted after 100 entries." << std::endl;


	}

	void Ex2()
	{


		Utils::WaveStr sound2a(1, 16, 8000);

		std::vector<short> data2a;

		int sineFreq = 400;
		int sineMaxAmpl = 15000;
		for (int i = 0; i < 16000; ++i)
		{
			data2a.push_back(std::sin((sineFreq * 2 * 3.14 / 16000) * (i + 1)) * sineMaxAmpl);
		}

		sound2a.SetDataLong(data2a);

		sound2a.WriteToFile("output/ex2a.wav");


		Utils::WaveStr sound2b(1, 16, 8000);

		std::vector<short> data2b;
		sineFreq = 1000;
		sineMaxAmpl = 10000;
		for (int i = 0; i < 16000; ++i)
		{
			data2b.push_back(std::sin((sineFreq * 2 * 3.14 / 16000) * (i + 1)) * sineMaxAmpl);
		}

		sound2b.SetDataLong(data2b);

		sound2b.WriteToFile("output/ex2b.wav");


		Utils::WaveStr sound2c(1, 16, 8000);

		std::vector<short> data2c;
		for (int i = 0; i < 16000; ++i)
		{
			data2c.push_back(data2a[i] + data2b[i]);
		}

		sound2c.SetDataLong(data2c);

		sound2c.WriteToFile("output/ex2c.wav");
		
	}
}
