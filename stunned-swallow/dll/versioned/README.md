# Version Specific Files #

This directory contains versioned specific files for Phasmophobia. These are
almost always the specific types, and IL2CPP mappings we use for our projects.
The build format is as follows:

- `<buildType><build id>`
  - The build type is one of the following:
    - `s` for a build that originally was pushed to stable
    - `b` for a build that originally was pushed to beta
    - `d` for a build that originally was pushed to developer (I don't have access to these so they'll probably never exist)
    - If a build is pushed to multiple branches at once, the one that comes first in this list where the build is will be used.
  - The build id is the actual build id for steam (this is also what dkinghter posts in all of his update posts).

These files are generated out of band, and the exact process in which they're
generated is not documented. If you need to generate mappings for a new build,
please wait for me to do it first. You can always file an issue.

## Specific Files ##

All builds contain the following files:

  - `metadata.json.brotli`: a brotli compressed version of IL2CPP Metadata. They have been compressed so GitHub doesn't yell at me.
  - `win/appdata/` && `win/framework/`: bindings generated for interacting with Phasmophobia's IL2CPP codebase when we are injected in.