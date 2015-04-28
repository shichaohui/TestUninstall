package com.sch.uninstallcallback;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.RandomAccessFile;

import android.content.Context;
import android.os.Environment;

public class WriteFileToSdcard {

	/** 要操作的文件名字 */
	private String fileName = "";
	/** 保存文件路径 */
	private String fileDir = "";
	/** 拼接后的插入内容 */
	private String content = "";
	/** 网页上文字的尺寸 */
	private final int fontSize = 6; // 尺寸无效
	/** 网页上图标的尺寸 */
	private final int gameIconWH = 150;
	/** html文件的开始文本 */
	private final String fileBeginStr= "<html>\n<body>\n";
	/** html文件的结束文本 */
	private final String fileEndStr = "</body>\n</html>";
	/** 下载链接 */
	private final String link = "http://games.meiriq.com//download";
	/** html文件中的下载链接 */
	private final String fileDownlooadLink = "<br /><br /><font size='" + fontSize + "'>海量精品游戏等你发现</font>" +
			"<a href='"+ link + "'>" +
			"<font size='" + fontSize + 1 + "'>点我下载</font>" + "</a><br />\n";

	public WriteFileToSdcard() {
	}
	
	/**
	 * 写html文件到sdcard
	 * @param context
	 * @param fileName 要操作的文件名字
	 * @param fileDir 保存文件路径
	 * @param shortcutName 桌面图标的名字
	 * @param shortcutIcon 桌面图标的图片的链接
	 */
	public void writeHtmlToSdcard(Context context, String fileName,
			String fileDir, String shortcutName, String shortcutIcon) {
		this.fileName = fileName;
		this.fileDir = fileDir;
		// sdcard不可用
		if(!Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
	        return;
	    }
		// 拼接html字符串
		content = "<img src='" + shortcutIcon + "' height='" + gameIconWH + "' width='" + gameIconWH + "'/>" +
				"<font size='" + fontSize +"'>" + shortcutName + "</font>" + "\n<br />\n";
		// 文件已存在
		if (new File(fileDir).exists() && new File(fileDir + fileName).exists()) {
			notFirstWrite();
		} else {
			firstWrite();
		}
	}

	/** 第一次创建文件写文件 */
	private void firstWrite() {
		OutputStream outputStream = null;
		try {
			outputStream = new FileOutputStream(new File(fileDir + fileName)); // 文件输出流
			outputStream.write(fileBeginStr.getBytes());  // 向文件中写数据
			outputStream.write(content.getBytes());
			outputStream.write(fileDownlooadLink.getBytes());
			outputStream.write(fileEndStr.getBytes());
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (outputStream != null) {
				try {
					outputStream.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	/** 非第一次向文件中插入内容 */
	private void notFirstWrite() {
		try {
			RandomAccessFile raf = new RandomAccessFile(fileDir + fileName,"rw"); // 以读写权限访问文件
			// 计算插入位置
			long offset = raf.length() - fileEndStr.getBytes().length - fileDownlooadLink.getBytes().length;
			raf.seek(offset); // 移动位置
			raf.write(content.getBytes()); // 插入内容
			raf.write(fileDownlooadLink.getBytes()); // 插入数据会覆盖后面的内容，所以这里重新写入
			raf.write(fileEndStr.getBytes()); // 插入数据会覆盖后面的内容，所以这里重新写入
			raf.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
}
