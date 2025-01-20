import cv2

# 读取两张图像
image = cv2.imread('Fire4.jpg')

# 获取第二张图像的尺寸
height = 683
width = 790

# 调整第一张图像的尺寸与第二张图像一致
resized_image = cv2.resize(image, (width, height))

# 保存调整后的图像
cv2.imwrite('resized_image3.jpg', resized_image)
