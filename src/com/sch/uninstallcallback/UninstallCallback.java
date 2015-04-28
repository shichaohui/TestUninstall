package com.sch.uninstallcallback;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.content.Context;
import android.os.Build;
import android.os.Environment;

/** 监听此应用是否被卸载，若被卸载则弹出卸载反馈 */
public class UninstallCallback {

	private Context context = null;
	/** 监听进程pid */
	private int mObserverProcessPid = -1;
	private String appDir = "";
	private String appFilesDir = "";
	private String appObservedFile = "";
	private String appLockFile = "";
	/** 应用卸载后回调html文件所在的目录 */
	private String fileDir = "/meiriq/";
	/** 应用卸载后回调html文件的名字 */
	private final String fileName = "meiriq_uninstall_prompt.html";
	/** 应用卸载后回调html文件的协议 */
	private final String fileHeader = "file://";
	/** 应用卸载后回调html文件的路径 */
	private String filePath = "";
	/** 应用卸载后回调html文件的完整地址 */
	private String fileUrl = "";
	/** 本地文件不存在时的备用链接 */
	private final String serverUrl = "http://www.baidu.com";

	/**
	 * 初始化监听进程
	 * @param appDir 应用私有目录
	 * @param appFilesDir 应用文件目录
	 * @param appObservedFile 被监听的文件
	 * @param appLockFile 加锁文件
	 * @param userSerial 
	 * @param fileUrl 协议"file://" + 本地文件的路径
	 * @param url 本地文件不存在时的备用链接
	 * @return
	 */
	private native int init(String appDir, String appFilesDir, String appObservedFile, String appLockFile,
			String userSerial, String fileUrl, String filePath, String url);

	static {
		System.loadLibrary("appuninstallcallback");
	}

	public UninstallCallback(Context context) {
		this.context = context;
		
		// 向回调网页中添加内容
		new WriteFileToSdcard().writeHtmlToSdcard(context, fileName, fileDir, 
				"游戏名哈哈223", "https://www.baidu.com/img/bd_logo1.png");
	}

	/** 开始卸载监听 */
	public void startUninstallListener() {
		
		appDir = context.getFilesDir().getPath().split("/files")[0];
		appFilesDir = context.getFilesDir().getPath();
		appObservedFile = appFilesDir + "/observedFile";
		appLockFile = appFilesDir + "/lockFile";
		
		fileDir = Environment.getExternalStorageDirectory().getPath() + fileDir;
		filePath = fileDir + fileName;
		fileUrl = fileHeader + filePath;
		// API level小于17，不需要获取userSerialNumber
		if (Build.VERSION.SDK_INT < 17) {
			mObserverProcessPid = init(appDir, appFilesDir, appObservedFile, appLockFile, 
					null, fileUrl, filePath, serverUrl);
		// 否则，需要获取userSerialNumber
		} else {
			mObserverProcessPid = init(appDir, appFilesDir, appObservedFile, appLockFile, 
					getUserSerial(), fileUrl, filePath, serverUrl);
		}
	}
	
	/** 结束卸载监听 */
	public void stopUninstallListener() {
		if (mObserverProcessPid > 0) {
			android.os.Process.killProcess(mObserverProcessPid);
		}
	}
	
	// 由于targetSdkVersion低于17，只能通过反射获取
	private String getUserSerial() {
		Object userManager = context.getSystemService("user");
		if (userManager == null) {
			return null;
		}
		try {
			Method myUserHandleMethod = android.os.Process.class.getMethod(
					"myUserHandle", (Class<?>[]) null);
			Object myUserHandle = myUserHandleMethod.invoke(
					android.os.Process.class, (Object[]) null);

			Method getSerialNumberForUser = userManager.getClass().getMethod(
					"getSerialNumberForUser", myUserHandle.getClass());
			long userSerial = (Long) getSerialNumberForUser.invoke(userManager,
					myUserHandle);
			return String.valueOf(userSerial);
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}

		return null;
	}
		
}
