#pragma once

#include "utils.hpp"

namespace STMLab6
{
	void Ex1()
	{
		std::string outputPath = "output/";
		Utils::WaveStr sound1(1, 16, 8000);
		sound1.SetDataLong(Utils::GenerateSine(8000, 400, 15000, 1024));
		Utils::WaveStr sound2(1, 16, 8000);
		sound2.SetDataLong(Utils::GenerateSine(8000, 1000, 10000, 1024));

		Utils::WaveStr sound3(1, 16, 8000);

		std::vector<short> s3Data;

		for (int i = 0; i < sound1.m_dataLong.size(); ++i)
		{
			s3Data.push_back(sound1.m_dataLong[i] + sound2.m_dataLong[i]);
		}

		sound3.SetDataLong(s3Data);
		sound3.WriteToFile(outputPath + "sin3.wav");


		sound3.DFTFilter(400, 0, 0);
		sound3.WriteToFile(outputPath + "sin3LOW.wav");

		sound3.SetDataLong(s3Data);
		sound3.DFTFilter(0, 400, 1);
		sound3.WriteToFile(outputPath + "sin3HIGH.wav");

		sound3.SetDataLong(s3Data);
		sound3.DFTFilter(0, 400, 2);
		sound3.WriteToFile(outputPath + "sin3BP.wav");

		sound3.SetDataLong(s3Data);
		sound3.DFTFilter(0, 400, 3);
		sound3.WriteToFile(outputPath + "sin3BR.wav");

		sound3.SetDataLong(s3Data);
		sound3.ApplyFIRLowPass(1000, 15);
		sound3.WriteToFile(outputPath + "sin3FIRLOW.wav");
	}
}