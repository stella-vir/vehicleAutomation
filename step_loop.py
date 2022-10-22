import math

# 0 1 2 3 4 5 6 7 8 9 10
# 1 2 3 3 2 1 2 3 3 2 1
r = 1
# for i in range(11):
#     print(r)
#     # 0 1 5 6
#     if i < 2 or 4 < i < 7:
#         r += 1
#     # 3 4 8 9
#     if 2 < i < 5 or 7 < i < 10:
#         r -= 1
#     # 2 7
#     r = r

# rewire your neurons
sides = 5
protrudingSides = 5
s = sides * protrudingSides
a = 2 * math.pi / 25
# 3
pt = (protrudingSides + 1) / 2

for i in range(1, s):
    for j in range(protrudingSides * 2):

        # 0 1 5 6
        #     if i < 2 or 4 < i < 7:
        if j < pt - 1 or protrudingSides - 1 < j < protrudingSides - 1 + pt:
            r += 1
            v = math.acos(a * i) * r
            print('+ ', i , j, v, r)
        # 3 4 8 9
        #     if 2 < i < 5 or 7 < i < 10:
        if pt - 1 < j < protrudingSides or protrudingSides - 1 + pt < j < protrudingSides * 2 - 1:
            r -= 1
            v = math.acos(a * i) * r
            print('- ', i, j, v, r)
        # 2 7
        r = r
        v = math.acos(a * i) * r
        print('= ', i, j, v, r)






# end
