

#ifndef HANDLE_FILE_H
#define HANDLE_FILE_H

#include "server.h"

String formatBytes(size_t bytes);
String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();

#endif
