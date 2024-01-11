#pragma once


namespace BINDU
{
	template<class T>
	struct Range
	{
		T min;
		T max;

		Range() :min{ 0 }, max{ 0 }
		{}

		Range(T min, T max) :min{ min }, max{ max }
		{}

	};

	using iRange = Range<int>;
	using fRange = Range<float>;

};
