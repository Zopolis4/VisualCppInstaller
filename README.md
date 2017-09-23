Windows 10 Headers and Libraries
================================

Open bash terminal, `cd` to the `Installers` directory and do

    # Replace USERNAME by your actual user name
    export SDK=C:\Users\USERNAME\Downloads\SDK
    for f in *.msi ; do echo "msiexec /a \"$f\" TARGETDIR=$SDK" ; done >a.bat

Open Windows command prompt and execute

    @echo off
    a.bat

MSVC Headers and Libraries (Without Admin)
==========================================

Fetch the file `visualstudio.15.release.chman` at https://aka.ms/vs/15/release and look for URL to `visualstudio.vsman` in that file e.g. https://download.visualstudio.microsoft.com/download/pr/11425389/a0bbc4b88f1988a4f20ee723c6ec428a/visualstudio.vsman
Download `visualstudio.vsman` and there are links to the Visual Studio installer packages, grab them with

    grep "\"https://.*\" visualstudio.vsman >a.txt

Use [wget](http://gnuwin32.sourceforge.net/packages/wget.htm) to download them all

    wget --no-check-certificate -q -i a.txt