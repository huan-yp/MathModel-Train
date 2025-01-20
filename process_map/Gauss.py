import cv2

# 读取图像
image = cv2.imread('Fire2.jpg')

# 应用高斯模糊
blurred_image = cv2.GaussianBlur(image, (15, 15), 0)

# 显示原始图像和模糊后的图像
cv2.imshow('Original Image', image)
cv2.imshow('Blurred Image', blurred_image)
cv2.waitKey(0)
cv2.destroyAllWindows()

# 保存模糊后的图像
success = cv2.imwrite(r'C:\Users\Lenovo\Desktop\MCM\workspace-1.18\Fire4.jpg', blurred_image) 
if not success:
    print("Failed to save image")