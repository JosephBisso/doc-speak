if ($args) {
    switch ($args) {
        "clean" { 
            Write-Host "Cleaning build directory..." -ForegroundColor Cyan
            if (Test-Path build) {
                rmdir -Recurse -Force build 
            }
        }
        Default {
            Write-Host "Unknown argument: $args" -ForegroundColor Yellow
        }
    }

} else {
    Write-Host "Building..." -ForegroundColor Cyan
    cmake -G Ninja -S . -B build
    cmake --build build --target unit_tests
    
    $repo = "$PWD"
    if ($LASTEXITCODE -eq 0) {
       pushd build/bin && ctest --test-dir "$repo\build\DocSpeak\tests"
       popd
    }
}

$color = if ($LASTEXITCODE -eq 0) {"green"} else {"yellow"}
Write-Host "Done" -ForegroundColor $color
exit $LASTEXITCODE