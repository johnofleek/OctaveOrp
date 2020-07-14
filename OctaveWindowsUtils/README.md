## Collection of Windows utils

## blueprintDownload.ps1
This Windows 10 powershell utility downloads the requested blueprint and associated edge
 JavaScript from the Octave cloud via the cloud api.
 
 ```
 PS C:\Users\john\Documents\octave> .\blueprintDownload.ps1

help

Enter commands like the following - look on octave.sierrawireless.io for your actual values
./blueprintDownload.ps1 -user "yourOctaveUserName" -authtoken "YourOctaveMasterToken" -company "yourOctaveCompany" -blueprint "nameOftheBlueprint
 ```
 
 In subfolder .\blueprint it creates a .json root blueprint .json file   
 In folder .\blueprint\edgeactions\ it creates one .json file for each JavaScript edge action

Filenames are all of the form [id].json
 