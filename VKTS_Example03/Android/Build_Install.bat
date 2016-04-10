REM Copy assets.
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\texture_ndc.vert.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\texture.frag.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\texture\crater_lake.tga .\assets\texture\

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk
