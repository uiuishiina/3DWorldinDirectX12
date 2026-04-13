//drawsystem.c
#include"drawsystem.h"
#include"../debug/debugsystem.h"
#include"Device.h"
#include"CommandQueue.h"
#include"CommandAllocator.h"
#include"CommandList.h"

//各種作成関数チェックマクロ
#define CHECK_CREATE_CLEAR(expr)\
	do{\
		if(!(expr)){\
			DEBUG_LOG_WARNING(#expr,"S_FALSE");\
			return false;\
		}\
		DEBUG_LOG(#expr,"S_OK");\
	} while (0)

//-------------------------------------------------------------------------------------------------------

class DrawSystem::Impl final
{
	CommandQueue		Queue_{};
	CommandAllocator	Allocator_{};
	CommandList			List_{};

	const int BufferSize_ = 2;
public:
	Impl() = default;
	~Impl() = default;

	[[nodiscard]] bool Initialize(HWND HWND, int width, int height) {

		
		CHECK_CREATE_CLEAR(Device::Instance().Create());
		CHECK_CREATE_CLEAR(Queue_.Create());
		CHECK_CREATE_CLEAR(Allocator_.Create(BufferSize_));
		CHECK_CREATE_CLEAR(List_.Create(Allocator_.GetAllocator(0)));
		DEBUG_LINELOG(100);
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
	DEBUG_LINELOG(100);
	return true;
}

//---------------------------------------------------------------------------------------------------------

