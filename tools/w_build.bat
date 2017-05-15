echo ----------------
echo omicron_config
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omicron_config

echo ----------------
echo omicron_report
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omicron_report

echo ----------------
echo omicron_runtime
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omicron_runtime

echo ----------------
echo omi_qt
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omi_qt

echo ----------------
echo omi_gl
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omi_gl

echo ----------------
echo pxtrace
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=pxtrace

echo ----------------
echo omi_bullet
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omi_bullet

echo ----------------
echo omi_al
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omi_al

