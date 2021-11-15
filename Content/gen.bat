@echo off
taskkill /F /IM UE4Editor.exe

setlocal enabledelayedexpansion

set DIR="%cd%"/PyCode
echo DIR=%DIR%

rd /s /Q %DIR%

set str=%DIR%
for /f %%i in ('call vstr_len %str%') do set str_len=%%i
echo %str_len%


echo import common > pub_fairy_gui.py
echo import os >> pub_fairy_gui.py
echo import sys >> pub_fairy_gui.py
echo sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))) >> pub_fairy_gui.py

for /R %DIR% %%i in (*.py) do (
set TMPSTR=%%i
set TMPSTR=!TMPSTR:\=/!
echo common.load_or_create("!TMPSTR:~%str_len%%,-3!"^) >> pub_fairy_gui.py 
)
for /R %DIR% %%i in (*.py) do (
set TMPSTR=%%i
set TMPSTR=!TMPSTR:\=.!
echo import PyCode.!TMPSTR:~%str_len%,-3%! >> pub_fairy_gui.py 
)

echo "-------------------------------------------- del file succ"
:: "E:\Program Files\Epic Games\UE_4.26\Engine\Binaries\Win64\UE4Editor-Cmd.exe"  "E:\FUI\utest\FairyGUI-unreal-example\FGUITestProject.uproject" -stdout -ExecutePythonScript="E:\FUI\utest\FairyGUI-unreal-example\Content\Python\main.py"

:: 后台运行
"E:\Program Files\Epic Games\UE_4.26\Engine\Binaries\Win64\UE4Editor-Cmd.exe" "E:\FUI\utest\FairyGUI-unreal-example\FGUITestProject.uproject" -stdout -run=pythonscript -script="E:\FUI\utest\FairyGUI-unreal-example\Content\Python\main.py"

echo "-------------------------------------------- work done!"



:: py "E:\FUI\utest\FairyGUI-unreal-example\Content\Python\init_unreal.py"

