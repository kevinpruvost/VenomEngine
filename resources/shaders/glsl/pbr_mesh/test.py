import numpy as np
import math
import numpy.linalg as la

n1 = np.array([0, -1, 0])
n2 = np.array([-2, -1, 0])
p1 = np.array([0, 1, 0])
p2 = np.array([1, 1, 0])
n1 = n1 / np.linalg.norm(n1)
n2 = n2 / np.linalg.norm(n2)
angle = 45 / 180 * math.pi
range = 1
radius = range / math.sin(angle)

# Parallel case
# distance = np.dot(p2 - p1, n1)
# distance = np.linalg.norm(distance)
# print(distance)

# Perpendicular case
# Perpendicular Planes intersections make a line, we need the signed distance of p1 from
# distance = np.dot(n1, p2 - p1)
# signedDistance = distance
# print(signedDistance)

# Other case
# distance = np.dot(p2 - p1, n1)
# signedDistance = distance
# print(signedDistance)

# Last case if intersection happens out of the apex and range of the cone
w = np.cross(n1, np.cross(n2, n1))
w = w / np.linalg.norm(w)

def dOfPlane(w, p):
    return -np.dot(w, p)

def distanceFromPlane(normalPlane, pointFromNormalPlame, directionPoint, point):
    d = dOfPlane(normalPlane, pointFromNormalPlame)
    return -(np.dot(normalPlane, point) + d) / (np.dot(directionPoint, normalPlane))

# d is the minimum distance from the plane to the origin 0 0 0
# dw = -np.dot(w, p1)
# t = -(np.dot(w, p2) + dw) / (np.dot(n2, w))
t = distanceFromPlane(w, p1, n2, p2)
Ia = p2 + t * n2
B = p1 + range * n1
a = np.dot(B - Ia, n1)

theta = math.acos(np.dot(n2, w)) * 180 / math.pi
c = a / math.sin(theta * math.pi / 180)

# get b by using pythagorean theorem with c and a
b = math.sqrt(c * c - a * a)

print("n1:", n1)
print("n2:", n2)
print("t:", t)
print("radius:", radius)
print("Ia:", Ia)
print("B:", B)
print("a:", a)
# print("w:", w)
# print("theta:", theta)
# print("c:", c)
# print("b:", b)

# # result = b <= radius
# # print(result)

# # Last case
# Ic = B + b * w
# print("Ic:", Ic)
# # Right !

# IaToIc = Ic - Ia
# IaToIc = IaToIc / np.linalg.norm(IaToIc)

# tD = distanceFromPlane(n1, p1, n2, p2)

# Id = p2 + tD * IaToIc
# print("tD:", tD)
# print("Id:", Id)

# signedDistanceId = np.dot(Id - p1, w)
# print("signedDistanceId:", signedDistanceId)
# signedDistanceIc = np.dot(Ic - p1, w)
# print("signedDistanceIc:", signedDistanceIc)
# print("Product is:", signedDistanceId * signedDistanceIc)
# print("Intersection:", signedDistanceId * signedDistanceIc < 0)