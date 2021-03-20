izzy_cry_inputs = ['dump_file_prefs.cc_tmpl', 'dump_localization.cc_tmpl', 'dump_voice.cc_tmpl']
make_izzy_cry = custom_target('gen-output',
  input : izzy_cry_inputs,
  output : ['dump_file_prefs.cc', 'dump_localization.cc', 'dump_voice.cc'],
  command : ['pwsh', '../src/handlers/resolve_bid_funs.ps1', get_option('build-id')] + izzy_cry_inputs
)