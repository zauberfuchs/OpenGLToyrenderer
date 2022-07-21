SET EXE_VCPKG="I:/vcpkg/vcpkg.exe"
SET TRIPLET=x64-windows

SET PAKETS=assimp ^
glm ^
glew ^
glfw3 ^
imgui[opengl3-glew-binding] ^
imgui[glfw-binding]

%EXE_VCPKG% --triplet %TRIPLET% install %PAKETS%
%EXE_VCPKG% --triplet %TRIPLET% export %PAKETS% --zip

pause
