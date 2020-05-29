import utility
import Coding
import os.path as path

# begin
TABLE_HASH = 'd55fcddd97ee1f9836bc020d4a7c9af4a4bccc68'
# end

def gen_table(target):
    if(utility.is_valid_table(filename=target, file_hash=TABLE_HASH)):
        print('[codetb gen] gb2312 mapping file unchange, skiped! hash: %s' %
              utility.file_sha1(target))
        return
    with open(target, 'w', encoding='utf-8') as f:
        f.write(
            '# this file is auto generated\n# do not modify if you arn\'t know what you are doing!\n')
        f.write('# 这是自动生成的文件\n# 如果你不知道你在干什么，请不要修改这个文件!\n')
        f.write('# source, target\n')
        for i in range(65536):
            code = i.to_bytes(byteorder='big', length=2)

            try:
                ch = code.decode(encoding='gb2312')   # 按GB2312解码
                if len(ch) == 1:
                    code_unicode = ch.encode('unicode-escape')
                    f.write('0x%04x, ' % i)
                    f.write('0x%s ' % str(code_unicode)[5:-1].zfill(4))
                    f.write('# %s\n' % ch)
            except:
                pass
    utility.undate_hash(__file__, utility.file_sha1(target))
    print('[codetb gen] susseful generated gb2312 to unicode mapping file, hash: %s' % utility.file_sha1(target))


if __name__ == "__main__":
    gen_table('gb2312.codetab')
    pass
