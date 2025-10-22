#include "platform.h"

int main(void) {
	platform_init();
	//事件系统初始化
	app_sched_init();
	user_event_init();
	module_init();
	uevt_bc_e(UEVT_APP_BOOT);
	while(1) {
		app_sched_execute();
	}
}



