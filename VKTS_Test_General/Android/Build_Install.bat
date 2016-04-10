REM Copy assets.
xcopy /y ..\..\VKTS_Binaries\test\general\*.txt .\assets\test\general\
xcopy /y ..\..\VKTS_Binaries\test\general\*.tga .\assets\test\general\
xcopy /y ..\..\VKTS_Binaries\test\general\*.hdr .\assets\test\general\

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk