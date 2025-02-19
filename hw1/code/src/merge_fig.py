import cv2
import numpy as np

# 读取四张图片
img1 = cv2.imread("../fig/grid_range_0.01.png")
img2 = cv2.imread("../fig/grid_range_0.05.png")
img3 = cv2.imread("../fig/grid_range_0.10.png")
img4 = cv2.imread("../fig/grid_range_0.20.png")

# 调整尺寸确保图片大小相同（可选）
size = (1000, 1000)
img1 = cv2.resize(img1, size)
img2 = cv2.resize(img2, size)
img3 = cv2.resize(img3, size)
img4 = cv2.resize(img4, size)

# 拼接成四宫格
top_row = np.hstack((img1, img2))
bottom_row = np.hstack((img3, img4))
grid_image = np.vstack((top_row, bottom_row))

# 显示拼接后的图片
cv2.imshow("Grid Image", grid_image)
cv2.waitKey(0)
cv2.destroyAllWindows()

# 保存图片
cv2.imwrite("../fig/grid_range_m.png", grid_image)

# 读取四张图片
img1 = cv2.imread("../fig/grid_knn_0.01.png")
img2 = cv2.imread("../fig/grid_knn_0.05.png")
img3 = cv2.imread("../fig/grid_knn_0.10.png")
img4 = cv2.imread("../fig/grid_knn_0.20.png")

# 调整尺寸确保图片大小相同（可选）
size = (1000, 1000)
img1 = cv2.resize(img1, size)
img2 = cv2.resize(img2, size)
img3 = cv2.resize(img3, size)
img4 = cv2.resize(img4, size)

# 拼接成四宫格
top_row = np.hstack((img1, img2))
bottom_row = np.hstack((img3, img4))
grid_image = np.vstack((top_row, bottom_row))

# 显示拼接后的图片
cv2.imshow("Grid Image", grid_image)
cv2.waitKey(0)
cv2.destroyAllWindows()

# 保存图片
cv2.imwrite("../fig/grid_knn_m.png", grid_image)
