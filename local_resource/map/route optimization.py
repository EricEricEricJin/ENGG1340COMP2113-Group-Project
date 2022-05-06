# This is a Python algorithm used to compute route optimization
#for later translation into c++
#there will be values attached to each square in a map
#could be the gain of the game
#the goal is to find the route from starting point to end point
#covering the greatest possible values along the way
#starts by inputting two points as starting point and ending point
#and the optimised route will be shown



# from bottom left to top right
a = int(input("Please input the x coordinate of starting point"))
b = int(input("Please input the y coordinate of starting point"))
m = int(input("Please input the x coordinate of ending point"))
n = int(input("Please input the y coordinate of ending point"))

#example of a square matrix
array1 = [[15, 72, 45, 28, 59, 11, 23, 65, 77, 84],[60, 59, 33, 35, 57, 58, 93, 50, 73, 49],[10, 91, 34, 75, 40, 46, 30, 39, 68, 67],[15, 33, 56, 84, 29, 11, 20, 15, 65, 44],[55, 64, 78, 15, 45, 57, 66, 44, 98, 100],[14, 22, 35, 77, 88, 93, 55, 64, 88, 89],[15, 11, 23, 27, 16, 20, 30, 34, 45, 76],[91, 93, 65, 42, 83, 95, 55, 20, 24, 34],[45, 45, 97, 38, 52, 25, 80, 81, 89, 93],[26, 43, 17, 18, 93, 57, 39, 40, 47, 90] ]


def RouteOptimize(array1,a,b,m,n):
    array2 = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0] ]
    array3 = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0] ]
    array2[a][b] = array1[a][b]
    i = a
    while i > m:
        array2[i - 1][b] = array2[i][b] + array1[i - 1][b]
        i = i - 1

    j = b

    while j <= n - 1:
        array2[a][j + 1] = array2[a][j] + array1[a][j + 1]
        j = j + 1

    j = a - 1
    while j >= m:
        k = b + 1
        while k <= n:
            if array2[j][k - 1] > array2[j + 1][k]:
                array2[j][k] = array2[j][k - 1] + array1[j][k]
            else:
                array2[j][k] = array2[j + 1][k] + array1[j][k]
            k = k + 1
        j = j - 1
    for i in range(len(array2)):
        for j in range(len(array2[i])):
            print(array2[i][j], end=" ")
        print()
    j = m
    k = n
    array3[j][k] = 1

    while j < a and k > b:


        if array2[j][k - 1] > array2[j + 1][k]:
            array3[j][k - 1] = 1
            k = k - 1

        else:
            array3[j + 1][k] = 1
            j = j + 1

    if j == a:
        while k >= b:
            array3[j][k] = 1
            k = k - 1

    if k == b:
        while j <= a:
            array3[j][k] = 1
            j = j + 1

    for i in range(len(array3)):
        for j in range(len(array3[i])):
            print(array3[i][j], end=" ")
        print()
    return array3
def rotate90degrees(array):
    array4 = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0] ]
    for i in range(len(array)):
        for j in range(len(array[i])):
            array4[i][j] = array[len(array)-1-j][i]
            
    for i in range(len(array4)):
        for j in range(len(array4[i])):
            print(array4[i][j],end=" ")
        print()
    return array4
def rotate180degrees(array):
    array4 = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0] ]
    for i in range(len(array)):
        for j in range(len(array[i])):
            array4[i][j] = array[len(array)-1-i][len(array)-1-j]

    for i in range(len(array4)):
        for j in range(len(array4[i])):
            print(array4[i][j],end=" ")
        print()
    return array4

def rotate270degrees(array):
    array4 = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0, 0, 0, 0] ]
    for i in range(len(array)):
        for j in range(len(array[i])):
            array4[i][j] = array[j][len(array)-1-i]

    for i in range(len(array4)):
        for j in range(len(array4[i])):
            print(array4[i][j],end=" ")
        print()
    return array4



if a > m and b < n:
    print(RouteOptimize(array1,a,b, m,n))
    print()

elif a < m and b < n:
    A = len(array1)-a-1
    B = b
    M =len(array1)-m-1
    N = n
    rotate90degrees(RouteOptimize(rotate270degrees(array1),A,B,M,N))
    print()

elif a < m and b > n:
    A = len(array1) - a - 1
    B = len(array1) - b - 1
    M = len(array1) - m - 1
    N = len(array1) - n - 1
    rotate180degrees(RouteOptimize(rotate180degrees(array1),A,B,M,N))
    print()

else:
    A = b
    B = len(array1) -1 -a
    M = n
    N = len(array1) - 1 - m
    rotate270degrees(RouteOptimize(rotate90degrees(array1), A, B, M, N))
    print()