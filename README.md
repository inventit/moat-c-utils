Moat C Utility Libirary
===

## About this

This is a utility library for a MOAT C application works on the ServiceSync client. These functions are useful to develop a MOAT C application.

## Usage

Import the `sseutils.gypi` from your `moatapp.gyp`.

```
{
  'variables': {
    'sseutils_root': './ssegw-utils-moatc', <-- Add
  },
  'includes': [
    'common.gypi',
    'config.gypi',
    './ssegw-utils-moatc/sseutils.gypi', <-- Add
  ],
  'targets': [
    # File deliver and fetch application
    {
      'target_name': '<(package_name)',
      'sources': [
        '<@(sseutils_src)', <-- Add
        'src/<(package_name).c',
       ],
... (snip) ...
```
You can find the samples in `samples` directory.

## Change History

### Changes in `1.0.0` (September 4, 2015)

* Initial release
