@REM @Author: Giannis
@REM @Date:   2021-05-02 06:13:26
@REM @Last Modified by:   Giannis
@REM Modified time: 2021-05-02 06:14:33

call %VK_SDK_PATH%\Bin\glslc.exe shader.vert -o vert.spv
call %VK_SDK_PATH%\Bin\glslc.exe shader.frag -o frag.spv
pause
