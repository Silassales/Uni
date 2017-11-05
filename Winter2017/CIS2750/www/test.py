blah = input()
array_string = input()
array_list = array_string.split(" ") 
count = 0
last_pos = 0 
pos = 0
#find pos where there is one odd number alone
old_i = 0
for i in array_list:
    pos += 1
    if (((int(i) % 2) == 0) and ((old_i % 2) != 0)):
        size = (pos - last_pos) - 2
        n = size - 1
        count_add = ((n + 1)(n + 2) / 2)
        count = count + count_add
        last_pos = pos
    old_i = int(i)
print(count)
