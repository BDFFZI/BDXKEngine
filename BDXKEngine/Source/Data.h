#pragma once
#include<functional>

#include<vector>
#include"Color.h"

struct Data
{
	template<typename TValue>
	void Foreach(std::vector<TValue> vertor, std::function<void(TValue)> handler)
	{
		for (TValue value: vertor)
		{
			handler(value);
		}
	}
};

