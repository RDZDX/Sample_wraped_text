"C:\Program Files\MRE_SDK\tools\DllPackage.exe" "D:\MyGitHub\Sample_wraped_text\Sample_wraped_text.vcproj"
if %errorlevel% == 0 (
 echo postbuild OK.
  copy Sample_wraped_text.vpp ..\..\..\MoDIS_VC9\WIN32FS\DRIVE_E\Sample_wraped_text.vpp /y
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)

