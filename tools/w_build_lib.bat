echo ----------------
echo omicron_api
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omicron_api

echo ----------------
echo omicron_runtime
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omicron_runtime
