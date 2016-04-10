REM Copy assets.
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\vertex_only_ndc.vert.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\red.frag.spv .\assets\shader\SPIR\V\

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk
