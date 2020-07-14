# powershell script
# to download Latest Blueprint and associated 

# GET the Blueprint data for all Blueprints from Octave

# ./blueprintDownload.ps1 -user "Octaveuser" -authtoken "master" -company "linkwave_technologies

param (
    $user ,
    $authtoken,
    $company, 
    $blueprint = "JohnArduinoLCDandTimer"
)

If($user -eq $null -or $authtoken -eq $null -or $company -eq $null -or $blueprint -eq $null)
{
    Write-Host "help"
    Write-Host "Enter commands like the following - look on octave.sierrawireless.io for your actual values"
    Write-Host './blueprintDownload.ps1 -user "yourOctaveUserName" -authtoken "YourOctaveMasterToken" -company "yourOctaveCompany" -blueprint "nameOftheBlueprint'
    return;
}


#$user
#$authtoken
#$company


$uribase = "https://octave-api.sierrawireless.io/v5.0/" + $company + "/"

#$uribase

$param = @{
    Uri         = $uribase + "blueprint"
    Method      = "GET"
    Headers     = @{ 'X-Auth-Token' = $authtoken
                     'X-Auth-User' = $user }
}

$response = Invoke-RestMethod @param
#$response.body | ConvertTo-Json -Depth 100


### For an individual blueprint id, create a URI then extract the blueprint data

Foreach ($body in $response.body)
{
    if ($body.displayName -eq $blueprint)
    {
        #$body | ConvertTo-Json -Depth 100 
        
        ## save to file the blueprint
        $pathblueprint = ".\blueprint\" + $body.displayName + "\"
        $filenameblueprint = $pathblueprint + $body.id  + ".json"
        #$filenameblueprint
        If(!(test-path $pathblueprint))
        {
              New-Item -ItemType Directory -Force -Path $pathblueprint
        }
        $body | ConvertTo-Json -Depth 100 | Out-File $filenameblueprint
        Write-Host "File written: " $filenameblueprint
        
#       First retrieve the Blueprint ID 
        Foreach ($localAction in $body.localActions )
        {
            # $localAction | ConvertTo-Json -Depth 100 
            # then use the edge action ids and values embedded in blueprint
            # to extract the edge action data used with this blueprint
            ForEach ($blueprint_id in $localAction.PSObject.Properties)
            {
                # $blueprint_id.Name
                $id = $blueprint_id.Name        
                #$id
                $version = $localAction.$id.version
                
                # Retrieve each EdgeAction code for the Blueprint requested
                $param.Uri = $uribase + "versions/" + "local-action/" + $id + "/" + $version
                $responseJs = Invoke-RestMethod @param
                #$responseJs.body | ConvertTo-Json -Depth 100 
                
                ## save to files the actual edge actions used by the blueprint
                $pathjson = $pathblueprint + "edgeactions\"
                $filenamejson = $pathjson + $blueprint_id.Name + ".json"
                #$filenamejson
                If(!(test-path $pathjson))
                {
                      New-Item -ItemType Directory -Force -Path $pathjson
                }
                $responseJs.body | ConvertTo-Json -Depth 100 | Out-File $filenamejson
                Write-Host "File written: " $filenamejson
            }
        }
    }
}







