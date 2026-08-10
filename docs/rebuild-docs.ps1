$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
if ($ScriptDir) { Set-Location $ScriptDir }
Write-Host "Rebuilding documentation with MkDocs..."
mkdocs build --config-file ../mkdocs.yml