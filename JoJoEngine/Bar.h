#ifndef __BAR_H__
#define __BAR_H__

#include "Timer.h"
#include <vector>

class Bar
{
	public:
		Bar();
		~Bar();

		void StartBar(const int& check);
		void FillBar(const float& new_data);
		const std::vector<float> GetData() const;

	private:
		Timer timer;
		int timer_check;
		std::vector<float> data;
	};


#endif
