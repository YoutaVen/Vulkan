REM Copy assets.
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong.vert.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong.frag.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\cartoon_tree_with_plane\*.vkts .\assets\cartoon_tree_with_plane\
xcopy /y ..\..\VKTS_Binaries\cartoon_tree_with_plane\*.tga .\assets\cartoon_tree_with_plane\

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk
