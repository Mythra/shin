$start_dir = Get-Location

try {
  cd "$PSScriptRoot\..\..\..\..\stunned-swallow\ui\"

  if(!(Test-Path -Path ".\builddir")) {
    Write-Host "Running Messon Setup... May Take awhile."
    meson setup builddir
  }

  meson compile -C builddir
  cd $start_dir
} catch {
  cd $start_dir
}