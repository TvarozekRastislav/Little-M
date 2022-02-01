from tkinter.ttk import Separator
import pandas as pd
import numpy as np
import json
import matplotlib.pyplot as plt
from sklearn.metrics import f1_score, accuracy_score, f1_score, precision_score, recall_score, classification_report, confusion_matrix, plot_confusion_matrix
from micromlgen import port
from sklearn import svm, datasets
from sklearn.model_selection import train_test_split

from datetime import datetime
def export_to_c(clf):
    LABELS = ['1','2','3'] #our existing labels
    classMap = {} #create an empty dict
    for i, label in zip(range(3),LABELS): #interate over the range and the labels at the same time 
        classMap[i]=label #fill our dict
    c_code = port(clf,classmap=classMap) #convert our model

    with open("model.h", "w") as f:
        f.write(c_code)
        f.close()

#Let's print the size of the .h file
import os
model_h_size = os.path.getsize("model.h")
def create_model(df, random_state, test_split, kernel, c):
    print(f"{datetime.now()} --- model training start")

    X = df[df.columns.difference(["room"])]
    y = df["room"]

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=test_split, random_state=random_state)
    clf = svm.SVC(kernel = kernel, C = c, gamma = 0.001)

    clf.fit(X_train, y_train)
    y_pred = clf.predict(X_test)

    plt.rcParams['figure.figsize'] = 10, 5

    print(f"{datetime.now()} --- model trained succesfully:")

    print(f1_score(y_test, y_pred, average='micro'))
    print(precision_score(y_test, y_pred, average="macro"))
    print(recall_score(y_test, y_pred, average="macro"))

    print(classification_report(y_test,y_pred))

    return clf

def df_clear(df):
    df.columns = ["room","1","n","2","n","3","n"]
    df.pop("n")
    df = df.dropna(axis = 0)
    df = df.astype({"3": int})

    return df

def json_load(file):
    print(f"{datetime.now()} --- opening file - {file} ")
    try:
        with open(file) as f:
            opened_file = json.load(f)
    except Exception as e:
        print(f"{datetime.now()} --- loading - {file} failed ")  
        print(f"{datetime.now()} --- error: {e} \n")    

    print(f"{datetime.now()} --- file opened succesfully")
    return opened_file


def main():
    print(f"{datetime.now()} --- loading config ")
    config = json_load("config.json")

    in_file = config["in_file"]
    out_file = config["out_file"]
    separator = config["separator"]

    random_state = config["random_state"]
    test_split = config["test_split"]
    kernel = config["kernel"]
    c = config["c"]
    print(f"{datetime.now()} --- loading {in_file} as dataframe ")
    df = pd.read_csv('data_in.txt', sep=separator, header=None,skiprows=2)

    print(f"{datetime.now()} --- clearing data \n")
    df = df_clear(df)

    print(f"{datetime.now()} --- num of values: \n{df.room.value_counts()} ")
    print(f"{datetime.now()} --- shape of df: {df.shape} ")
    
    print(f"{datetime.now()} --- Saving dataframe as {out_file}")
    df.to_csv(out_file)

    try:
        model = create_model(df, random_state, test_split, kernel, c)
    except Exception as e:
        print(f"{datetime.now()} --- vytváranie modelu zlyhalo")  
        print(f"{datetime.now()} --- error: {e} \n")    

    try: 
        export_to_c(model)
    except Exception as e: 
        print(f"{datetime.now()} --- exportovanie modelu zlyhalo")  
        print(f"{datetime.now()} --- error: {e} \n")
        exit()

    print(f"{datetime.now()} --- model bol úspešne exportovaný")  
if __name__ == "__main__":
    main()