# SimpleChat

这是一个基于MFC(微软基础库)和FTP(工作在TCP模型的第四层应用层的文件传输协议)的聊天室项目  
预计暑假中期上线(

## 已知问题
**SC-1** 由于需要utf-8支持,转换代码中使用了隐式转换文件的长度,但因为UINT64和DWORD(INT32)转换可能会导致文件过长时丢失精度.
