#!/usr/bin/env python

import errno

try:
  import json
except ImportError:
  import simplejson as json

import optparse
import os
import re
import shutil
import sys
import subprocess

moat_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

parser = optparse.OptionParser()

parser.add_option("--prepare",
  action="store_true",
  dest="prepare",
  help="prepare test environment")

(options, args) = parser.parse_args()

def load_config(path):
  s = open(os.path.join(path, 'config.gypi')).read()
  s = re.sub(r'#.*?\n', '', s) # strip comments
  s = re.sub(r'\'', '"', s) # convert quotes
  return json.loads(s)

def quote_against_shell_expansion(s):
  import pipes
  return pipes.quote(s)

def put_text_back_into_terminal_input_buffer(text):
  # use of this means that it only works in an interactive session
  # (and if the user types while it runs they could insert chracters between the characters in 'text'!)
  import fcntl, termios
  for c in text:
    fcntl.ioctl(1, termios.TIOCSTI, c)

def change_parent_process_directory(dest):
  # the horror
  put_text_back_into_terminal_input_buffer("cd "+quote_against_shell_expansion(dest)+"\n")

def copy_file(src, dst):
  if os.path.exists(dst):
    os.remove(dst)
  print 'copy %s to %s' %(src, dst)
  shutil.copy2(src, dst)

def exit():
  sys.exit()

def error_exit(msg):
  if msg:
    print 'Install error: ' + msg
  exit()

def write(filename, data):
  print "creating ", filename
  f = open(filename, 'w+')
  f.write(data)

def prepare_test_env(app_id, pkg_id, test_dir, out_dir):
  copy_file(os.path.join(out_dir, 'lib.target', pkg_id + '.so'), os.path.join(test_dir, pkg_id + '.so'))
  copy_file(os.path.join(moat_root, 'package', 'package.json'), os.path.join(test_dir, 'package.json'))
  write(os.path.join(test_dir, '.app-id'), app_id)

def run(args):
  config = load_config(moat_root)
  variables = config['variables']
  target_defaults = config['target_defaults']
  if not variables or not target_defaults:
    error_exit('bad config')
  target_arch = variables.get('target_arch')
  if not target_arch:
    error_exit('config: no target arch')
  host_arch = variables.get('host_arch')
  if not host_arch:
    error_exit('config: no host arch')
  if host_arch != target_arch:
    print 'WARNING: Architecture is not match. host=%s, target=%s.' %(host_arch, target_arch)
  type = target_defaults.get('default_configuration')
  if not type:
    error_exit('not default_configuration')
  app_id = variables.get('application_id')
  pkg_id = variables.get('package_name')
  if not app_id:
    error_exit('config: no application id')
  if not pkg_id:
    error_exit('config: no package name')
  test_root = os.path.join(moat_root, 'test', target_arch)
  out_dir = os.path.join(moat_root, 'out', target_arch, type)
  if options.prepare:
    prepare_test_env(app_id, pkg_id, test_root, out_dir)
    print '\n'
    change_parent_process_directory(test_root)
    print '\nYou can run your application here with the following command:'
    print '$ LD_LIBRARY_PATH=./lib/ ./apprunner\n\n'

  exit()

if __name__ == '__main__':
  run(sys.argv[:])
