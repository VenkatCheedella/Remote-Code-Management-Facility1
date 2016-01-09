:run.bat
@echo off
                                            


start "Server 1080" "Debug/Executive.exe" localhost 1080

start "Server 1081" "Debug/Executive.exe" localhost 1081
