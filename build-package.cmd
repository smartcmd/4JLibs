@echo off
powershell -ExecutionPolicy Bypass -File "%~dp0build-package.ps1" %*
