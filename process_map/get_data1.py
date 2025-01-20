from PIL import Image

def get_image_rgb_values(image_path):
    # 打开图像
    image = Image.open(image_path)
    # 将图像转换为RGB模式
    image = image.convert('RGB')
    width, height = image.size
    rgb_values = []
    
    print(width)
    print(height)
    # 遍历每个像素点
    for y in range(height):
        for x in range(width):
            # 获取像素点的RGB值
            r, g, b = image.getpixel((x, y))
            rgb_values.append((r, g, b))
    
    return rgb_values

# 示例：读取图像并获取所有像素点的RGB值
image_path = 'resized_image3.jpg'
rgb_values = get_image_rgb_values(image_path)
# 打开一个文件用于写入 (w 模式表示写入)
ans = []
for i in range(683):
    ans.append([0]*790)
for i in range(683):
    for j in range(790):
        if (rgb_values[i*683+j][0] > rgb_values[i*683+j][1] and rgb_values[i*683+j][0] > rgb_values[i*683+j][2]):
            ans[i][j] = 1 
        else: ans[i][j] = 0
data = []
for i in range(681):
        data.append([0]*788)
for i in range(1,682):
    for j in range(1,789):
        data[i-1][j-1] = ans[i][j]*0.4+(ans[i-1][j]+ans[i+1][j]+ans[i][j-1]+ans[i][j+1])*0.1+(ans[i-1][j-1]+ans[i-1][j+1]+ans[i+1][j-1]+ans[i+1][j+1])*0.05


with open('output2.txt', 'w') as file:
    # 写入内容到文件
    for i in range(681): 
        for j in range(788):
           file.write(f"{round(data[i][j],2)}      ")
        file.write("\n")