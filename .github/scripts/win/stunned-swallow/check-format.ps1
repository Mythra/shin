$start_dir = Get-Location

$cf_internal=(Get-Command clang-format)
if ($cf_internal -eq $null) {
  throw "Please install the clang-format cli tool to format code."
}

try {
  cd "$PSScriptRoot\..\..\..\..\stunned-swallow\dll\"
  python "$PSScriptRoot\..\..\run-clang-format.py" --recursive "./src"
  if (-Not $?) {
    throw "Clang format failed on DLL"
  }
  cd "$PSScriptRoot\..\..\..\..\stunned-swallow\ui\"
  python "$PSScriptRoot\..\..\run-clang-format.py" --recursive "./src"
  if (-Not $?) {
    throw "Clang format failed on UI"
  }
  cd $start_dir
} catch {
  cd $start_dir
  throw
}