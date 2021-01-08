# Parameters
Param(
    [int]$NumberOfImages = 2
)
# Global action
$ErrorActionPreference = "Stop"

# Parameters validity check
if($NumberOfImages -le 0) {
    throw "The NumberOfImages parameter must be greater than 0"
}

# Global variable
$ImgFolderPath = "$PSScriptRoot/Images"
$LogPath = "$PSScriptRoot/displayedImages.log"

# Function
function Write-Log {
    Param(
        [Parameter(Mandatory)]
        [string]$Message
    )
    Add-Content $LogPath -Value "[ $(Get-Date -Format "yyyy/MM/dd HH:mm") ] $Message"
}

function Configure-Folder {
    if(-Not $(Test-Path -Path $ImgFolderPath)) {
        Write-Warning "The images folder doesn't exist and will therefore be created"
        New-Item -ItemType "directory" -Path $ImgFolderPath > $null
    }

    if(-Not $(Test-Path -Path $LogPath)) {
        Write-Warning "The log file doesn't exist and will therefore be created"
        New-Item -ItemType "file" -Path $LogPath > $null
    }
}

function Get-ImageFormatIsh() {
    Param (
        [Parameter(Mandatory)]
        [int]$width,
        [Parameter(Mandatory)]
        [int]$height
    )

    $eps = 30
    $Wider = $height -lt ($width - $eps)
    $Higher = $height -gt ($width + $eps)

    if((-Not $Wider) -and (-Not $Higher)) {
        return 'Square'
    } elseif($Wider -and $Higher) {
        Write-Host "daaaamn"
    } elseif(-Not $Higher) {
        return 'Landscape'
    } else {
        return 'Portrait'
    }
}

function Get-RandomWallpaperUrl {
    $WebRespContent = (Invoke-WebRequest -Uri "https://artstation.com/random_project.json").Content
    $JsonResp = ConvertFrom-Json -InputObject $WebRespContent
    Write-Log "AUTHOR: $($JsonResp.user.permalink)"
    Write-Log "IMAGE URL: $($JsonResp.assets[0].image_url)`n"
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

function Clean-ImageDirectory {
    [CmdletBinding()]
    Param()
    Remove-Item "$ImgFolderPath/*"

}

# Main function
Clean-ImageDirectory
for($i = 0; $i -lt $NumberOfImages; $i++) {
    $WallpaperUrl = Get-RandomWallpaperUrl
    Install-Wallpaper $WallpaperUrl -Remote
}