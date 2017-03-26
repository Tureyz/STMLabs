#pragma once
#include "utils.hpp"

namespace STMLab4
{
	void Ex123()
	{
		std::string inputPath = "input/bike.wav";
		std::string ex2OutputPath = "output/bikeEx2.wav";
		std::string ex3OutputPath = "output/bikeEx3.wav";
		Utils::WaveStr soundFile = Utils::WaveStr::LoadFromFile(inputPath);

		soundFile.DBFSScale();

		soundFile.Compress(-40, 2, false);
		soundFile.Compress(-60, 2, true);
		soundFile.WriteToFile(ex2OutputPath);
		soundFile.DBFSNormalize();
		soundFile.WriteToFile(ex3OutputPath);

	}
}