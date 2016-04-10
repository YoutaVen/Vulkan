REM Copy assets.
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong_skinning.vert.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong_skinning.frag.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\astro_boy\*.vkts .\assets\astro_boy\
xcopy /y ..\..\VKTS_Binaries\astro_boy\*.tga .\assets\astro_boy\

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk
