param (
  [Parameter(mandatory=$true)]
  [String]$srcBid,
  [Parameter(mandatory=$true)]
  [String[]]$dstBids
)

$ScriptDir=$PSScriptRoot
$VersionMapping=(Get-Content -Raw -Path "$ScriptDir/version_mapping.json" | ConvertFrom-Json)

if (-Not ("$srcBid" -in $VersionMapping.PSobject.Properties.Name)) {
  throw "$srcBid not in version_mapping.json!"
}
if (-Not (Test-Path -Path "$ScriptDir/$srcBid/metadata.json")) {
  throw "$srcBid has no metadata.json present!"
}

$SrcMetadata=(Get-Content -Raw -Path "$ScriptDir/$srcBid/metadata.json" | ConvertFrom-Json)

foreach ($dstBid in $dstBids) {
  if (-Not (Test-Path -Path "$ScriptDir/$dstBid/metadata.json")) {
    throw "$dstBid has no metadata.json present!"
  }
  $dstMetadata=(Get-Content -Raw -Path "$ScriptDir/$dstBid/metadata.json" | ConvertFrom-Json)
  $dstBidObj=('{}' | ConvertFrom-Json)

  foreach ($property in $VersionMapping.$srcBid.PSObject.Properties) {
    $UserFuncName = $property.Name
    $MappedNameSrc = $property.Value

    if ($MappedNameSrc.EndsWith('__MethodInfo')) {
      $genericMethodSrc=( $SrcMetadata.addressMap.methodInfoPointers | Where-Object { $_.name -eq "$MappedNameSrc" } )
      $MatchableName=($MappedNameSrc.Split("_")[0])
      $dstMetadataOptions=( $dstMetadata.addressMap.methodInfoPointers | Where-Object { $_.name.StartsWith($MatchableName) } )
      $dstMatches=@()

      foreach ($dstOption in $dstMetadataOptions) {
        if ($dstOption.dotNetSignature -eq $genericMethodSrc.dotNetSignature) {
          $dstMatches += $dstOption.name
        }
      }

      if ($dstMatches.Length -eq 0) {
        Write-Host "Failed to find generic match based on signature for $MappedNameSrc"
      } ElseIf ($dstMatches.Length -eq 1) {
        $dstBidObj | Add-Member -Type NoteProperty -Name "$UserFuncName" -Value "$($dstMatches[0])"
      } else {
        Write-Host "Found multiple matches for signature for $MappedNameSrc for build id: $dstBid"
        foreach ($dstOption in $dstMatches) {
          Write-Host "Duplicate: $dstOption - $MappedNameSrc - $dstBid"
        }
      }
    } else {
      $genericMethodSrc=( $SrcMetadata.addressMap.constructedGenericMethods | Where-Object { $_.name -eq "$MappedNameSrc" } )

      if ($genericMethodSrc -eq $null) {
        # This is not a generic method, attempt to just find one by same name.
        $dstMethodExists=( $dstMetadata.addressMap.methodDefinitions | Where-Object { $_.name -eq "$MappedNameSrc" } )
        if ($dstMethodExists -eq $null) {
          Write-Host "Failed to find non-generic method: $MappedNameSrc in build: $dstBid"
        } else {
          $dstBidObj | Add-Member -Type NoteProperty -Name "$UserFuncName" -Value "$MappedNameSrc"
        }
      } else {
        # Is a Generic Object, find matches.
        $MatchableName=($MappedNameSrc.Split("_")[0])
        $dstMetadataOptions=( $dstMetadata.addressMap.constructedGenericMethods | Where-Object { $_.name.StartsWith($MatchableName) } )
        $dstMatches=@()

        foreach ($dstOption in $dstMetadataOptions) {
          if ($dstOption.dotNetSignature -eq $genericMethodSrc.dotNetSignature) {
            $dstMatches += $dstOption.name
          }
        }

        if ($dstMatches.Length -eq 0) {
          Write-Host "Failed to find generic match based on signature for $MappedNameSrc"
        } ElseIf ($dstMatches.Length -eq 1) {
          $dstBidObj | Add-Member -Type NoteProperty -Name "$UserFuncName" -Value "$($dstMatches[0])"
        } else {
          Write-Host "Found multiple matches for signature for $MappedNameSrc for build id: $dstBid"
          foreach ($dstOption in $dstMatches) {
            Write-Host "Duplicate: $dstOption - $MappedNameSrc - $dstBid"
          }
        }
      }
    }
  }

  $VersionMapping | Add-Member -Type NoteProperty -Name "$dstBid" -Value $dstBidObj
}

$VersionMapping | ConvertTo-Json -depth 100 | Set-Content "$ScriptDir/version_mapping.json"