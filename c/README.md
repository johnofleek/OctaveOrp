# Octave remote C code project

This is a work in progress - at the moment the API is being defined.  

For info on the API development [click](../docs/remoteC_api/cpp_orp_microApi.md) 

# Current status
API test orp_api_test.c builds and runs  
The API does nothing at the moment 


# Visual Studio Code

Configuration  
https://code.visualstudio.com/docs/cpp/config-mingw

## markdown viewer
CTRl SHIFT V

## make a symlink in powershell
From 
https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.management/new-item?view=powershell-6


Used this to symlink core c code into arduino or other target project

```
New-Item -ItemType SymbolicLink -Path "Link" -Target "Target"
```
Replace the "Link" portion with the path to the symbolic link you want 
 to create (including the file name and its extension for files)

Replace the "Target" portion with the path (relative or absolute) that
 the new link refers to.

```
New-Item -ItemType SymbolicLink -Path "Link" -Target .
```

Source:  
C:\Users\john\Documents\Arduino\octaveArduinoGH2\c  

Destination:  
C:\Users\john\Documents\Arduino\libraries  



```
cmd /c mklink
Creates a symbolic link.

MKLINK [[/D] | [/H] | [/J]] Link Target

        /D      Creates a directory symbolic link.  Default is a file
                symbolic link.
        /H      Creates a hard link instead of a symbolic link.
        /J      Creates a Directory Junction.
        Link    Specifies the new symbolic link name.
        Target  Specifies the path (relative or absolute) that the new link
                refers to.
```

In my case
```
cmd /c mklink /d .\c C:\Users\john\Documents\Arduino\octaveArduinoGH2\c
```