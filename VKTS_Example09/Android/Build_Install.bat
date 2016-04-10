REM Copy assets.
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong_skinning.vert.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong_skinning.frag.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\transport_shuttle\*.vkts .\assets\transport_shuttle\
xcopy /y ..\..\VKTS_Binaries\transport_shuttle\*.tga .\assets\transport_shuttle\

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk
