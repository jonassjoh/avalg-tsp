n1 = input()
n2 = input()
count = 0
with open(n1) as f1:
    with open(n2) as f2:
        for line in f1:
            line2 = f2.readline()
            if not line2 == line:
                print(count, line, line2)
            f2.next()
            count += 1
