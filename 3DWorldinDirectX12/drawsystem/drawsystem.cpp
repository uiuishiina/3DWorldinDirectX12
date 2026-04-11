#include"drawsystem.h"
#include"../debug/debugsystem.h"
#include"Device.h"

#define CHECK_CLEAR(expr)\
	do{\
		if(!(expr)){\
			return false;\
		}\
		DEBUG_LOG(#expr,"S_OK");\
	} while (0)

[[nodiscard]] bool DrawSystem::Initialize(HWND HWND, int width, int height) {
	CHECK_CLEAR(Device::Instance().Create());


}