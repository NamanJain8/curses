import re

regex_header = '^atf_test_case .*$'
tests = []
with open("../headers.sh", "r+") as fp:
    for line in fp.readlines():
        if re.match(regex_header, line):
            tests.append(line.split(' ')[-1][:-1])

line = "atf_add_test_case %s"

test_lines = []
for t in tests:
    test_lines.append(line%(t))

with open("lines.sh", "w+") as fp:
    for line in test_lines:
        fp.write(line+"\n")