$ErrorActionPreference = "Stop"
<#
    Based on Artstation REST API
#>
function Get-RandomWallpaperUrl {
    $WebRespContent = (Invoke-WebRequest -Uri "https://artstation.com/random_project.json").Content
    $JsonResp = ConvertFrom-Json -InputObject $WebRespContent
    $ImageUrl = $JsonResp.assets[0].image_url
    return $ImageUrl
}

function Get-ImageExtension {
    Param(
        [Parameter(Mandatory)]
        [string]$Name
    )
    $RigthDot = $Name.SubString($Name.LastIndexOf('.')).ToLower()
    Switch ($RigthDot) {
        {$RigthDot -match "^\.jpg[\?]{0,1}[0-9]*$"} { return "jpg" }
        {$RigthDot -match "^\.png[\?]{0,1}[0-9]*$"} { return "png" }
        Default { throw "The extension of $Name cannot be recognized" }
    }

}

function Get-ImageName {
    Param(
        [Parameter(Mandatory)]
        [string]$Path
    )
    $Path = $Path -replace '/', '\'
    return $Path.SubString($Path.LastIndexOf('\') + 1)
}

function Install-Wallpaper {
    [CmdletBinding()]
    Param (
        [Parameter(Mandatory)]
        [string]$Path,
        [switch]$Remote
    )
   
    $ImgFolderPath = "$PSScriptRoot/Images"
    Write-Host $ImgFolderPath
    if(-Not $(Test-Path -Path $ImgFolderPath)) {
        Write-Warning "The images folder doesn't exist and will therefore be created"
        New-Item -ItemType "directory" -Path $ImgFolderPath > $null
    }

    $RandomId = Get-Random -Minimum 1073741824
    $Name = Get-ImageName -Path $Path
    $Extension = Get-ImageExtension -Name $Name
    $DestinationPath = "$ImgFolderPath/$RandomId.$Extension"
    while(Test-Path -Path $DestinationPath) {
        $RandomId = Get-Random -Minimum 1073741824
        $DestinationPath = "$ImgFolderPath/$RandomId.$Extension"
    }

    if($Remote) {
        Invoke-WebRequest -Uri $Path -OutFile $DestinationPath
    } else {
        Copy-item -Path $Path -Destination $DestinationPath
    }

}

$WallpaperUrl = Get-RandomWallpaperUrl
Install-Wallpaper $WallpaperUrl -Remote