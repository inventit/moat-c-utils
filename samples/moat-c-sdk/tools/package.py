#!/usr/bin/env python

import errno

try:
  import json
except ImportError:
  import simplejson as json

import os
import re
import shutil
import sys
import subprocess
import zipfile
import moatutils

moat_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
moat_path = moatutils.pathinfo(moat_root)
package_name = None
target_defaults = None
variables = None

def abspath(*args):
  path = os.path.join(*args)
  return os.path.abspath(path)

def load_config(moat_root):
  s = open(os.path.join(moat_root, 'config.gypi')).read()
  s = re.sub(r'#.*?\n', '', s) # strip comments
  s = re.sub(r'\'', '"', s) # convert quotes
  return json.loads(s)

def isfile(path):
  if not os.path.isfile(path):
    return False
  return True

def tidy_package(path):
  try:
    shutil.rmtree(path)
  except OSError, e:
    if e.errno != errno.ENOENT: raise

def exit():
  tidy_package(moat_path.PACKAGE_PATH)
  sys.exit()

def error_exit(msg):
  if msg:
    print 'MOAT Packaging error: ' + msg
  exit()

def copy_resources(res_path, pkg_path, token_src, token_dst):
  if not isfile(token_src):
    error_exit('\n  %s could not found' %(token_src))
  path = os.path.join(res_path, 'package.json')
  if not isfile(path):
    error_exit('\n  %s could not found' %(path))

  os.path
  tidy_package(pkg_path)
  shutil.copytree(res_path, pkg_path)
  shutil.copy2(token_src, token_dst)
  empty = os.path.join(pkg_path, '.empty')
  if isfile(empty):
    os.unlink(empty)

def copy_library(src_dir, app_name, pkg_dir):
  src_path = os.path.join(src_dir, app_name + '.so')
  if not isfile(src_path):
    error_exit('\n  %s could not found' %(src_path))
  print 'copying a file [%s] into [%s]...' %(src_path, pkg_dir)
  shutil.copy2(src_path, pkg_dir)

def verify_token(token, cert_path):

  if not token.verify_and_extract(cert_path):
    error_exit('\n  verifing token has failed')
  package_id = token.get_content('packageId')
  if not package_id:
    error_exit('\n  packageId could not found')
  if package_id != package_name:
    error_exit('\n  package name is not matched : token=[%s], build=[%s]' %(package_id, package_name))

def sign_token(token, cert_path, key_path, out_path):
  if not token.sign(cert_path, key_path, out_path):
    error_exit('\n  failed to sign the token')

def run(args):
  global moat_path, package_name, target_defaults, variables
  try:
    p = subprocess.check_call(['jarsigner', '-help'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  except OSError, e:
    error_exit('\n  jarsigner command cound not found.\n  Please install JDK.')
  except subprocess.CalledProcessError:
    pass

  conf = load_config(moat_path.MOAT_ROOT)
  variables = conf['variables']
  target_defaults = conf['target_defaults']
  package_name = variables.get('package_name')
  token_path = variables.get('token_path')
  arch = variables.get('target_arch')

  print 'creating a package of ' + package_name + '...'
  library_path = os.path.join(moat_root, 'out', arch, target_defaults.get('default_configuration'), 'lib.target')
  tmp_token_path = os.path.join(moat_path.PACKAGE_PATH, 'token.org')
  token = moatutils.token(tmp_token_path)
  keystore = moatutils.keystore(moat_path.CERTS_PATH, package_name)
  package = moatutils.package(moat_path.MOAT_ROOT, package_name)
  meta_path = os.path.join(moat_path.RESOURCES_PATH, 'package.json')
  if not os.path.isfile(meta_path):
    error_exit('\n  package.json cound not found.\n')
  try:
    package.load_metafile(meta_path)
  except moatutils.MetaFormatError, e:
    error_exit('\n  package.json Format Error\n' + e.err)
  copy_resources(moat_path.RESOURCES_PATH, moat_path.PACKAGE_PATH, token_path, tmp_token_path)
  copy_library(library_path, package_name, moat_path.PACKAGE_PATH)
  verify_token(token, os.path.join(moat_path.CERTS_PATH, 'moat.pem'))
  sign_token(token, keystore.pem_path, keystore.key_path, os.path.join(moat_path.PACKAGE_PATH, 'token.bin'))
  try:
    os.unlink(tmp_token_path)
  except OSError, e:
    if e.errno != errno.ENOENT: raise
  if not package.archive(moat_path.PACKAGE_PATH, arch):
    error_exit('failed to archive')
  if not package.sign(keystore.path):
    error_exit('failed to sign')
  exit()

if __name__ == '__main__':
  run(sys.argv[:])
