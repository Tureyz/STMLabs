#pragma once
#include "utils.hpp"

namespace STMLab5
{
	void Ex124()
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


		sound1.WriteToFile(outputPath + "sin1.wav");
		sound2.WriteToFile(outputPath + "sin2.wav");
		sound3.WriteToFile(outputPath + "sin3.wav");



		std::vector<float> mag1 = Utils::WaveStr::DFTMag(sound1);
		std::vector<float> mag2 = Utils::WaveStr::DFTMag(sound2);
		std::vector<float> mag3 = Utils::WaveStr::DFTMag(sound3);


		Utils::MagToCSV(mag1, outputPath + "mag1.csv");
		Utils::MagToCSV(mag2, outputPath + "mag2.csv");
		Utils::MagToCSV(mag3, outputPath + "mag3.csv");


		Utils::WaveStr sound4(1, 15, 8000);
		sound4.SetDataLong(Utils::WaveStr::IFT(Utils::WaveStr::DFT(sound3)));
		sound4.WriteToFile(outputPath + "sin4.wav");
	}
}