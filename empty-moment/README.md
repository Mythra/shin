# Empty Moment #

Empty Moment is a small script using `depot-downloader` in order to
keep the beta branch synced with Phamophobia, and synchronizing them to a
remote Backblaze bucket so they can be downloaded later on, and then notifying
a Discord Webhook that a new build has gone live, and been archived.

This for me actually runs on a solo account with just phasmophobia bought, and
nothing else on a server. This way I don't have to be up to catch new
Phasmophobia beta's. I just get a nice discord notification in the morning.

It also includes a small script for downloading just the `GameAssembly.dll`,
`global-metadata.dat`, and `unity default resources` from the backblaze bucket
that has been archived to. This is all the information needed in order to start
reverse engineering the games code. Specifically:

  - `global-metadata.dat` contains the obfuscated function names from IL2CPP
                          and where they are located in `GameAssembly.dll`.
  - `Game-Assembly.dll` contains the actual native code that is executed.
  - `unity default resources` allows us to determine the unity version the
                              game was built with to better aid in
                              decompilation.
