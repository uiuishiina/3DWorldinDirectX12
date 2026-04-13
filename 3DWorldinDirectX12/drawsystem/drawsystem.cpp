//drawsystem.c
#include"drawsystem.h"
#include"../debug/debugsystem.h"
#include"Device.h"
#include"CommandQueue.h"

//各種作成関数チェックマクロ
#define CHECK_CREATE_CLEAR(expr)\
	do{\
		if(!(expr)){\
			DEBUG_LOG_WARNING(#expr,"S_FALSE");\
			return false;\
		}\
		DEBUG_LOG(#expr,"S_OK");\
	} while (0)

class DrawSystem::Impl final
{
	CommandQueue Queue_{};
public:
	Impl() = default;
	~Impl() = default;

	[[nodiscard]] bool Initialize(HWND HWND, int width, int height) {
		CHECK_CREATE_CLEAR(Device::Instance().Create());

		CHECK_CREATE_CLEAR(Queue_.Create());

		return true;
	}


};

//--------------------------------------------------------------------------------------------------------
DrawSystem::DrawSystem() = default;
DrawSystem::~DrawSystem() = default;

[[nodiscard]] bool DrawSystem::Initialize(HWND HWND, int width, int height) {

	impl_ = std::make_unique<Impl>();

	if (!impl_->Initialize(HWND, width, height)) {
		DEBUG_LOG_ERROR("Impl::Initialize() Abnormal Termination")
		return false;
	}
	DEBUG_LOG_INFO("Impl::Initialize() Normal Termination");

	DEBUG_LOG_INFO("DrawSystem::Initialize() Normal Termination");
	return true;
}

//---------------------------------------------------------------------------------------------------------

