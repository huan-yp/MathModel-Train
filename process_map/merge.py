# 打开文件并读取所有内容
temp1 = []
temp2 = []
with open('output.txt', 'r') as file:
    for line in file:
        parts = line.split()  # 按空格分离
        temp1.extend(parts)

with open('output2.txt','r') as file:
    for line in file:
        parts = line.split()  # 按空格分离
        temp2.extend(parts)

content = []
for i in range(681):
    for  j in range(788):
        content.append(temp1[i*681+j])
        content.append(temp2[i*681+j])

with open('merge.txt', 'w') as file:
    # 写入内容到文件
    file.write("681 788")
    file.write("\n")
    for i in range(681): 
        for j in range(788):
           file.write(f"{content[(i*681+j)*2-1]}  ")
           file.write(f"{content[(i*681+j)*2]}  ")
        file.write("\n")