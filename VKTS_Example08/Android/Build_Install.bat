REM Copy assets.
xcopy /y ..\..\VKTS_Binaries\shader\SPIR\V\mandelbrot.comp.spv .\assets\shader\SPIR\V\

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk
