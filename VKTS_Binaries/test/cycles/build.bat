@echo off

for /r %%F in ("*.vert") do (
  echo Building vertex shader '%%~nxF'
  glslangValidator.exe -V %%~nxF | findstr "ERROR"
  move /Y vert.spv %%~nxF.spv >nul 2>&1
  echo.
)

for /r %%F in ("*.tesc") do (
  echo Building tessellation control shader '%%~nxF'
  glslangValidator.exe -V %%~nxF | findstr "ERROR"
  move tesc.spv %%~nxF.spv >nul 2>&1
  echo.
)

for /r %%F in ("*.tese") do (
  echo Building tessellation evaluation shader '%%~nxF'
  glslangValidator.exe -V %%~nxF | findstr "ERROR"
  move tese.spv %%~nxF.spv >nul 2>&1
  echo.
)

for /r %%F in ("*.geom") do (
  echo Building geometry shader '%%~nxF'
  glslangValidator.exe -V %%~nxF | findstr "ERROR"
  move geom.spv %%~nxF.spv >nul 2>&1
  echo.
)

for /r %%F in ("*.frag") do (
  echo Building fragment shader '%%~nxF'
  glslangValidator.exe -V %%~nxF | findstr "ERROR"
  move frag.spv %%~nxF.spv >nul 2>&1
  echo.
)

for /r %%F in ("*.comp") do (
  echo Building compute shader '%%~nxF'
  glslangValidator.exe -V %%~nxF | findstr "ERROR"
  move /Y comp.spv %%~nxF.spv >nul 2>&1
  echo.
)
