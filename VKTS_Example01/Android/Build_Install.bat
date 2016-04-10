REM Copy assets.
REM Nothing for this example.

cd jni
call ndk-build
cd ..
call ant debug
call adb install -r bin/NativeActivity-debug.apk
