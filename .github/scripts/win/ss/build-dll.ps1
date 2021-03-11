param (
  [string]$bid = "none"
)

$start_dir = Get-Location

try {
  cd "$PSScriptRoot\..\..\..\..\stunned-swallow\dll\"

  if(!(Test-Path -Path ".\builddir")) {
    Write-Host "Running Messon Setup... May Take awhile."
    meson setup builddir
  }

  if ($bid -ne "none") {
    cd builddir
    meson configure -Dbuild-id="$bid"
    cd ..
  } else {
    cd builddir
    meson configure -Dbuild-id="b6326492"
    cd ..
  }

  meson compile -C builddir
  cd $start_dir
} catch {
  cd $start_dir
}