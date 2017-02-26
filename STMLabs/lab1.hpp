#pragma once
#include "utils.hpp"
#include <vector>
#include <algorithm>
#include <functional>
#include <array>
#include <unordered_map>
#include <iostream>
#include <iomanip>

namespace STMLab1
{

	static int DivideList(std::vector<std::pair<std::string, float>> inputList)
	{
		int beginWalker = 0, endWalker = inputList.size() - 1;
		float beginSum = 0, endSum = 0;

		while (beginWalker != endWalker)
			if (beginSum > endSum)
				endSum += inputList[--endWalker].second;
			else
				beginSum += inputList[++beginWalker].second;

		return beginWalker;
	}

	static Utils::ShannonFaroNode *SplitEvenly(std::vector<std::pair<std::string, float>> inputList)
	{
		if (inputList.size() == 1)
		{
			return new Utils::ShannonFaroNode(inputList[0].first);
		}
		else
		{
			int idx = DivideList(inputList);

			return new Utils::ShannonFaroNode(
				SplitEvenly(std::vector<std::pair<std::string, float>>(inputList.begin(), inputList.begin() + idx)),
				SplitEvenly(std::vector<std::pair<std::string, float>>(inputList.begin() + idx, inputList.end())));
		}
	}

	static Utils::ShannonFaroNode *ShannonFaro(std::vector<std::pair<std::string, float>> inputList)
	{
		auto sortedList = inputList;

		std::sort(sortedList.begin(), sortedList.end(), [](std::pair<std::string, float> a, std::pair<std::string, float> b) { return a.second < b.second; });

		return SplitEvenly(sortedList);
	}

	static void GetCodesFromTree(Utils::ShannonFaroNode *root, std::unordered_map<std::string, std::string> &result, std::string crtAcc)
	{
		if (!root->m_left && !root->m_right)
		{
			result[root->m_symbol] = crtAcc;
		}
		else
		{
			GetCodesFromTree(root->m_left, result, crtAcc + "0");
			GetCodesFromTree(root->m_right, result, crtAcc + "1");
		}
	}

	void Ex1()
	{

		std::vector<std::pair<std::string, float>> inputList;
		inputList.push_back(std::make_pair(std::string("Negru"), 100));
		inputList.push_back(std::make_pair(std::string("Alb"), 100));
		inputList.push_back(std::make_pair(std::string("Galben"), 20));
		inputList.push_back(std::make_pair(std::string("Portocaliu"), 5));
		inputList.push_back(std::make_pair(std::string("Rosu"), 5));
		inputList.push_back(std::make_pair(std::string("Purpuriu"), 3));
		inputList.push_back(std::make_pair(std::string("Albastru"), 20));
		inputList.push_back(std::make_pair(std::string("Verde"), 3));


		std::unordered_map<std::string, std::string> codes;

		GetCodesFromTree(ShannonFaro(inputList), codes, std::string(""));

		for (auto pair : codes)
			std::cout << pair.first << " --> " << pair.second << std::endl;
	}


	static std::unordered_map<std::string, std::unordered_map<std::string, float>> ex2Info;
	static const std::string ex2Terminator("Albastru");

	static float Encode(std::vector<std::string> inputList)
	{
		float low = 0, high = 1;

		for (auto symbol : inputList)
		{
			float range = high - low;
			high = low + range * ex2Info[symbol]["High"];
			low = low + range * ex2Info[symbol]["Low"];
		}

		return (high + low) / 2;
	}


	std::string findInInterval(float freq)
	{
		for (auto pair : ex2Info)
		{
			if (pair.second["Low"] <= freq && pair.second["High"] > freq)
				return pair.first;
		}
	}

	static std::vector<std::string> Decode(float code)
	{
		std::vector<std::string> result;

		std::string crtSymbol;
		while (crtSymbol != ex2Terminator)
		{
			crtSymbol = findInInterval(code);
			result.push_back(crtSymbol);

			code = (code - ex2Info[crtSymbol]["Low"]) / (ex2Info[crtSymbol]["High"] - ex2Info[crtSymbol]["Low"]);
		}

		return result;
	}

	void Ex2()
	{
		ex2Info["Negru"] = { { "Low", 0.00 }, { "High", 0.40 } };
		ex2Info["Alb"] = { { "Low", 0.40 }, { "High", 0.65 } };
		ex2Info["Galben"] = { { "Low", 0.65 }, { "High", 0.80 } };
		ex2Info["Rosu"] = { { "Low", 0.80 }, { "High", 0.90 } };
		ex2Info["Albastru"] = { { "Low", 0.90 }, { "High", 1.00 } };


		std::vector<std::string> sequence = { "Alb", "Negru", "Negru", "Galben", "Rosu", "Albastru" };

		float code = Encode(sequence);
		std::cout << "Code: " << code << std::endl << "Decoded: ";

		for (auto str : Decode(code))
		{
			std::cout << str << " ";
		}
		std::cout << std::endl;

	}

}