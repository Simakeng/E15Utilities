import os.path as path
from hashlib import sha1


def file_sha1(filename):
    with open(filename, 'rb') as f:
        return sha1(f.read()).hexdigest()


def is_valid_table(filename, file_hash=''):
    try:
        return file_hash == file_sha1(filename)
    except:
        return False


def undate_hash(this, hash):
    with open(this, 'r', encoding='utf-8') as f:
        code = f.read()
    s = code.find('# begin')
    e = code.find('# end') + len('# end')
    code = code[:s] + "# begin\nTABLE_HASH = '%s'\n# end" % hash + code[e:]
    with open(this, 'w', encoding='utf-8') as f:
        f.write(code)
