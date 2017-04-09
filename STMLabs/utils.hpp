#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

namespace Utils
{
	const float e = 2.7182818;
	const float PI = 3.14159;

	std::vector<short> GenerateSine(float sampleRate, float freq, float maxAmpl, float resultSize)
	{
		std::vector<short> result;

		for (int i = 0; i < resultSize; ++i)
		{
			result.push_back((sin(i * 2 * PI / sampleRate * freq) + 1) * maxAmpl);
		}

		return result;
	}

	class ShannonFaroNode
	{
	public:
		ShannonFaroNode *m_left, *m_right;
		std::string m_symbol;

		ShannonFaroNode()
		{
			m_left = m_right = NULL;
			m_symbol = "";
		}

		ShannonFaroNode(ShannonFaroNode *left, ShannonFaroNode *right) : m_left(left), m_right(right) { m_symbol = ""; }
		ShannonFaroNode(std::string value) : m_symbol(value) { m_left = m_right = NULL; }
	};

	template <typename T> int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	std::vector<unsigned char> LoadWavFileRaw(std::string path)
	{
		std::ifstream input(path, std::ios::binary);

		return std::vector<unsigned char>((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
	}

	std::string bytesToHumanReadable(size_t bytes)
	{
		std::vector<std::string> units = { "B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };

		int i = 0;

		double asd = bytes;
		while (asd > 1024)
		{
			asd /= 1024;
			i++;
		}

		return std::to_string(asd) + " " + units[i];
	}


	size_t getFieldFromAudioFile(std::vector<unsigned char> &file, int start, int offset, bool littleEndian = true)
	{
		size_t result = 0;
		for (int i = start; i < start + offset; ++i)
		{
			result += file[!littleEndian ? start + offset - i - 1 : i] << (8 * (i - start));
		}

		return result;
	}


	class WaveStr
	{
	public:
		std::string m_chunkID;
		uint32_t m_chunkSize;
		std::string m_format;
		std::string m_subChunk1ID;
		uint32_t m_subChunk1Size;
		uint16_t m_audioFormat;
		uint32_t m_byteRate;
		uint16_t m_blockAlign;
		std::string m_subChunk2ID;
		uint32_t m_subChunk2Size;
		std::vector<unsigned char> m_data;
		std::vector<short> m_dataLong;
		std::vector<float> m_dbfsLong;

		uint16_t m_numChannels;
		uint32_t m_sampleRate;
		uint16_t m_bitsPerSample;


		static WaveStr LoadFromFile(std::string path)
		{
			WaveStr result;
			std::vector<unsigned char> soundFileRaw = Utils::LoadWavFileRaw(path);


			result.m_chunkSize = Utils::getFieldFromAudioFile(soundFileRaw, 4, 4);
			result.m_numChannels = Utils::getFieldFromAudioFile(soundFileRaw, 22, 2);
			result.m_sampleRate = Utils::getFieldFromAudioFile(soundFileRaw, 24, 4);
			result.m_byteRate = Utils::getFieldFromAudioFile(soundFileRaw, 28, 4);
			result.m_blockAlign = Utils::getFieldFromAudioFile(soundFileRaw, 32, 2);
			result.m_bitsPerSample = Utils::getFieldFromAudioFile(soundFileRaw, 34, 2);
			result.m_subChunk2Size = Utils::getFieldFromAudioFile(soundFileRaw, 40, 4);



			if (result.m_bitsPerSample == 8)
			{
				result.m_data = std::vector<unsigned char>(soundFileRaw.begin() + 44, soundFileRaw.end());
			}
			else
			{
				const short *data = reinterpret_cast<const short*>(soundFileRaw.data() + 44);
				result.m_dataLong = std::vector<short>(data, data + result.m_subChunk2Size / sizeof(short));
				result.m_dbfsLong.resize(result.m_dataLong.size());
			}


			return result;
		}

		WaveStr(uint16_t numChannels, uint16_t bitsPerSample, uint32_t sampleRate)
		{
			m_chunkID = "RIFF";
			m_format = "WAVE";
			m_subChunk1ID = "fmt ";
			m_subChunk2ID = "data";
			m_subChunk1Size = 16;
			m_audioFormat = 1;

			m_numChannels = numChannels;
			m_bitsPerSample = bitsPerSample;
			m_sampleRate = sampleRate;

			m_blockAlign = m_numChannels * m_bitsPerSample / 8;
			m_byteRate = m_sampleRate * m_blockAlign;
		}

		void SetNumChannels(std::size_t val)
		{
			m_numChannels = val;

			m_blockAlign = m_numChannels * m_bitsPerSample / 8;
			m_byteRate = m_sampleRate * m_blockAlign;
		}

		void SetData(std::vector<unsigned char> val)
		{
			m_data = val;
			m_subChunk2Size = m_data.size();
			m_chunkSize = 36 + m_subChunk2Size;
		}

		void SetDataLong(std::vector<short> val)
		{
			m_dataLong = val;
			m_subChunk2Size = m_dataLong.size() * sizeof(short);
			m_chunkSize = 36 + m_subChunk2Size;

			m_dbfsLong.resize(m_dataLong.size());
		}

		void SetSampleRate(std::size_t val)
		{
			m_sampleRate = val;

			m_byteRate = m_sampleRate * m_blockAlign;
		}

		void SetBitsPerSample(std::size_t val)
		{
			m_bitsPerSample = val;

			m_blockAlign = m_numChannels * m_bitsPerSample / 8;
			m_byteRate = m_sampleRate * m_blockAlign;
		}

		WaveStr()
		{
			m_chunkID = "RIFF";
			m_format = "WAVE";
			m_subChunk1ID = "fmt ";
			m_subChunk2ID = "data";
			m_subChunk1Size = 16;
			m_audioFormat = 1;
		}


		void WriteToFile(std::string outputPath)
		{
			std::ofstream outputFile(outputPath, std::ios::binary);

			if (outputFile.is_open())
			{

				outputFile.write(m_chunkID.c_str(), m_chunkID.size());
				outputFile.write((char *)&m_chunkSize, sizeof(m_chunkSize));
				outputFile.write(m_format.c_str(), m_format.size());
				outputFile.write(m_subChunk1ID.c_str(), m_subChunk1ID.size());
				outputFile.write((char *)&m_subChunk1Size, sizeof(m_subChunk1Size));
				outputFile.write((char *)&m_audioFormat, sizeof(m_audioFormat));
				outputFile.write((char *)&m_numChannels, sizeof(m_numChannels));
				outputFile.write((char *)&m_sampleRate, sizeof(m_sampleRate));
				outputFile.write((char *)&m_byteRate, sizeof(m_byteRate));
				outputFile.write((char *)&m_blockAlign, sizeof(m_blockAlign));
				outputFile.write((char *)&m_bitsPerSample, sizeof(m_bitsPerSample));
				outputFile.write(m_subChunk2ID.c_str(), m_subChunk2ID.size());
				outputFile.write((char *)&m_subChunk2Size, sizeof(m_subChunk2Size));

				if (m_bitsPerSample == 8)
				{
					outputFile.write((char *)m_data.data(), m_data.size());
				}
				else
				{
					outputFile.write((char *)m_dataLong.data(), m_dataLong.size() * sizeof(short));
				}

				outputFile.close();
			}
		}

		void EncodeMu()
		{
			SetBitsPerSample(8);

			for (short el : m_dataLong)
			{
				m_data.push_back(128 + static_cast<unsigned char>(floor(128 * M(static_cast<float>(el) / 32768))));
			}

			m_dataLong.clear();
			m_subChunk2Size = m_data.size();
			m_chunkSize = 36 + m_subChunk2Size;
		}

		void DecodeMu()
		{
			SetBitsPerSample(16);

			for (unsigned char el : m_data)
			{

				m_dataLong.push_back(static_cast<short>(ceil(D(static_cast<float>(el - 128) / 128) * 32768)));
			}

			m_data.clear();
			m_subChunk2Size = m_dataLong.size() * sizeof(short);
			m_chunkSize = 36 + m_subChunk2Size;
		}

		std::vector<uint16_t> ComputeHistogram()
		{
			if (m_bitsPerSample == 8)
			{
				std::vector<uint16_t> result(256, 0);

				for (auto el : m_data)
				{
					result[el]++;
				}
				return result;
			}
			else
			{
				std::vector<uint16_t> result(32768, 0);
				for (auto el : m_dataLong)
				{
					result[el]++;
				}
				return result;
			}
		}


		void DBFSScale()
		{
			for (int i = 0; i < m_dataLong.size(); ++i)
			{
				m_dbfsLong[i] = ToDBFS(m_dataLong[i]);
			}
		}

		void Compress(float threshold, float compressFactor, float which)
		{
			short pcmThreshold = FromDBFS(threshold);

			for (int i = 0; i < m_dataLong.size(); ++i)
			{
				if (m_dbfsLong[i] > threshold && !which)
				{
					m_dataLong[i] = sgn(m_dataLong[i]) * abs(pcmThreshold) + sgn(m_dataLong[i]) * abs((m_dataLong[i] - pcmThreshold) / compressFactor);
				}
				else if (m_dbfsLong[i] < threshold && which)
				{
					m_dataLong[i] = sgn(m_dataLong[i]) * abs(pcmThreshold) + sgn(m_dataLong[i]) * abs((m_dataLong[i] - pcmThreshold) * compressFactor);
				}
			}
		}


		void DBFSNormalize()
		{

			float maxAmpl = -9999999999999999999999.f;


			for (auto el : m_dataLong)
			{
				if (el > maxAmpl)
				{
					maxAmpl = el;
				}
			}

			float amplifyFactor = abs(pow(2, 16 - 1) / maxAmpl);

			for (int i = 0; i < m_dataLong.size(); ++i)
			{
				m_dataLong[i] *= amplifyFactor;
			}
		}

	private:

		float M(float x)
		{
			return sgn(x) * (log(1 + 255 * abs(x)) / 5.5452);
		}

		float D(float x)
		{
			return sgn(x) * ((pow(256, abs(x)) - 1) / 255);
		}

		float ToDBFS(short x)
		{
			return 20 * log10(abs(x) / pow(2, 16 - 1));
		}


		short FromDBFS(float x)
		{
			return pow(2, 16 - 1) * pow(e, (1.f / 20) * x * (log(2) + log(5)));
		}


	};


	std::vector<std::pair<float, float>> DFT(Utils::WaveStr &sound)
	{
		std::vector<std::pair<float, float>> result;

		for (int i = 0; i < sound.m_dataLong.size(); ++i)
		{
			std::pair<float, float> xk;

			for (int j = 0; j < sound.m_dataLong.size(); ++j)
			{
				float exp = 2 * PI * i * j / sound.m_dataLong.size();

				xk.first += sound.m_dataLong[j] * cos(exp);
				xk.second -= sound.m_dataLong[j] * sin(exp);
			}

			result.push_back(xk);
		}

		return result;
	}

	std::vector<short> IFT(std::vector<std::pair<float, float>> values)
	{

		std::vector<short> result;

		for (int i = 0; i < values.size(); ++i)
		{
			std::pair<float, float> partial;

			for (int j = 0; j < values.size(); ++j)
			{
				float exp = 2 * PI * i * j / values.size();
				partial.first += values[j].first * cos(exp);
				partial.second += values[j].second * sin(exp);
			}

			result.push_back((partial.first - partial.second) / values.size());
		}

		return result;
	}

	std::vector<float> DFTMag(Utils::WaveStr &sound)
	{
		std::vector<float> result;

		for (auto xk : DFT(sound))
		{
			result.push_back(sqrt(xk.first * xk.first + xk.second * xk.second));
		}

		return result;
	}

	void MagToCSV(std::vector<float> mag, std::string outputPath)
	{
		std::ofstream outputFile(outputPath);

		if (outputFile.is_open())
		{
			outputFile << "Index,Magnitude" << std::endl;

			for (int i = 0; i < mag.size(); ++i)
			{
				outputFile << i << "," << mag[i] << std::endl;
			}

			outputFile.close();
		}
	}
}