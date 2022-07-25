# SimpleChat

这是一个基于MFC(微软基础库)和FTP(工作在TCP模型的第四层应用层的文件传输协议)的聊天室项目  
预计暑假中期上线(

## 已知问题
**SC-1** 由于需要utf-8支持,转换代码中使用了隐式转换文件的长度,但因为UINT64和DWORD(INT32)转换可能会导致文件过长时丢失精度.    

~~**SC-2** 由于使用了CString对象的find方法在验证用户名和密码时只需要符合用户文件的一个任意字符(比如空格)即可登录.~~**(这个问题其实是因为测试时用户名和密码相同)**  

**SC-3** 在聊天记录过长时,显示的字符串长度会超出文本框所能显示的.  
  
**SC-4** 登陆时由于项目中哈希算法的随机性(?)需要多按几次登录按钮才能登录
