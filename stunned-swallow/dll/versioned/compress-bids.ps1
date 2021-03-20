param (
  [Parameter(mandatory=$true)]
  [String[]]$dstBids,
  [switch]$gitRm = $false
)

$brotli=(Get-Command brotli)
if ($brotli -eq $null) {
  throw "Please install the brotli cli tool, to compress metadatas."
}

$ScriptDir=$PSScriptRoot
foreach ($dstBid in $dstBids) {
  Write-Host "Compressing for $dstBid..."
  brotli -kfZo "$ScriptDir/$dstBid/metadata.json.brotli" -- "$ScriptDir/$dstBid/metadata.json"
  if ($gitRm) {
    git rm --cached "$ScriptDir/$dstBid/metadata.json"
  }
}
Write-Host "Done!"