@echo off

title Fusion 2.5+ DX11 Shader ���빤�� - Custom By Defisym
@rem mode con cols=72 lines=20
mode con cols=96
color 3F

:Init

setlocal
set error=0

@rem set FXCOPTS=/nologo /WX /Ges /Zi /Zpc /Qstrip_reflect /Qstrip_debug
set FXCOPTS=/nologo /WX /Ges /Qstrip_reflect /Qstrip_debug
set FXCOPTSNS=/nologo /Ges /Qstrip_reflect /Qstrip_debug

set PCFXC=fxc\fxc.exe
set OUTPUT_PATH=%~dp0\Shader_Output\

:0

cls
echo ------------------------------------
echo Fusion 2.5+ DX11 Shader ���빤��
echo Custom By Defisym
echo.
echo Ver. 1.5.190404
echo ------------------------------------
echo.

cd /d %~dp0

:1

echo �뽫��Ҫ�����shader�ļ� (��չ��.hlsl) ���������ļ��и�Ŀ¼
echo ��֤���������ļ�����ͬһ·����
echo.
echo ģʽ��1���������ļ�������ָ���ļ�
echo.
echo ģʽ��2�������뵱ǰ·����ȫ���ļ�
echo.
echo ģʽ��3�����˳�����
echo.

set /p Mode_Select=������ģʽ�����֣���

set mode=0
if %Mode_Select% equ 1 goto :mode1
if %Mode_Select% equ 2 goto :mode2
if %Mode_Select% equ 3 (	
	endlocal
	exit /b	
)

cls
echo.
echo ģʽ����
echo.

goto :1


:mode1
set mode=1

cls
echo.
set /p FileName=������shader�ļ��� (��������չ��) ��
echo.

set error=0
goto :compile

:mode2
set mode=2

cls
echo.
echo ��ʼ���롭��
echo.

for /f  %%a in ('dir  /b "*.hlsl*" ') do (
	setlocal EnableDelayedExpansion
	
	set error=0	
	set FileName=%%~na
	call :compile

)

echo.
echo �������
echo.

echo.
@pause

goto :0

@rem ------------------------------------------------
@rem Compile

:compile

@rem *** compile non-premultiplied version
set FXCEXT=fxc

echo ------------------------------------
echo.
echo ���ڱ���DX11 ��Ԥ�� �汾����

call :CompileShaderHLSL4 "%FileName%" ps ps_main

echo .
if %error% == 0 (
    echo DX11 ��Ԥ�� �汾����ɹ���
	echo.
) else (
    echo DX11 ��Ԥ�� �汾�������
	echo.
)

@rem *** compile premultiplied version
set FXCEXT=premultiplied.%FXCEXT%

echo ------------------------------------
echo.
echo ���ڱ���DX11 Ԥ�� �汾����

call :CompileShaderHLSL4 "%FileName%" ps ps_main_pm

echo .
if %error% == 0 (
    echo DX11 Ԥ�� �汾����ɹ���
	echo.
) else (
    echo DX11 Ԥ�� �汾�������
	echo.
)


if %mode% equ 1 (

	echo ------------------------------------
	echo �������
	echo ------------------------------------
	echo.
	
	@pause
	goto :mode1
)

if %mode% equ 2 (	
	exit /b
)

@rem ------------------------------------------------
@rem CompileShader

:CompileShader
@rem set fxc=%PCFXC% %1.fx %FXCOPTS% /T%2_4_0_level_9_1 /E%3 /FhCompiled\%1_%3.inc /FdCompiled\%1_%3.pdb /Vn%1_%3
set fxc=%PCFXC% "%~1.fx11" %FXCOPTS% /T%2_4_0_level_9_1 /E%3 /Fo"%~1.%FXCEXT%"
echo.
echo %fxc%
%fxc% || set error=1
exit /b

:CompileShaderSM4
@rem set fxc=%PCFXC% %1.fx %FXCOPTS% /T%2_4_0 /E%3 /FhCompiled\%1_%3.inc /FdCompiled\%1_%3.pdb /Vn%1_%3
set fxc=%PCFXC% "%~1.fx11" %FXCOPTS% /T%2_4_0 /E%3 /Fo"%~1.%FXCEXT%"
echo.
echo %fxc%
%fxc% || set error=1
exit /b

:CompileShaderHLSL
@rem set fxc=%PCFXC% %1.hlsl %FXCOPTS% /T%2_4_0_level_9_1 /E%3 /FhCompiled\%1_%3.inc /FdCompiled\%1_%3.pdb /Vn%1_%3
set fxc=%PCFXC% "%~1.hlsl" %FXCOPTS% /T%2_4_0_level_9_1 /E%3 /Fo"%~1.%FXCEXT%"
echo.
echo %fxc%
%fxc% || set error=1
exit /b

:CompileShaderHLSL4
@rem set fxc=%PCFXC% %1.hlsl %FXCOPTS% /T%2_4_0_level_9_1 /E%3 /FhCompiled\%1_%3.inc /FdCompiled\%1_%3.pdb /Vn%1_%3
@rem set fxc=%PCFXC% "%~1.hlsl" %FXCOPTS% /T%2_4_0 /E%3 /Fo"%~1.%FXCEXT%"
set fxc=%PCFXC% "%~1.hlsl" %FXCOPTS% /T%2_4_0 /E%3 /Fo"%OUTPUT_PATH%%~1.%FXCEXT%"

echo.
echo %fxc%
%fxc% || set error=1
exit /b

:CompileShaderHLSL4ns
@rem set fxc=%PCFXC% %1.hlsl %FXCOPTS% /T%2_4_0_level_9_1 /E%3 /FhCompiled\%1_%3.inc /FdCompiled\%1_%3.pdb /Vn%1_%3
set fxc=%PCFXC% "%~1.hlsl" %FXCOPTSNS% /T%2_4_0 /E%3 /Fo"%~1.%FXCEXT%"
echo.
echo %fxc%
%fxc% || set error=1
exit /b



@rem ------------------------------------------------
@rem Hello from Dalian China