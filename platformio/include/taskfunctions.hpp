#include <lvgl.h>

void config_time(lv_task_t *task, bool status)
{
	if (status == 3)
	{
		for (int i = 0; i < 10; i++)
			dateTimeClient.update();
		configTime(Rtc, dateTimeClient);
	}
}