# Empty Moment #

Empty Moment is at it's core a small script using `depot-downloader` in order to
keep the beta branch synced with Phamophobia, and synchronizing them to a
remote Backblaze bucket so they can be downloaded later on, and then notifying
a Discord Webhook that a new build has gone live, and been archived.

This for me actually runs on a solo account with just phasmophobia bought, and
nothing else on a server. This way I don't have to be up to catch new
Phasmophobia beta's. I just get a nice discord notification in the morning.

It also includes a small script for downloading just the `GameAssembly.dll`,
and `global-metadata.dat` files needed for il2cpp reversing from any build.