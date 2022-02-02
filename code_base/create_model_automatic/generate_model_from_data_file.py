from tkinter.ttk import Separator
import pandas as pd
import numpy as np
import json
import matplotlib.pyplot as plt
import logging
from sklearn.metrics import f1_score, accuracy_score, f1_score, precision_score, recall_score, classification_report, confusion_matrix, plot_confusion_matrix
from micromlgen import port
from sklearn import svm, datasets
from sklearn.model_selection import train_test_split
from datetime import datetime

level = logging.DEBUG
fmt = '[%(levelname)s %(asctime)s - %(message)s]'
logging.basicConfig(level=level, format=fmt)

def export_to_c(clf):
    LABELS = ['1','2','3'] 
    classMap = {} 
    for i, label in zip(range(3),LABELS): 
        classMap[i]=label 
    c_code = port(clf,classmap=classMap)

    with open("model.h", "w") as f:
        f.write(c_code)
        f.close()

import os
def create_model(df, random_state, test_split, kernel, c):
    logging.info("model training start")


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
    df = df.drop([2,4,6], axis = 1)
    df.columns = ["room","1","2","3"]
    df = df.dropna(axis = 0)
    df = df.astype({"3": int, "2" : int , "1": int})

    return df

def json_load(file):
    logging.info("opening file")

    try:
        with open(file) as f:
            opened_file = json.load(f)
    except Exception as e:
        logging.error("loading - %s failed ", file)
        logging.error("loading - error: %s", e)

    logging.info("file opened succesfully")
    return opened_file


def main():
    logging.info("loading config")
    config = json_load("config.json")

    in_file = config["in_file"]
    out_file = config["out_file"]
    separator = config["separator"]

    random_state = config["random_state"]
    test_split = config["test_split"]
    kernel = config["kernel"]
    c = config["c"]

    logging.info("Config je nastavený:\nin_file: %s\noutfile: %s\nseparator: %s\nrandom_state: %s\ntest_split: %s\nkernel: %s\nc: %s",
     in_file, out_file ,separator, random_state, test_split, kernel, c)


    logging.info("loading %s as dataframe", in_file)
    df = pd.read_csv('data_in.txt', sep = separator, header = None)

    logging.info("loading %s clearing data", in_file)
    df = df_clear(df)

    logging.info("num of values: \n%s", df.room.value_counts())
    logging.info("shape of df: %s ", df.shape)
    
    logging.info("Saving dataframe as %s", out_file)
    df.to_csv(out_file)

    try:
        model = create_model(df, random_state, test_split, kernel, c)
    except Exception as e:
        logging.error("vytváranie modelu zlyhalo")
        logging.error("error: %s", e)

    try: 
        export_to_c(model)
        with open("model.h", "r") as f:
            list_of_lines = f.readlines()
            list_of_lines[1] = "#include <stdarg.h>\n"
    
        with open("model.h", "w") as f:
            f.writelines(list_of_lines)
    
    except Exception as e: 
        logging.error("exportovanie modelu zlyhalo")
        logging.error("error: %s", e)
        exit()
    logging.info("model bol úspešne exportovaný")

if __name__ == "__main__":
    main()