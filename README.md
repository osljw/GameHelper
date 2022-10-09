

# Todo

- 编译后应用程序文件的图标
- taskbar图标
	- taskbar上显示的图标
	- taskbar上鼠标悬停预览窗口左上角图标
		- 由WNDCLASSEXW中的hIcon进行控制， imgui的窗口类型在platform相关代码中设置


# proto 编译
1. nuget包管理安装grpc tools
2. 编译proto文件
```

./packages/Grpc.Tools.2.49.1/tools/windows_x64/protoc.exe --cpp_out=proto -Iproto record.proto


python -m grpc_tools.protoc --python_out=python --grpc_python_out=python -I. proto/record.proto
```