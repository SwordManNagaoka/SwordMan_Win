#pragma once
#include <string>

class RankSelector
{
public:
	[[nodiscard]] const std::string  execute(const int scoreData) const noexcept
	{
		std::string rankName = "";
		if (scoreData >= 0 && scoreData <= 2000)
		{
			rankName = "D";
		}
		else if (scoreData >= 2000 && scoreData <= 4000)
		{
			rankName = "C";
		}
		else if (scoreData >= 4000 && scoreData <= 6000)
		{
			rankName = "B";
		}
		else if (scoreData >= 6000 && scoreData <= 8000)
		{
			rankName = "A";
		}
		else if (scoreData >= 8000)
		{
			rankName = "S";
		}
		return rankName;
	}
};