REM Copy assets.
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\vertex_normal_texcoord.vert.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\texture_light.frag.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\textured_cube\*.vkts .\assets\textured_cube\
xcopy /y ..\..\VKTS_Binaries\textured_cube\*.tga .\assets\textured_cube\

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk
