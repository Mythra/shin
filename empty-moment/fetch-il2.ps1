param (
  [Parameter(mandatory=$true)]
  [String[]]$bids,
  [Parameter(mandatory=$true)]
  [String]$bucket
)

if ([string]::IsNullOrWhiteSpace($bucket)) {
  Write-Host "Please provide a bucket name!"
  exit
}

foreach ($buildId in $bids) {
  if (!(Test-Path "./$buildId")) {
    New-Item -ItemType Directory -Path "./$buildId"
  }
  # Fetch GameAssembly.dll which is the actual unity code.
  b2 download-file-by-name "$bucket" "$buildId/GameAssembly.dll" "$buildId/GameAssembly.dll"
  # Fetch global-metadata.dat which contains the mappings.
  b2 download-file-by-name "$bucket" "$buildId/Phasmophobia_Data/il2cpp_data/Metadata/global-metadata.dat" "$buildId/global-metadata.dat"
  # Fetch the resource file for automatically detecting the unity version.
  b2 download-file-by-name "$bucket" "$buildId/Phasmophobia_Data/Resources/unity default resources" "$buildId/unity default resources"
}