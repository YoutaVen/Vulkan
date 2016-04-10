REM Copy assets.
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong_displace.vert.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong_displace.tesc.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong_displace.tese.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong_displace.geom.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\phong_displace.frag.spv .\assets\shader\SPIR\V\
xcopy /y ..\..\VKTS_Binaries\valley_terrain\*.vkts .\assets\valley_terrain\
xcopy /y ..\..\VKTS_Binaries\valley_terrain\*.tga .\assets\valley_terrain\

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk
