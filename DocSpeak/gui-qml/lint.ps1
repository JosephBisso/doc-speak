$qmlFiles = Get-ChildItem *.qml, *.js | Select-Object -ExpandProperty Name

$qmllint = Get-Command qmllint.exe -ErrorAction SilentlyContinue
if ($qmllint) {
    . $qmllint --check-unqualified $qmlFiles

} else {
    Write-Host ("`t`tSkipped`n|-> Cannot find qmllint.exe") -ForegroundColor Yellow
    exit -1
}