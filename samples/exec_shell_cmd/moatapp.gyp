{
  'variables': {
    'sseutils_root': '../..',
  },
  'includes': [
    'common.gypi',
    'config.gypi',
    '../../sseutils.gypi',
  ],
  'targets': [
    # your M2M/IoT application
    {
      'target_name': '<(package_name)',
      'sources': [
        '<@(sseutils_src)',
        'src/<(package_name).cc',
      ],
      'product_prefix': '',
      'type': 'shared_library',
      'cflags': [ '-fPIC' ],
      'include_dirs' : [
        '<(sseutils_include)',
      ],
      'libraries': [
        '-lmoatapp',
      ],
      'dependencies': [
      ],
    },
  ],
}
