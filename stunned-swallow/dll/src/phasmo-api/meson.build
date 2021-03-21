template_files = [
  'bindings.cc_tmpl',
]

make_izzy_cry = custom_target('gen-output',
  input : template_files,
  output : ['bindings.cc'],
  command : ['pwsh', '../src/phasmo-api/resolve-bid-funs.ps1', get_option('build-id')] + template_files
)