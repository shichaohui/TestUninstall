/* 头文件begin */
#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <sys/stat.h>

#include <android/log.h>
/* 头文件end */

/* 宏定义begin */
//清0宏
#define MEM_ZERO(pDest, destSize) memset(pDest, 0, destSize)

//LOG宏定义
#define LOG_INFO(tag, msg) __android_log_write(ANDROID_LOG_INFO, tag, msg)
#define LOG_DEBUG(tag, msg) __android_log_write(ANDROID_LOG_DEBUG, tag, msg)
#define LOG_WARN(tag, msg) __android_log_write(ANDROID_LOG_WARN, tag, msg)
#define LOG_ERROR(tag, msg) __android_log_write(ANDROID_LOG_ERROR, tag, msg)
/* 宏定义end */

#ifndef _Included_com_sch_uninstallcallback_UninstallCallback
#define _Included_com_sch_uninstallcallback_UninstallCallback
#ifdef __cplusplus
extern "C" {
#endif

#undef com_sch_uninstallcallback_UninstallCallback_MODE_PRIVATE
#define com_sch_uninstallcallback_UninstallCallback_MODE_PRIVATE 0L
#undef com_sch_uninstallcallback_UninstallCallback_MODE_WORLD_READABLE
#define com_sch_uninstallcallback_UninstallCallback_MODE_WORLD_READABLE 1L
#undef com_sch_uninstallcallback_UninstallCallback_MODE_WORLD_WRITEABLE
#define com_sch_uninstallcallback_UninstallCallback_MODE_WORLD_WRITEABLE 2L
#undef com_sch_uninstallcallback_UninstallCallback_MODE_APPEND
#define com_sch_uninstallcallback_UninstallCallback_MODE_APPEND 32768L
#undef com_sch_uninstallcallback_UninstallCallback_MODE_MULTI_PROCESS
#define com_sch_uninstallcallback_UninstallCallback_MODE_MULTI_PROCESS 4L
#undef com_sch_uninstallcallback_UninstallCallback_BIND_AUTO_CREATE
#define com_sch_uninstallcallback_UninstallCallback_BIND_AUTO_CREATE 1L
#undef com_sch_uninstallcallback_UninstallCallback_BIND_DEBUG_UNBIND
#define com_sch_uninstallcallback_UninstallCallback_BIND_DEBUG_UNBIND 2L
#undef com_sch_uninstallcallback_UninstallCallback_BIND_NOT_FOREGROUND
#define com_sch_uninstallcallback_UninstallCallback_BIND_NOT_FOREGROUND 4L
#undef com_sch_uninstallcallback_UninstallCallback_BIND_ABOVE_CLIENT
#define com_sch_uninstallcallback_UninstallCallback_BIND_ABOVE_CLIENT 8L
#undef com_sch_uninstallcallback_UninstallCallback_BIND_ALLOW_OOM_MANAGEMENT
#define com_sch_uninstallcallback_UninstallCallback_BIND_ALLOW_OOM_MANAGEMENT 16L
#undef com_sch_uninstallcallback_UninstallCallback_BIND_WAIVE_PRIORITY
#define com_sch_uninstallcallback_UninstallCallback_BIND_WAIVE_PRIORITY 32L
#undef com_sch_uninstallcallback_UninstallCallback_BIND_IMPORTANT
#define com_sch_uninstallcallback_UninstallCallback_BIND_IMPORTANT 64L
#undef com_sch_uninstallcallback_UninstallCallback_BIND_ADJUST_WITH_ACTIVITY
#define com_sch_uninstallcallback_UninstallCallback_BIND_ADJUST_WITH_ACTIVITY 128L
#undef com_sch_uninstallcallback_UninstallCallback_CONTEXT_INCLUDE_CODE
#define com_sch_uninstallcallback_UninstallCallback_CONTEXT_INCLUDE_CODE 1L
#undef com_sch_uninstallcallback_UninstallCallback_CONTEXT_IGNORE_SECURITY
#define com_sch_uninstallcallback_UninstallCallback_CONTEXT_IGNORE_SECURITY 2L
#undef com_sch_uninstallcallback_UninstallCallback_CONTEXT_RESTRICTED
#define com_sch_uninstallcallback_UninstallCallback_CONTEXT_RESTRICTED 4L
#undef com_sch_uninstallcallback_UninstallCallback_RESULT_CANCELED
#define com_sch_uninstallcallback_UninstallCallback_RESULT_CANCELED 0L
#undef com_sch_uninstallcallback_UninstallCallback_RESULT_OK
#define com_sch_uninstallcallback_UninstallCallback_RESULT_OK -1L
#undef com_sch_uninstallcallback_UninstallCallback_RESULT_FIRST_USER
#define com_sch_uninstallcallback_UninstallCallback_RESULT_FIRST_USER 1L
#undef com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_DISABLE
#define com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_DISABLE 0L
#undef com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_DIALER
#define com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_DIALER 1L
#undef com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_SHORTCUT
#define com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_SHORTCUT 2L
#undef com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_SEARCH_LOCAL
#define com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_SEARCH_LOCAL 3L
#undef com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_SEARCH_GLOBAL
#define com_sch_uninstallcallback_UninstallCallback_DEFAULT_KEYS_SEARCH_GLOBAL 4L

/*
 * Class:     com_sch_uninstallcallback_UninstallCallback
 * Method:    init
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT int JNICALL Java_com_sch_uninstallcallback_UninstallCallback_init(
		JNIEnv *, jobject, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring);

#ifdef __cplusplus
}
#endif
#endif