@echo off

set VCTargetsPath=c:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120

msbuild ..\Omicron.sln /v:n /p:Configuration=omicron
msbuild ..\Omicron.sln /v:n /p:Configuration=qto

echo.
echo --------------------------------
echo.

