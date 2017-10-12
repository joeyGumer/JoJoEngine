#include "Bar.h"

Bar::Bar()
{

}

Bar::~Bar()
{
	data.clear();
}

void Bar::StartBar(const int& check)
{
	timer_check = check;
	timer.Start();
}

void Bar::FillBar(const float & new_data)
{
	if (timer.Read() > timer_check)
	{
		timer.Start();

		if (data.size() > 100)
		{
			for (int i = 1; i < data.size(); i++)
				data[i - 1] = data[i];

			data[data.size() - 1] = new_data;
		}

		else
			data.push_back(new_data);
	}
}

const std::vector<float> Bar::GetData() const
{
	return data;
}
