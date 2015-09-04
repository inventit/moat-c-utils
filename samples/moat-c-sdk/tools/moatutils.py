#!/usr/bin/env python

import os
import pprint
import collections
import subprocess
import locale
import zipfile
import hashlib

try:
  import json
except ImportError:
  import simplejson as json

def package_hash(package_name):
  m = hashlib.md5(package_name)
  return m.hexdigest()

class pathinfo:

  def __init__(self, moat_root):
    self.MOAT_ROOT = moat_root
    self.PACKAGE_PATH = os.path.join(moat_root, '.package')
    self.RESOURCES_PATH = os.path.join(moat_root, 'package')
    self.CERTS_PATH = os.path.join(moat_root, 'certs')
    self.SOURCE_PATH = os.path.join(moat_root, 'src')
    self.TOKEN_NAME = 'token.bin'

class token:

  def __init__(self, token_path):
    self.path = os.path.abspath(token_path)
    self.json = None
    self.package_name = None

  def verify_and_extract(self, cert_path):
    if not cert_path:
      return False
    if not os.path.isfile(self.path):
      return False

    tmp = os.path.abspath('./.token.json')
    print 'verifying the token'
    try:
      p = subprocess.check_call(['openssl', 'smime', '-verify',
        '-in', self.path, '-CAfile', cert_path, '-out', tmp, '-inform', 'der'],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError:
      return False

    try:
      s = open(tmp).read()
      self.json = json.loads(s)
      os.unlink(tmp)
    except ValueError:
      print 'decoding JSON has failed'
      return False
 
#    print self.json.get('applicationId')
#    print self.json.get('packageId')
    if not self.json.get('applicationId') or not self.json.get('packageId'):
      return False
    self.package_name = str(self.json.get('packageId'))
    return True

  def get_content(self, key):
    if not self.json:
      return None
    return self.json.get(key)

  def sign(self, cert_path, key_path, out_path):
    if not cert_path or not key_path or not out_path:
      return False
    if not self.package_name:
      return False
    print 'signing %s' %(out_path)
    try:
      p = subprocess.check_call(['openssl', 'smime', '-sign',
        '-in', self.path, '-out', out_path, '-signer', cert_path,
        '-inkey', key_path, '-passin', 'pass:' + package_hash(self.package_name),
        '-nodetach', '-outform', 'der', '-binary'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError:
      return False
    return True


class keystore:

  def __init__(self, certs_path, package_name):
    self.certs_path = certs_path
    self.package_name = package_name
    self.path = os.path.join(certs_path, package_name + '.keystore')
    self.p12_path = os.path.join(certs_path, package_name + '.p12')
    self.key_path = os.path.join(certs_path, package_name + '.key')
    self.pem_path = os.path.join(certs_path, package_name + '.pem')

  def has(self):
    if os.path.isfile(self.path) and os.path.isfile(self.p12_path) and os.path.isfile(self.key_path) and os.path.isfile(self.pem_path):
      return True
    return False

  def clear(self):
    if os.path.isfile(self.path):
      os.unlink(self.path)
    if os.path.isfile(self.p12_path):
      os.unlink(self.p12_path)
    if os.path.isfile(self.key_path):
      os.unlink(self.key_path)
    if os.path.isfile(self.pem_path):
      os.unlink(self.pem_path)

  def generate(self):
    if os.path.isfile(self.path):
      return True

    (lang, enc) = locale.getdefaultlocale()
    lang = str(lang)
    if str(lang).find('_') >= 0:
      c = str(lang).split('_')[1]
    else:
      c = 'US'
    dname = 'CN=MOAT Application, O=' + self.package_name + ', C=' + c
    print 'generating a keystore'
    try:
      p = subprocess.check_call(['keytool', '-genkey',
        '-keystore', self.path, '-storepass', package_hash(self.package_name),
        '-alias', self.package_name, '-keypass', package_hash(self.package_name),
        '-dname', dname, '-validity', '18250'],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError:
      return False
    return True

  def export_pkcs12(self):
    
    if not os.path.isfile(self.path):
      return False
    if os.path.isfile(self.p12_path):
      return True
    print 'generating a pkcs#12 certificate'
    try:
      p = subprocess.check_call(['keytool', '-importkeystore',
        '-srckeystore', self.path, '-destkeystore', self.p12_path, '-deststoretype', 'PKCS12',
        '-srcstorepass', package_hash(self.package_name),
        '-deststorepass', package_hash(self.package_name)],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError:
      return False
    return True

  def export_key(self):
    if not os.path.isfile(self.p12_path):
      return False
    if os.path.isfile(self.key_path):
      return True
    print 'exporting a key'
    try:
      p = subprocess.check_call(['openssl', 'pkcs12', '-in', self.p12_path, '-nocerts',
        '-passin', 'pass:' + package_hash(self.package_name),
        '-passout', 'pass:' + package_hash(self.package_name),
        '-out', self.key_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError:
      return False
    return True

  def export_pem(self):
    if not os.path.isfile(self.p12_path):
      return False
    if os.path.isfile(self.pem_path):
      return True
    print 'exporting a public certificate'
    try:
      p = subprocess.check_call(['openssl', 'pkcs12', '-in', self.p12_path, '-nokeys',
        '-nodes', '-passin', 'pass:' + package_hash(self.package_name),
        '-out', self.pem_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError:
      return False
    return True

class MetaFormatError(Exception):
  def __init__(self, err):
    self.err = err
  def __str__(self):
    return repr(self.err)

class package:
  def __init__(self, root, package_name, version=None):
    self.deploy_to = root
    self.package_name = package_name
    self.version = version
    self.path = None

  def gen_metafile(self, path):
    meta_path = os.path.join(path, 'package.json')
    has_od = True
    try:
      meta = collections.OrderedDict()
    except AttributeError:
      has_od = False
    if has_od:
      meta['name'] = self.package_name
      meta['version'] = self.version
      meta['description'] = '[description]'
      maintainers = collections.OrderedDict({ 'name' : '[name]', 'email' : '[email]'})
      meta['maintainers'] = [maintainers]
      meta['bugs'] = collections.OrderedDict({ 'mail' : '[email]'})
      meta['dependencies'] = collections.OrderedDict({ 'moat' : '>=1.0.0 <2.0.0'})
      meta['main'] = self.package_name + '.so'
      meta['models'] = collections.OrderedDict({})
      data = json.dumps(meta, sort_keys=False, indent=4, separators=(',', ' : '))
    else:
      data = '''{
\t"name" : "%s",
\t"version" : "%s",
\t"description" : "[description]",
\t"maintainers" : [
\t\t{ "name" : "[name]", "email" : "[email]" }
\t],
\t"bugs" : {
\t\t"mail" : "[email]"
\t},
\t"dependencies" : {
\t\t"moat" : ">=1.0.0 <2.0.0"
\t},
\t"main" : "%s.so",
\t"models" : {}
}''' %(self.package_name, self.version, self.package_name)
    f = open(meta_path, 'w')
    f.write(data + '\n')
    return True

  def load_metafile(self, path):
    f = open(path, 'r')
    err = None
    try:
      data = json.loads(f.read())
      name = None
      try:
        name = data["name"]
        self.version = data["version"]
        models = data["models"]
      except KeyError, e:
        err = '"%s" is missing' %(e)
        raise MetaFormatError(err)
      if not err:
        if len(models) == 0:
          err = 'need to define at least one model'
          raise MetaFormatError(err)
        if name != self.package_name:
          err = 'name is not match'
          raise MetaFormatError(err)
    except ValueError, e:
      err = 'ValueError:%s' %(e)
      raise MetaFormatError(err)
    finally:
      f.close()
    return True

  def archive(self, path, arch):
    pkg_path = os.path.join(self.deploy_to, self.package_name + '_' + self.version + '_' + arch + '.zip')
    print 'archinving to %s' %(pkg_path)
    cwd = os.getcwd()
    os.chdir(path)
    zip = zipfile.PyZipFile(pkg_path, mode='w')
    try:
      for root, dirs, files in os.walk(path):
        for f in files:
          relpath = os.path.relpath(os.path.join(root, f), path)
          zip.write(relpath)
    finally:
      zip.close()
    os.chdir(cwd)
    self.path = pkg_path
    return True

  def sign(self, store_path):
    if not os.path.isfile(self.path):
      return False
    print 'signing %s' %(self.path)
    try:
      p = subprocess.check_call(['jarsigner', '-keystore', store_path,
        '-storepass', package_hash(self.package_name), self.path, self.package_name],
      stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except subprocess.CalledProcessError:
      return False
    return True
