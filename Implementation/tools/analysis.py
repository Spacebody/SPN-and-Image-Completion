with open("evaluation_mse4.txt", "r") as f:
    lines = f.readlines()
    name = []
    left = []
    bottom = []
    for line in lines[1:]:
        name.append(line.strip().split(" ")[0])
        left.append(int(float(line.strip().split(" ")[1])))
        bottom.append(int(float(line.strip().split(" ")[2])))
    with open("ori_mse.txt") as f2:
        name2 = []
        lines2 = f2.readlines()
        left2 = []
        bottom2 = []
        for line2 in lines2[1:]:
            name2.append(line2.strip().split(" ")[0])
            left2.append(int(float(line2.strip().split(" ")[1])))
            bottom2.append(int(float(line2.strip().split(" ")[2])))
        print("Category,MSE on LEFT(Exp #4),MSE on LEFT(Poon)")
        for n, n2, my, poon in zip(name, name2, left, left2):
            if my <= poon:
                print(n + ",$" + str(my) + "$,$" + str(poon) + "$")
            if n == "yin_yang":
                print(n + ",$" + str(my) + "$,$" + str(poon) + "$")
        print("Category,MSE on BOTTOM(Exp #4),MSE on BOTTOM(Poon)")
        for n, n2, my, poon in zip(name, name2, bottom, bottom2):
            if my <= poon:
                print(n + ",$" + str(my) + "$,$" + str(poon) + "$")
            if n == "yin_yang":
                print(n + ",$" + str(my) + "$,$" + str(poon) + "$")
