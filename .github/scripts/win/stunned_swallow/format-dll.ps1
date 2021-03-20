$start_dir = Get-Location

try {
  cd "$PSScriptRoot\..\..\..\..\stunned-swallow\dll\"

  if(!(Test-Path -Path ".\builddir")) {
    Write-Host "Running Meson Setup... May Take awhile."
    meson setup builddir
  }

  cd ".\builddir"
  ninja clang-format
  cd $start_dir
} catch {
  cd $start_dir
}