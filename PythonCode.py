import re
import string

def strip(str):
    return str.strip("\r\n")

def groceryListCount():
    list = []
    groceryList = dict({})
    with open("groceries.txt") as file:
        list = file.readlines()
        list = map(strip, list)

    for item in list:
        if item in groceryList.keys():
            groceryList[item] += 1;
        else:
            groceryList[item] = 1;
    return groceryList

def printGroceryListCount():
    groceryList = groceryListCount()
    for item in groceryList:
        frequency = groceryList[item]
        print("{item} {frequency}".format(item=item, frequency=frequency))

def createDataFile():
    groceryList = groceryListCount()
    print("Creating data file...")
    file = open("frequency.dat", "w")

    for item in groceryList:
        frequency = groceryList[item]
        file.write("{item} {frequency}\n".format(item=item, frequency=frequency))
    
    print("Data file creation completed!\n\n")
    file.close()
    
def purchaseCount(item):
    groceryList = groceryListCount()
    return groceryList[item]

