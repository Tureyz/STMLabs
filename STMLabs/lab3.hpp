#include "utils.hpp"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>



namespace STMLab3
{

	void Ex1n2()
	{
		std::string inputPath = "input/beep.wav";
		std::string outputPathEncode = "output/beep1.wav";
		std::string outputPathDecode = "output/beep2.wav";
		Utils::WaveStr soundFile = Utils::WaveStr::LoadFromFile(inputPath);
		soundFile.EncodeMu();
		soundFile.WriteToFile(outputPathEncode);
		soundFile.DecodeMu();
		soundFile.WriteToFile(outputPathDecode);
	}

	void Ex3()
	{
		std::string inputPath = "output/beep1.wav";
		std::string outputPath = "output/beep1Hist.txt";
		Utils::WaveStr soundFile = Utils::WaveStr::LoadFromFile(inputPath);

		std::ofstream histFile(outputPath);

		if (histFile.is_open())
		{
			auto hist = soundFile.ComputeHistogram();

			int hist_w = 800;
			int hist_h = 600;
			int bin_w = cvRound((double)hist_w / 256);

			cv::Mat histMat(hist.size(), 1, CV_16UC1);
			memcpy(histMat.data, hist.data(), hist.size() * sizeof(uint16_t));

			cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(0, 0, 0));

			cv::Mat normalizedHist;
			/// Normalize the result to [ 0, histImage.rows ]
			normalize(histMat, normalizedHist, 0, histImage.rows, cv::NORM_MINMAX);

			for (int i = 0; i < hist.size(); ++i)
			{
				auto histVal = hist[i];

				histFile << histVal << std::endl;
				cv::line(histImage,	cv::Point(bin_w * i, hist_h), cv::Point(bin_w * i, hist_h - cvRound(static_cast<double>(normalizedHist.at<uint16_t>(i)))), cv::Scalar(255, 255, 255), 2, 8, 0);
			}

			histFile.close();			
			cv::imwrite("output/beep1Hist.png", histImage);
		}
	}
}