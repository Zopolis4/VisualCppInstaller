Windows 10 Headers and Libraries
================================

Fetch the SDK installer at https://go.microsoft.com/fwlink/p/?linkid=845298 and use it to download headers and libraries for C++ desktop developments (about 300 MB).

Open bash terminal (e.g. Git bash), `cd` to the `Installers` directory and do

    # Replace USERNAME by your actual user name
    export SDK=C:\Users\USERNAME\Downloads\SDK
    for f in *.msi ; do echo "msiexec /a \"$f\" TARGETDIR=$SDK" ; done >a.bat

Open Windows command prompt and execute

    @echo off
    a.bat

The headers and libraries should be available in `C:\Users\USERNAME\Downloads\SDK`.

MSVC Headers and Libraries (Without Admin)
==========================================

Fetch the file `visualstudio.15.release.chman` at https://aka.ms/vs/15/release and look for URL to `visualstudio.vsman` in that file e.g. http://download.microsoft.com/download/8/6/3/86383155-3307-43F2-B7C9-F4F9EC213623/visualstudio.vsman
Download the file `visualstudio.vsman`

Compile our program `vcinstaller.cpp` for example with our [DevMax]() and [C++ Compiler]() or if you trust us, fetch our binary releases
Put the program in the same folder containing `visualstudio.vsman` and run it to produce the two scripts
 * The shell script `vc_download.sh` to download the packages: Download [wget](http://gnuwin32.sourceforge.net/packages/wget.htm) and run this script.
 * <del>The batch script `vc_install.bat` to install the packages: After download script completes, open Windows command prompt, do `set VCINSTALLDIR=C:\Users\USERNAME\Downloads\MSVC` and then run this script.</del> As of latest release, they changed all MSI packages to VSIX so the script no longer works.
