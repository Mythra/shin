if ([string]::IsNullOrWhiteSpace($DISCORD_WEBHOOK)) {
  $DISCORD_WEBHOOK=$env:DISCORD_WEBHOOK
  if ([string]::IsNullOrWhiteSpace($DISCORD_WEBHOOK)) {
    Write-Host "No Discord Webhook provided for providing Updates!"
    exit
  }
}
if ([string]::IsNullOrWhiteSpace($STEAM_USERNAME)) {
  $STEAM_USERNAME=$env:STEAM_USERNAME
  if ([string]::IsNullOrWhiteSpace($STEAM_USERNAME)) {
    Write-Host "No Steam Username provided for providing Updates!"
    exit
  }
}
if ([string]::IsNullOrWhiteSpace($STEAM_PASSWORD)) {
  $STEAM_PASSWORD=$env:STEAM_PASSWORD
  if ([string]::IsNullOrWhiteSpace($STEAM_PASSWORD)) {
    Write-Host "No Steam Password provided for providing Updates!"
    exit
  }
}
if ([string]::IsNullOrWhiteSpace($B2BUCKET_NAME)) {
  $B2BUCKET_NAME=$env:B2BUCKET_NAME
  if ([string]::IsNullOrWhiteSpace($B2BUCKET_NAME)) {
    Write-Host "No Backblaze Bucket Name Provided!"
    exit
  }
}

$DISCORD_WEBHOOK = ($DISCORD_WEBHOOK -Replace('\r\n',''))
$DISCORD_WEBHOOK = ($DISCORD_WEBHOOK -Replace('\n',''))

$PreExistingManifestFiles = (dir -Path ./depots/739631 -Filter manifest_* -Recurse | %{$_.FullName}).Split([Environment]::NewLine)
foreach ($ManifestFile in $PreExistingManifestFiles) {
  rm $ManifestFile
}
dotnet DepotDownloader.dll -username "$STEAM_USERNAME" -password "$STEAM_PASSWORD" -remember-password -app 739630 -depot 739631 -beta beta -os windows -osarch 64 -manifest-only
$PostExistingManifestFiles = (dir -Path ./depots/739631 -Filter manifest_* -Recurse | %{$_.FullName}).Split([Environment]::NewLine)
if ($PostExistingManifestFiles.Count -eq 0) {
  Write-Host "Failed to log into manifest file!"
  if (Test-Path "last-was-error.txt" -PathType leaf) {
    Write-Host "Pinged Discord Already, doing nothing."
  } else {
    $MsgBody=(@{username = "Empty Moment"; avatar_url = "https://i.ytimg.com/vi/fofrXxlL4No/maxresdefault.jpg"; content = "<@&819406153082470410> Failed to fetch manifest file! Credentials Expired!"} | ConvertTo-Json -Depth 5 -EscapeHandling Default)
    Invoke-WebRequest -Uri "$DISCORD_WEBHOOK" -Method POST -Headers @{ "Content-Type" = "application/json" } -Body "$MsgBody"
    New-Item -Name "last-was-error.txt" -ItemType File
  }

  exit
}
Remove-Item "last-was-error.txt" -ErrorAction Ignore

if (!$PreExistingManifestFiles.contains($PostExistingManifestFiles[0])) {
  dotnet DepotDownloader.dll -username "$STEAM_USERNAME" -password "$STEAM_PASSWORD" -remember-password -app 739630 -beta beta -os windows -osarch 64 -validate
  cd 'depots/739631'
  $internal_measure_post=(Get-ChildItem -Directory | Measure-Object)
  cd '../../'
  $PostExistingFolderCount=$internal_measure_post.Count
  if ($PostExistingFolderCount -ne 1) {
    $latestDirObj=(Get-ChildItem "./depots/739631" | Sort-Object CreationTime -Descending | Select -First 1)
    $buildId=$latestDirObj.Name
    $MsgBody=(@{username = "Empty Moment"; avatar_url = "https://i.ytimg.com/vi/fofrXxlL4No/maxresdefault.jpg"; content = "<@&819406153082470410> Starting Archive for $buildID"} | ConvertTo-Json -Depth 5 -EscapeHandling Default)
    Invoke-WebRequest -Uri "$DISCORD_WEBHOOK" -Method POST -Headers @{ "Content-Type" = "application/json" } -Body "$MsgBody"
    b2 sync "./depots/739631/$buildId/" "b2://$B2BUCKET_NAME/b$buildId/"
    $oldestDirObj=(Get-ChildItem "./depots/739631" | Sort-Object CreationTime | Select -First 1)
    rm -r "./depots/739631/${oldestDirObj.Name}"
    $MsgBody=(@{username = "Empty Moment"; avatar_url = "https://i.ytimg.com/vi/fofrXxlL4No/maxresdefault.jpg"; content = "<@&819406153082470410> Finished Archive for $buildID"} | ConvertTo-Json -Depth 5 -EscapeHandling Default)
    Invoke-WebRequest -Uri "$DISCORD_WEBHOOK" -Method POST -Headers @{ "Content-Type" = "application/json" } -Body "$MsgBody"
  }
} else {
  Write-Host "No New Build."
}