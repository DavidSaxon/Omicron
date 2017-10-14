echo ----------------
echo omicron_api
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omicron_api

echo ----------------
echo omicron_runtime
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omicron_runtime

echo ----------------
echo omi_glfw
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=omi_glfw

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

echo ----------------
echo tests
echo ----------------
msbuild ..\Omicron.sln /v:n /p:Configuration=tests
