int convert(int gpio) {
    if (gpio == 2)
        return 8;

    if (gpio == 3)
        return 9;

    if (gpio == 4)
        return 7;
    
    if (gpio == 17)
        return 0;

    if (gpio == 27)
        return 2;
    
    if (gpio == 22)
        return 3;

    if (gpio == 10)
        return 12;

    if (gpio == 9)
        return 13;

    if (gpio == 11)
        return 14;

    if (gpio == 0)
        return 30;

    if (gpio == 5)
        return 21;

    if (gpio == 6)
        return 22;

    if (gpio == 13)
        return 23;

    if (gpio == 19)
        return 24;

    if (gpio == 26)
        return 25;

    if (gpio == 14)
        return 15;

    if (gpio == 15)
        return 16;

    if (gpio == 18)
        return 1;

    if (gpio == 23)
        return 4;

    if (gpio == 24)
        return 5;

    if (gpio == 25)
        return 6;

    if (gpio == 8)
        return 10;

    if (gpio == 7)
        return 11;
    
    if (gpio == 1)
        return 31;
    
    if (gpio == 12)
        return 26;
        
    if (gpio == 16)
        return 27;
            
    if (gpio == 20)
        return 28;
    
    if (gpio == 21)
        return 29;

    else
        return -1;
}