#pragma once
#include"logsystem.h"
#include"assertsystem.h"

#if _DEBUG
//変数チェックマクロ
#define DEBUG_CHECK_VALUE(X)\
	 do { \
        LOG_LINE_BREAK(1); \
        CALLER_LOG("[ Info ]", CHANGE_VALUE(X)); \
        LOG_LINE_BREAK(1); \
    } while(0)
//デバッグログ出力マクロ
#define DEBUG_LOG(...)\
	LOG("[ Log ]",__VA_ARGS__);
//ASSERTマクロ
#define DEBUG_ASSERT(expr) ASSERT(expr)
#define DEBUG_HR_ASSERT(expr) HRASSERT(expr)
#else
#define DEBUG_CHECK_VALUE(X) (void(0))
#define DEBUG_LOG(...) (void(0))
#define DEBUG_ASSERT(expr) (void(0))
#define DEBUG_HR_ASSERT(expr) (void(0))
#endif
