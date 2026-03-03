param(
    [string]$BuildDir = "build",
    [string]$InstallPrefix = "package",
    [string]$Generator = "Visual Studio 17 2022",
    [string]$Architecture = "x64",
    [string]$GeneratorInstance = "C:/Program Files/Microsoft Visual Studio/2022/Community",
    [switch]$Reconfigure
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildPath = Join-Path $scriptDir $BuildDir
$cachePath = Join-Path $buildPath "CMakeCache.txt"
$configureArgs = @("-S", $scriptDir, "-B", $buildPath)
$installPath = Join-Path $scriptDir $InstallPrefix

if($Reconfigure -or -not (Test-Path $cachePath)) {
    $configureArgs += @("-G", $Generator, "-A", $Architecture)

    if(Test-Path $GeneratorInstance) {
        $configureArgs += "-DCMAKE_GENERATOR_INSTANCE=$GeneratorInstance"
    }
}

Write-Host "Configuring project..."
& cmake @configureArgs

foreach($config in @("Debug", "Release")) {
    Write-Host "Building $config..."
    & cmake --build $buildPath --config $config

    Write-Host "Installing $config to $installPath..."
    & cmake --install $buildPath --config $config --prefix $installPath
}

Write-Host "Build artifacts are available in $installPath"
