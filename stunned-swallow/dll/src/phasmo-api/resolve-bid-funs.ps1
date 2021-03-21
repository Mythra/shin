param (
  [Parameter(Position=0)]
  [String]$bid,
  [Parameter(ValueFromRemainingArguments=$true)]
  [String[]]$srcs
)

$start_dir = Get-Location
$MappedFns=(Get-Content -Raw -Path "$start_dir/../versioned/version_mapping.json" | ConvertFrom-Json)

if ("$bid" -in $MappedFns.PSobject.Properties.Name) {
  foreach ($src in $srcs) {
    $dst=($src.Trim("_tmpl"))
    Copy-Item -Force "$start_dir/../src/phasmo-api/$src" "$start_dir/src/phasmo-api/$dst"
    ((Get-Content -path "$start_dir/src/phasmo-api/$dst" -Raw) -replace '`LOCALISATION_RSRC_LOAD`', ('app::' + $MappedFns.$bid.LOCALISATION_RSRC_LOAD)) | Set-Content -Path "$start_dir/src/phasmo-api/$dst"
    ((Get-Content -path "$start_dir/src/phasmo-api/$dst" -Raw) -replace '`LOCALISATION_RSRC_MI`', ('app::' + $MappedFns.$bid.LOCALISATION_RSRC_MI)) | Set-Content -Path "$start_dir/src/phasmo-api/$dst"
    ((Get-Content -path "$start_dir/src/phasmo-api/$dst" -Raw) -replace '`TEXT_ASSET_AS_STR`', ('app::' + $MappedFns.$bid.TEXT_ASSET_AS_STR)) | Set-Content -Path "$start_dir/src/phasmo-api/$dst"
    ((Get-Content -path "$start_dir/src/phasmo-api/$dst" -Raw) -replace '`GET_LOCALISED_STR`', ('app::' + $MappedFns.$bid.GET_LOCALISED_STR)) | Set-Content -Path "$start_dir/src/phasmo-api/$dst"
    ((Get-Content -path "$start_dir/src/phasmo-api/$dst" -Raw) -replace '`GET_SAVE_FILE_PATH`', ('app::' + $MappedFns.$bid.GET_SAVE_FILE_PATH)) | Set-Content -Path "$start_dir/src/phasmo-api/$dst"
    ((Get-Content -path "$start_dir/src/phasmo-api/$dst" -Raw) -replace '`FILE_READ_ALL`', ('app::' + $MappedFns.$bid.FILE_READ_ALL)) | Set-Content -Path "$start_dir/src/phasmo-api/$dst"
    ((Get-Content -path "$start_dir/src/phasmo-api/$dst" -Raw) -replace '`DATA_SCRAMBLER`', ('app::' + $MappedFns.$bid.DATA_SCRAMBLER)) | Set-Content -Path "$start_dir/src/phasmo-api/$dst"
  }
} else {
  Write-Host "Unknown Build ID!"
}