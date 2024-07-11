blueprints = custom_target('blueprints',
  input: files(
      'context.blp',
      'content.blp',
      'sidebar.blp',
      'window.blp'
  ),
  output: '.',
  command: [find_program('blueprint-compiler'), 'batch-compile', '@OUTPUT@', '@CURRENT_SOURCE_DIR@', '@INPUT@'],
)
