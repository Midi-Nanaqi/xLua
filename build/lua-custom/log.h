//
//  log.h
//  XLuaC
//
//  Created by Bepic on 2024/5/4.
//

#ifndef log_h
#define log_h

#include <stdio.h>

int folder_exists(const char *path);
void sLog(const char *filename, int size, const char *buff);
void sFile(int hasCrypto, const char *name, size_t size, const char *buff, const char *mode);

#endif /* log_h */
