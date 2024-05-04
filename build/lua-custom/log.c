//
//  Log.c
//  XLuaC
//
//  Created by Bepic on 2024/5/4.
//

#include "log.h"
#include <sys/stat.h>

static int index_ = 0;

int folder_exists(const char *path) {
    struct stat info;
    if (stat(path, &info) != 0) {
        return 0; // 文件夹不存在
    } else if (info.st_mode & S_IFDIR) {
        return 1; // 文件夹存在
    }
    return 2; // 路径存在但不是一个文件夹
}

void sLog(const char *filename, int size, const char *buff) {
    index_ = index_ + 1;
    // const char *dirPath = "/Users/mac/Desktop/GameDeveloper/Debug/Project/Abyss/Extra/Log/write";
    // const char *dirPath = "/Users/wzf/Desktop/GameDeveloper/Unity3D/Project/Abyss/Extra/Log/write";
    const char *dirPath = "/Users/logs";
    if (folder_exists(dirPath) != 1) { return; }
    char filePath[1024];
    sprintf(filePath, "%s/%d_%s_%d.txt", dirPath, index_, filename, size); // 引用头文件 #include <stdio.h>
    FILE *fp;
    /*
      以附加方式打开可读写的文度件。
      若文件不存在，则会建立该文件，
      如果文件存在，写入的数据会被加到文件尾后，
      即文件原先的内容会被保留
    */
    if((fp=fopen(filePath, "a+"))==NULL) {  // 注意文件路径必须正确
        // printf("\nopen file error"); // 貌似无法输出信息
        // luaL_error(L, "储存数据出现异常:[%s_%d] = %s;", filename, size, buff);
        fprintf(fp, "print log error:[%s_%d] = %s;", filename, size, buff);
    } else {
        fprintf(fp, "%s", buff);
    }
    fclose(fp);
}

void sFile(int hasCrypto, const char *name, size_t size, const char *buff, const char *mode) {
    char filename[1024];
    sprintf(filename, "%d_%s_%s", hasCrypto, name, mode);
    sLog(filename, size, buff);
}