#pragma once

#include "utils.hpp"

namespace STMLab2
{
	void Ex1()
	{
		std::vector<unsigned char> soundFile = Utils::LoadWavFile("input/sound1.wav");

		std::cout << "input/sound1.wav" << std::endl;
		std::cout << "File size: " << Utils::bytesToHumanReadable(Utils::getFieldFromAudioFile(soundFile, 4, 4)) << std::endl;
		std::cout << "Format: ";

		for (int i = 8; i < 8 + 4; ++i)
		{
			std::cout << soundFile[i];
		}

		std::cout << std::endl << "Audio format: " << Utils::getFieldFromAudioFile(soundFile, 20, 2) << std::endl;
		std::cout << "Number of channels: " << Utils::getFieldFromAudioFile(soundFile, 22, 2) << std::endl;
		std::cout << "Sample rate: " << Utils::getFieldFromAudioFile(soundFile, 24, 4) << std::endl;
		std::cout << "Bits per sample: " << Utils::getFieldFromAudioFile(soundFile, 34, 2) << std::endl;
		std::cout << "Data: " << std::endl;

		for (int i = 44; i < 44 + 100; ++i)
		{
			std::cout << (int)soundFile[i] << " ";
		}

		std::cout << std::endl << "Interrupted after 100 entries." << std::endl;


	}

	void Ex2()
	{


		Utils::WaveFile sound2a;
		sound2a.SetNumChannels(1);
		sound2a.SetBitsPerSample(16);
		sound2a.SetSampleRate(8000);

		std::vector<short> data2a;

		int sineFreq = 400;
		int sineMaxAmpl = 15000;
		for (int i = 0; i < 16000; ++i)
		{
			data2a.push_back(std::sin((sineFreq * 2 * 3.14 / 16000) * (i + 1)) * sineMaxAmpl);
		}

		sound2a.SetDataLong(data2a);

		sound2a.WriteToFile("output/ex2a.wav");


		Utils::WaveFile sound2b;
		sound2b.SetNumChannels(1);
		sound2b.SetBitsPerSample(16);
		sound2b.SetSampleRate(8000);

		std::vector<short> data2b;
		sineFreq = 1000;
		sineMaxAmpl = 10000;
		for (int i = 0; i < 16000; ++i)
		{
			data2b.push_back(std::sin((sineFreq * 2 * 3.14 / 16000) * (i + 1)) * sineMaxAmpl);
		}

		sound2b.SetDataLong(data2b);

		sound2b.WriteToFile("output/ex2b.wav");


		Utils::WaveFile sound2c;
		sound2c.SetNumChannels(1);
		sound2c.SetBitsPerSample(16);
		sound2c.SetSampleRate(8000);

		std::vector<short> data2c;
		for (int i = 0; i < 16000; ++i)
		{
			data2c.push_back(data2a[i] + data2b[i]);
		}

		sound2c.SetDataLong(data2c);

		sound2c.WriteToFile("output/ex2c.wav");
		
	}
}
