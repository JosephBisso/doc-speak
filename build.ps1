Function Build-Tests {

    $wget = Get-Command wget.exe
    if ($wget) {
        foreach($extension in "pbmm", "scorer") {
            if (!(Test-Path ".\lib\DeepSpeech\deepspeech-0.9.3-models.$extension")) {
                Write-Host ("!! Downloading deepspeech-0.9.3-models.$extension") -ForegroundColor Cyan
                wget "https://github.com/mozilla/DeepSpeech/releases/download/v0.9.3/deepspeech-0.9.3-models.$extension" -O ".\lib\DeepSpeech\deepspeech-0.9.3-models.$extension"
            }
        }
    }

    Write-Host "> Building..." -ForegroundColor Cyan
    cmake -G Ninja -S . -B build
    cmake --build build --target unit_tests
}

Function Start-Tests($repo) {
    Write-Host "`n> Running..." -ForegroundColor Cyan
    Push-Location build/bin && ctest --test-dir "$repo\build\DocSpeak\tests"
    Pop-Location
}

if ($args) {
    switch ($args) {
        "clean" { 
            Write-Host ">> Cleaning build directory..." -ForegroundColor Cyan
            if (Test-Path build) {
                Remove-Item -Recurse -Force build 
            }
        }
        "--build-only" { 
            Write-Host ">> Build the Tests only" -ForegroundColor Cyan
            Build-Tests
        }
        Default {
            Write-Host "! Unknown argument: $args" -ForegroundColor Yellow
        }
    }

} else {
    Build-Tests
    
    $repo = "$PWD"
    if ($LASTEXITCODE -eq 0 -and '--build-only' -notin $args) {
       Run-Tests($repo)
    }
}

$color = if ($LASTEXITCODE -eq 0) {"green"} else {"yellow"}
Write-Host "> Done" -ForegroundColor $color
exit $LASTEXITCODE