for i in range(16):
    print("{",end="")
    for j in range(18):
        if j==0:
            print(f"{-1},",end="")
        elif j == 17:
            print(f"{-1}",end="")
        else:
            if i%2==1:
                print(f"{i*16+j-1},",end="")
            else:
                print(f"{i*16+17-j-1},",end="")
    print("},")