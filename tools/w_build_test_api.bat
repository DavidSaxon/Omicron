echo ----------------
echo omicron_api
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omicron_api

echo ----------------
echo tests
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=tests
