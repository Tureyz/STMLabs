#pragma once
#include <string>

namespace Utils
{

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

}