from os import walk
import re

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def Diff(li1, li2): 
    return (list(set(li1) - set(li2)))

def Intersection(lst1, lst2): 
    return list(set(lst1) & set(lst2))

# find files which are already there in atf
print(bcolors.HEADER + "========== EXISTING ==========" + bcolors.ENDC)

existing_headers = []
existing_lines = []
regex_header = '^atf_test_case .*$'
regex_line = '^.*atf_add_test_case .*$'
with open("../t_curses.sh", "r") as fp:
    for line in fp.readlines():
        if re.match(regex_header, line):
            existing_headers.append(line.split(' ')[-1][:-1])
        if re.match(regex_line, line):
            existing_lines.append(line.split(' ')[-1][:-1])

print("Existing header: %d"%(len(existing_headers)))
print("Existing lines: %d"%(len(existing_lines)))

d1 = Diff(existing_headers, existing_lines)
d2 = Diff(existing_lines, existing_headers)
intersect = Intersection(existing_headers, existing_lines)

if len(d1) or len(d1):
    print("With Header only:", d1)
    print("With lines only:", d2)
    print(bcolors.WARNING + "Buggy t_curses file" + bcolors.ENDC)
    print("%d files are in sane state"%(len(intersect)))
else:
    print("%d files are in sane state"%(len(intersect)))
    print(bcolors.OKGREEN + "Sane t_curses file" + bcolors.ENDC)

print(bcolors.HEADER + "=============================" + bcolors.ENDC)


# find all the existing test files

files = []
for (_, _, fname) in walk("../tests"):
    files.extend(fname)
    break
print("Total %d files"%(len(files)))

tests = []
with open("../headers.sh", "r+") as fp:
    for line in fp.readlines():
        if re.match(regex_header, line):
            tests.append(line.split(' ')[-1][:-1])

print(Diff(files, tests))

# files = Diff(files, intersect)

# test_headers = []
# test_lines = []

# header = "atf_test_case %s\n%s_head()\n{\n    atf_set \"descr\" \"\"\n}\n%s_body()\n{\n    h_run %s\n}"
# line = "atf_add_test_case %s"

# print(bcolors.OKBLUE + header%(files[-1], files[-1], files[-1], files[-1]) + bcolors.ENDC)
# print(bcolors.OKBLUE + line%(files[3]) + bcolors.ENDC)

# for f in files:
#     test_headers.append(header%(f,f,f,f))
#     test_lines.append(line%(f))

# # print to file

# with open("headers", "w+") as fp:
#     for header in test_headers:
#         fp.write(header+"\n"*2)
# with open("lines", "w+") as fp:
#     for line in test_lines:
#         fp.write(line+"\n")
