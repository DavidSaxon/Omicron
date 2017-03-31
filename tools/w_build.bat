set VCTargetsPath=c:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V120

msbuild ..\Omicron.sln /v:n /p:Configuration=omicron_runtime
msbuild ..\Omicron.sln /v:n /p:Configuration=omi_qt
msbuild ..\Omicron.sln /v:n /p:Configuration=omi_gl
msbuild ..\Omicron.sln /v:n /p:Configuration=pxtrace
msbuild ..\Omicron.sln /v:n /p:Configuration=omi_bullet
msbuild ..\Omicron.sln /v:n /p:Configuration=omi_al

