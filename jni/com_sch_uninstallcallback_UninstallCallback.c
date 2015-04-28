/* 头文件begin */
#include "com_sch_uninstallcallback_UninstallCallback.h"
/* 头文件end */

#ifdef __cplusplus
extern "C"
{
#endif

/* 内全局变量begin */
static char TAG[] = "UninstallCallback.init";
static jboolean isCopy = JNI_TRUE;

static const char *app_dir = NULL;
static const char *app_files_dir = NULL;
static const char *app_observed_file = NULL;
static const char *app_lock_file = NULL;
/* 内全局变量 */

/*
 * Class:     com_sch_uninstallcallback_UninstallCallback
 * Method:    init
 * Signature: ()V
 * return: 子进程pid
 */
JNIEXPORT int JNICALL Java_com_sch_uninstallcallback_UninstallCallback_init(JNIEnv *env,
		jobject obj, jstring appDir, jstring appFilesDir, jstring appObservedFile, jstring appLockFile,
		jstring userSerial, jstring fileUrl, jstring filePath, jstring serverUrl) {

	app_dir = (*env)->GetStringUTFChars(env, appDir, &isCopy);
	app_files_dir = (*env)->GetStringUTFChars(env, appFilesDir, &isCopy);
	app_observed_file = (*env)->GetStringUTFChars(env, appObservedFile, &isCopy);
	app_lock_file = (*env)->GetStringUTFChars(env, appLockFile, &isCopy);

	jstring tag = (*env)->NewStringUTF(env, TAG);

	LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
			, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "init observer"), &isCopy));

	// fork子进程，以执行轮询任务
	pid_t pid = fork();
	if (pid < 0) {
		LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
				, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "fork failed !!!"), &isCopy));

		exit(1);
	} else if (pid == 0) {
		// 若监听文件所在文件夹不存在，创建
		FILE *p_filesDir = fopen(app_files_dir, "r");
		if (p_filesDir == NULL) {
			int filesDirRet = mkdir(app_files_dir, S_IRWXU | S_IRWXG | S_IXOTH);
			if (filesDirRet == -1) {
				LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
						, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "mkdir failed !!!"), &isCopy));

				exit(1);
			}
		}

		// 若被监听文件不存在，创建文件
		FILE *p_observedFile = fopen(app_observed_file, "r");
		if (p_observedFile == NULL) {
			p_observedFile = fopen(app_observed_file, "w");
		}
		fclose(p_observedFile);

		// 创建锁文件，通过检测加锁状态来保证只有一个卸载监听进程
		int lockFileDescriptor = open(app_lock_file, O_RDONLY);
		if (lockFileDescriptor == -1) {
			lockFileDescriptor = open(app_lock_file, O_CREAT);
		}
		int lockRet = flock(lockFileDescriptor, LOCK_EX | LOCK_NB);
		if (lockRet == -1) {
			LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
					, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "observed by another process"), &isCopy));

			exit(0);
		}
		LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
				, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "observed by child process"), &isCopy));

		// 分配空间，以便读取event
		void *p_buf = malloc(sizeof(struct inotify_event));
		if (p_buf == NULL) {
			LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
					, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "malloc failed !!!"), &isCopy));

			exit(1);
		}
		// 分配空间，以便打印mask
		int maskStrLength = 7 + 10 + 1;// mask=0x占7字节，32位整形数最大为10位，转换为字符串占10字节，'\0'占1字节
		char *p_maskStr = malloc(maskStrLength);
		if (p_maskStr == NULL) {
			free(p_buf);

			LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
					, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "malloc failed !!!"), &isCopy));

			exit(1);
		}

		// 开始监听
		LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
				, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "start observe"), &isCopy));

		// 初始化
		int fileDescriptor = inotify_init();
		if (fileDescriptor < 0) {
			free(p_buf);
			free(p_maskStr);

			LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
					, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "inotify_init failed !!!"), &isCopy));

			exit(1);
		}

		// 添加被监听文件到监听列表
		int watchDescriptor = inotify_add_watch(fileDescriptor, app_observed_file, IN_ALL_EVENTS);
		if (watchDescriptor < 0) {
			free(p_buf);
			free(p_maskStr);

			LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
					, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "inotify_add_watch failed !!!"), &isCopy));

			exit(1);
		}

		while(1) {
			// read会阻塞进程
			size_t readBytes = read(fileDescriptor, p_buf, sizeof(struct inotify_event));

			// 打印mask
			snprintf(p_maskStr, maskStrLength, "mask=0x%x\0", ((struct inotify_event *) p_buf)->mask);
			LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
					, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, p_maskStr), &isCopy));

			// 若文件被删除，可能是已卸载，还需进一步判断app文件夹是否存在
			if (IN_DELETE_SELF == ((struct inotify_event *) p_buf)->mask) {
				sleep(1);//休眠1秒
				FILE *p_appDir = fopen(app_dir, "r");
				// 确认已卸载
				if (p_appDir == NULL) {
					inotify_rm_watch(fileDescriptor, watchDescriptor);

					break;
				}
				// 未卸载，可能用户执行了"清除数据"
				else {
					fclose(p_appDir);
					LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
									, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "restart observe files"), &isCopy));
					// 重新创建被监听文件，并重新监听
					FILE *p_observedFile = fopen(app_observed_file, "w");
					fclose(p_observedFile);

					int watchDescriptor = inotify_add_watch(fileDescriptor, app_observed_file, IN_ALL_EVENTS);
					if (watchDescriptor < 0) {
						free(p_buf);
						free(p_maskStr);

						LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
								, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "inotify_add_watch failed !!!"), &isCopy));

						exit(1);
					}
				}
			}
		}

		// 释放资源
		free(p_buf);
		free(p_maskStr);

		// 停止监听
		LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
				, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "stop observe"), &isCopy));

		const char *url = NULL;
		if (fopen((*env)->GetStringUTFChars(env, filePath, &isCopy), "r") == NULL) {
		// 本地文件不存在使用备用链接
			url = (*env)->GetStringUTFChars(env, serverUrl, &isCopy);
		} else {
			// 本地文件存在使用本地链接
			url = (*env)->GetStringUTFChars(env, fileUrl, &isCopy);
		}

		if (userSerial == NULL) {
			// 执行命令am start -a android.intent.action.VIEW -d $(url)调用浏览器打开url
			execlp("am", "am", "start", "-a", "android.intent.action.VIEW", "-d",
					url, "-t", "text/html",
					(char *)NULL);
		} else {
			// 执行命令am start --user userSerial -a android.intent.action.VIEW -d $(url)调用浏览器打开url
			execlp("am", "am", "start", "--user", (*env)->GetStringUTFChars(env, userSerial, &isCopy),
					"-a", "android.intent.action.VIEW", "-d",
					url, "-t", "text/html",
					(char *)NULL);
		}

		// 执行命令失败log
		LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
				, (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "exec AM command failed !!!"), &isCopy));
	} else {
		// 父进程直接退出，使子进程被init进程领养，以避免子进程僵死，同时返回子进程pid
		return pid;
	}
}

#ifdef __cplusplus
}
#endif
