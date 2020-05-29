import utility
import importlib
import os.path as path
import os

root = path.dirname(__file__)
need_update = False
force_generate = False

# beg
code_tab_targets = [
    'gb2312'
]
# end

if(not path.exists(path.join(root, 'Generated'))):
    os.makedirs(path.join(root, 'Generated'))

for target in code_tab_targets:
    gen = importlib.import_module(target)
    gen.gen_table(path.join(root, 'Generated', target + '.codetab'))
