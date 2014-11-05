# HTCIA Training: July 15, 2014
#
# credit must be given to Linux Sleuthing (other credits listed in slides):
# http://linuxsleuthing.blogspot.com/2013/09/recovering-data-from-deleted-sqlite.html

from struct import unpack

with open('Chinook_Sqlite_deleted_lots.sqlite', 'rb') as f:
    data = f.read()

pageSize = unpack('>h', data[16:18])[0] # reads 2-byte page size beginning at byte offset 16

pageList = []

# find all table b-tree leaf pages
for offset in range(0, len(data), pageSize):
    if data[offset] == 13: # table b-tree leaf pages' first byte is \x0D (hex), i.e. 13 (decimal)
        pageList.append(offset)

print(pageList)