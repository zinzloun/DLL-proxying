# DLL Side-loading made simple
## Credits
This work is based on the following: https://github.com/tothi/dll-hijack-by-proxying, that I encourage you to read in case you don't know how this attack works.
## Lab setup
    FileZilla Client
      Version:          3.67.1
    Operating system:
      Name:           Windows 11 (build 22631), 64-bit edition
      Version:        10.0
      Platform:       64-bit system
## Requirements
- Python3 with pip
- Pefile
- GCC compiler. I use MSYS2 MINGW32, in case you can follow [this tutorial](https://www.linkedin.com/pulse/installing-gcc-compiler-windows-run-c-program-gitbash-david-michael) to install it.

      pip3 install pefile  

## Lab scenario
We must identify a DLL that is loaded by Filezilla.exe. There are many tools\techniques to accomplish the task, in my case I used Process Hacker:
![hacker](proc_hacker.png)


I dediced to exploit <b>C:\Program Files\FileZilla FTP Client\libnettle-8.dll</b> as .arget DLL. First I renamed the library as <b>libcrypto-2.dll</b> (you can choose the name you prefer, just keep it stealth :)). We can verify that the DLL is loaded by Filezilla client executing filezilla.exe; you will get the following error about missing libnettle-8.dll:
![err](err_fz.png)

The error confirm that we can take advantage of this DLL to perform a side-loading attack.

In this scenario we are going to perform the following actions:
- export the function contained in the original libnettle-8 DLL (now renamed as libcrypto-2) inside our malicious payload: libnettle-8.c
- compile our payload containing a reverse shell as the original payload: libnettle-8.dll

The python script takes care to create the def file containing the exported function, then return the <b>gcc</b> command to compile the payload as original DLL (libnettle-8).
<i>Note: I tried to execute gcc from the python script, using subprocess, but the resulting DLL is smaller (compared to the one eventually created issuing the command directly) 
and once executed throw an error related a missing entry point. At 
the moment I don't know what it's the problem. Any help in this sense is really appreciated</i>.

So once you have compiled our code as libnettle-8.dll, simple move it inside Filezilla Client home directory, you should have libcrypto already present in the folder, since we have previously renamed the original
libnettle-8.dll. Execute Filezilla and you should get a powershell back
