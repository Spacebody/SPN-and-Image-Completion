import matplotlib.pyplot as plt

with open("evaluation_mse.txt", "r") as f:
    lines = f.readlines()
    left = []
    bottom = []
    for line in lines[1:]:
        left.append(int(float(line.strip().split(" ")[1])))
        bottom.append(int(float(line.strip().split(" ")[2])))
    with open("evaluation_mse3.txt") as f2:
        lines2 = f2.readlines()
        left2 = []
        bottom2 = []
        for line2 in lines2[1:]:
            left2.append(int(float(line2.strip().split(" ")[1])))
            bottom2.append(int(float(line2.strip().split(" ")[2])))
        print(min(min(left), min(left2)))
        print(max(max(left), max(left2)))
        print(min(min(bottom), min(bottom2)))
        print(max(max(bottom), max(bottom2)))
        
        f = plt.figure(figsize=(10, 10))
        ax1 = f.add_subplot(221)
        ax2 = f.add_subplot(222)
        ax3 = f.add_subplot(223)
        ax4 = f.add_subplot(224)

        ax1.set_xlabel('Exp#1 result in LEFT')
        ax1.set_ylabel('Exp#3 result in LEFT')
        ax1.scatter(left, left2, c="black", marker="+")

        ax2.scatter(left, left2, c="black", marker="+")
        ax2.plot([1000, 25000], [1000, 25000], ls="-", c="r")

        # with diag line
        ax3.set_xlabel('Exp#1 result in BOTTOM')
        ax3.set_ylabel('Exp#3 result in BOTTOM')
        ax3.scatter(bottom, bottom2, c="black", marker="+")

        ax4.scatter(bottom, bottom2, c="black", marker="+")
        ax4.plot([1000, 25000], [1000, 25000], ls="-", c="r")

        f.savefig("caltech_mse_exp1vs3.pdf")
        f.show()
