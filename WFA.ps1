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
$ImgFolderPath = "$PSScriptRoot\Images"
$LogPath = "$PSScriptRoot\displayedImages.log"
$TmpFolderPath = "$PSScriptRoot\Temp"

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

    if(-Not $(Test-Path -Path $TmpFolderPath)) {
        Write-Warning "The temp folder doesn't exist and will therefore be created"
        New-Item -ItemType "directory" -Path $TmpFolderPath > $null
    }
}

function Get-FormatIsh {
    Param (
        [Parameter(Mandatory)]
        [int]$width,
        [Parameter(Mandatory)]
        [int]$height,
        [int]$epsilon = 0
    )

    $Wider = $height -lt ($width - $epsilon)
    $Higher = $height -gt ($width + $epsilon)

    if((-Not $Wider) -and (-Not $Higher)) {
        return 'Square'
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
    Write-Log "IMAGE URL: $($JsonResp.assets[0].image_url)"
    Write-Log "ORIGINAL SIZE: $($JsonResp.assets[0].Width)x$($JsonResp.assets[0].Height)"
    $ImageUrl = $JsonResp.assets[0].image_url
    return @{Url=$ImageUrl; Width=$JsonResp.assets[0].Width; Height=$JsonResp.assets[0].Height}
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
        {$RigthDot -match "^\.gif[\?]{0,1}[0-9]*$"} { return "gif" }
        Default { throw "The extension of $Name cannot be recognized" }
    }

}

function Get-ImageFullname {
    Param(
        [Parameter(Mandatory)]
        [string]$Path
    )
    $Path = $Path -replace '/', '\'
    return $Path.SubString($Path.LastIndexOf('\') + 1)
}

function Get-ImageName {
    Param(
        [Parameter(Mandatory)]
        [string]$Path
    )
    $Fullname = $Path.SubString($Path.LastIndexOf('\')+1)
    return $Fullname.SubString(0, $Fullname.LastIndexOf('.'))
}

function Get-MonitorsSize {
    Add-Type -AssemblyName System.Windows.Forms
    $Monitors = [System.Windows.Forms.Screen]::AllScreens
    $Monitor = $Monitors[0]
    # Temporary dealing with only one monitor size
    return @{Width=$Monitor.Bounds.width; Height=$Monitor.Bounds.height}
}

function Remove-TemporaryFiles {
    Remove-Item "$TmpFolderPath\*"
}

function Fit-ImageToScreen{
    Param(
        [Parameter(Mandatory)]
        [string]$ImgPath,
        [Parameter(Mandatory)]
        [int]$ImgWidth,
        [Parameter(Mandatory)]
        [int]$ImgHeight
    )

    # Temporary dealing with only one monitor size
    $MonitorSize = Get-MonitorsSize
    $Width = $MonitorSize['Width']
    $Height = $MonitorSize['Height']
    
    $ImgName = Get-ImageName $ImgPath
    $bluredPath = "$TmpFolderPath\tmp_blured_$ImgName.jpg"
    $cropedpath = "$TmpFolderPath\tmp_composited_$ImgName.jpg"
    $toobigPath = "$TmpFolderPath\tmp_toobig_$ImgName.jpg"
    $destinationPath = "$ImgFolderPath\$ImgName.jpg"

    $MonitorFormat = Get-FormatIsh $Width $Height
    [float]$MonitorRatio = 0.0
    [float]$ImageRatio = 0.0

    $OriginalResized = $false
    if($MonitorFormat -eq 'Portrait') {
        $MonitorRatio = $Height / $Width
        $ImageRatio = $ImgHeight / $ImgWidth
        if($ImageRatio -gt $MonitorRatio) {
            if($ImgHeight -gt $Height) {
                magick.exe $ImgPath -resize "x$Height" -compress lossless $toobigPath
                $OriginalResized = $true
            }
            $Sizing = "$Width"
        } else {
            if($ImgWidth -gt $Width) {
                magick.exe $ImgPath -resize "$Width" -compress lossless $toobigPath
                $OriginalResized = $true
            }
            $Sizing = "x$Height"
        }
    } else { # landscape and square
        $MonitorRatio = $Width / $Height
        $ImageRatio = $ImgWidth / $ImgHeight
        if($ImageRatio -gt $MonitorRatio) {
            if($ImgWidth -gt $Width) {
                magick.exe $ImgPath -resize "$Width" -compress lossless $toobigPath
                $OriginalResized = $true
            }
            $Sizing = "x$Height"
        } else {
            if($ImgHeight -gt $Height) {
                magick.exe $ImgPath -resize "x$Height" -compress lossless $toobigPath
                $OriginalResized = $true
            }
            $Sizing = "$Width"
        }
    }
    
    if($OriginalResized) {
        magick.exe $toobigPath -resize $Sizing -blur 0x20 -compress lossless $bluredPath
        magick.exe $bluredPath -gravity center -crop "${Width}x$Height+0+0" -compress lossless $cropedpath
        magick.exe composite -gravity center $toobigPath $cropedpath -compress lossless $destinationPath
    } else {
        magick.exe $ImgPath -resize $Sizing -blur 0x20 -compress lossless $bluredPath
        magick.exe $bluredPath -gravity center -crop "${Width}x$Height+0+0" -compress lossless $cropedpath
        magick.exe composite -gravity center $ImgPath $cropedpath -compress lossless $destinationPath
    }

}


function Install-Wallpaper {
    [CmdletBinding()]
    Param (
        [Parameter(Mandatory)]
        [hashtable]$Wallpaper,
        [switch]$Remote
    )

    $Path = $Wallpaper['Url']
    $RandomId = Get-Random -Minimum 1073741824
    $Name = Get-ImageFullname -Path $Path
    $Extension = Get-ImageExtension -Name $Name
    Write-Log("NEW NAME: $RandomId.$Extension")
    $DestinationPath = "$TmpFolderPath\$RandomId.$Extension"
    while(Test-Path -Path $DestinationPath) {
        $RandomId = Get-Random -Minimum 1073741824
        $DestinationPath = "$TmpFolderPath\$RandomId.$Extension"
    }

    if($Remote) {
        Invoke-WebRequest -Uri $Path -OutFile $DestinationPath
    } else {
        Copy-item -Path $Path -Destination $DestinationPath
    }
    $MonitorSize = Get-MonitorsSize
    
    $ImgWidth = $Wallpaper['Width']
    $ImgHeight = $Wallpaper['Height']
    $MonitorWidth = $MonitorSize['Width']
    $MonitorHeight = $MonitorSize['Height']

    if(-Not ($ImgWidth -eq $MonitorWidth) -Or -Not ($ImgHeight -eq $MonitorHeight)) {
        Fit-ImageToScreen $DestinationPath $ImgWidth $ImgHeight
    }

}

function Clean-ImageDirectory {
    [CmdletBinding()]
    Param()
    Remove-Item "$ImgFolderPath\*"

}

# Main function
Clean-ImageDirectory
for($i = 0; $i -lt $NumberOfImages; $i++) {
    $Wallpaper = Get-RandomWallpaperUrl
    Install-Wallpaper $Wallpaper -Remote
    Write-Log(" ")
}
Remove-TemporaryFiles