# Stunned Swallow #

Stunned Swallow is a small UI wrapper around an injected DLL that performs
things that are just easier with the game dynamically executing. The core thing
this is able to do is extract the voice lines/localization texts from the game.

## Building Notes ##

Stunned Swallow currently only supports Windows, as supporting anything more
than that takes honestly just too much effort that I'm willing to put in at
this point in time. If you'd like to contribute other OS's compatability I'd
be more than happy to accept your work.

Stunned Swallow is using [meson](https://mesonbuild.com/) as the build system
for all native code used. The C/C++ building process, especially on Windows is
far from intuitive, and I will always be incredibly frustrated by every part of
it. A couple tips for newcomers:

  - Install the latest visual studio (community edition), install all C++ stuff
    and check the updater commonly. I reguarly see it way out of date with no
    tools telling me besides their UI for actual Visual Studio which I don't
    use, and is incredibly cluttered anyway.

  - Ensure if you're on a 64 bit machine (search for "This PC", right click,
    select properties, System Type) that you are using a 64 bit developer
    command line. You can do this by launching the "native tools command
    prompt x64" if you're comfortable with CMD, or if you are like me using
    powershell you need to import some obscure module, and run a command to
    get it all setup (I've got this in a [powershell profile][psp] so i can't
    forget). My powershell profile looks like the following:

    ```powershell
    Import-Module C:\VS2019-Base-Install-Directory\Common7\Tools\Microsoft.VisualStudio.DevShell.dll
    Enter-VsDevShell -VsInstallPath "C:\VS2019-Base-Install-Directory" -SkipAutomaticLocation -DevCmdArguments '-arch=x64 -host_arch=x64 -no_logo'
    ```

  - Don't be afraid to nuke any builddirectories.

Scripts for DLL:

```powershell
# Building
.\.github\scripts\win\ss\build-dll.ps1
# Formatting, if you have clang-format installed
.\.github\scripts\win\ss\format-dll.ps1
```

## Architecture Notes ##

Stunned Swallow uses a [Named Pipe][np], something roughly equivelant to a unix
socket (aka a network transfer over a local piece of memory). This way the DLL
once injected can listen to requests from the existing user interface that
injected it, without having to create it's own sort of UI that's within the DLL
itself.

It only expects one person to connect to the named socket at a time, and
expects one command to be sent, and it will send something back. It will always
send something back. If there's no response it wants to send back it will send
back a string simply saying: `"no-op"`.

## Commands ##

The following commands are implemented inside of Stunned Swallow, which you can
call.

### `dump_localization` ###

***NOTE: localization vs localisation is an intentional difference.***

This command is the first command implemented in Stunned Swallow, which can
be thought of as the "core" command. It returns the `localisation.csv` along
with special phrases in JSON.

The result is a JSON blob that looks like the following when successful:

  ```json
  {
    "localisation.csv": "localization.csv content"
  }
  ```

The result is the JSON shape when there's an error:

  ```json
  {
    "error": "a basic error message in english"
  }
  ```

### `dump_voice_words` ###

This command implements dumping just the "important" voice phrases currently
configured with the users voice language.

The result is a JSON blob that looks like the following when successful:

  ```json
  {
    "phrases": ["list", "of", "base listening", "phrases"],
    "ouija": ["list", "of", "ouija phrases", "that", "listen"],
    "spirit_box": ["list", "of", "spirit box q's", "that", "listen"],
    "no_no_words": ["list", "of", "no", "no", "words"]
  }
  ```

The result is the JSON shape when there's an error:

  ```json
  {
    "error": "a basic error message in english"
  }
  ```

[psp]: https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_profiles?view=powershell-7
[np]: https://docs.microsoft.com/en-us/windows/win32/ipc/named-pipes