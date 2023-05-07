$qmlFiles = Get-ChildItem *.qml, *.js | Select-Object -ExpandProperty Name

if ($True -and (Test-Path "C:\Qt\6.2.3\msvc2019_64\bin\qmllint.exe")) {
    $qmllint = "C:\Qt\6.2.3\msvc2019_64\bin\qmllint.exe"

    . $qmllint --resource qml.qrc --unqualified info --unused-imports info $qmlFiles

} else {
    $qmllint = Get-Command qmllint.exe -ErrorAction SilentlyContinue

    if (-not $qmllint) {
        Write-Host ("`t`tSkipped`n|-> Cannot find qmllint.exe") -ForegroundColor Yellow
        exit -1    
    }

    . $qmllint --check-unqualified $qmlFiles
}

