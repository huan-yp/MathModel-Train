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
image_path = r'C:\Users\Lenovo\Desktop\MCM\2021 B\Fire2.jpg'
rgb_values = get_image_rgb_values(image_path)
# 打开一个文件用于写入 (w 模式表示写入) 
# 打开一个文件用于写入 (w 模式表示写入)
with open('output.txt', 'w') as file:
    # 写入内容到文件
    for i in range(497): 
        for j in range(683):
           file.write(f"{rgb_values[i*683+j]}")
        file.write("\n")





