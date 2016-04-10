@echo off

for /D /r %%D in (*) do (

	cd %%D

	for /r %%F in ("*.vert") do (
	  echo Building vertex shader '%%~nxF'
	  glslangValidator.exe -V %%~nxF | findstr "ERROR"
	  move /Y vert.spv ..\..\SPIR\V\%%~nxF.spv >nul 2>&1
	  echo.
	)

	for /r %%F in ("*.tesc") do (
	  echo Building tessellation control shader '%%~nxF'
	  glslangValidator.exe -V %%~nxF | findstr "ERROR"
	  move tesc.spv ..\..\SPIR\V\%%~nxF.spv >nul 2>&1
	  echo.
	)
	
	for /r %%F in ("*.tese") do (
	  echo Building tessellation evaluation shader '%%~nxF'
	  glslangValidator.exe -V %%~nxF | findstr "ERROR"
	  move tese.spv ..\..\SPIR\V\%%~nxF.spv >nul 2>&1
	  echo.
	)
	
	for /r %%F in ("*.geom") do (
	  echo Building geometry shader '%%~nxF'
	  glslangValidator.exe -V %%~nxF | findstr "ERROR"
	  move geom.spv ..\..\SPIR\V\%%~nxF.spv >nul 2>&1
	  echo.
	)
	
	for /r %%F in ("*.frag") do (
	  echo Building fragment shader '%%~nxF'
	  glslangValidator.exe -V %%~nxF | findstr "ERROR"
	  move frag.spv ..\..\SPIR\V\%%~nxF.spv >nul 2>&1
	  echo.
	)
	
	for /r %%F in ("*.comp") do (
	  echo Building compute shader '%%~nxF'
	  glslangValidator.exe -V %%~nxF | findstr "ERROR"
	  move /Y comp.spv ..\..\SPIR\V\%%~nxF.spv >nul 2>&1
	  echo.
	)

	cd ..
	
)
