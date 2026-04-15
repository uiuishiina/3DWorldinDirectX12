#pragma once
#include"logsystem.h"
#include"assertsystem.h"

#if _DEBUG
//変数チェックマクロ
#define DEBUG_CHECK_VALUE(X)\
	 do { \
        LOG_LINE_BREAK(1); \
        CHANGE_VALUE(X); \
        LOG_LINE_BREAK(1); \
    } while(0)
//デバッグログ出力マクロ
#define DEBUG_LOG(...)\
	LOG(__VA_ARGS__);
//デバッグ情報出力マクロ
#define DEBUG_LOG_INFO(...)\
	LOG_INFO(__VA_ARGS__);
//デバッグ注意ログマクロ
#define DEBUG_LOG_WARNING(...)\
	LOG_WARNING(__VA_ARGS__);
//デバッグエラーログマクロ
#define DEBUG_LOG_ERROR(...)\
	LOG_ERROR(__VA_ARGS__);
#define DEBUG_LINELOG(expr)\
	do{\
		std::string str;\
		for(int i = 0; i < expr; i++){\
			str += "=";\
		}\
		LOG_UNKWOUN(str);\
	}while(0)
	

//ASSERTマクロ
#define DEBUG_ASSERT(expr) ASSERT(expr)
#define DEBUG_HR_ASSERT(expr) HRASSERT(expr)
#else
#define DEBUG_CHECK_VALUE(X) (void(0))
#define DEBUG_LOG(...) (void(0))
#define DEBUG_LOG_INFO(...) (void(0))
#define DEBUG_LOG_WARNING(...) (void(0))
#define DEBUG_LOG_ERROR(...) (void(0))
#define DEBUG_ASSERT(expr) (void(0))
#define DEBUG_HR_ASSERT(expr) (void(0))
#endif