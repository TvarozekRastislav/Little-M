from tkinter.ttk import Separator
import pandas as pd
import numpy as np
import json
from datetime import datetime

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

    print(f"{datetime.now()} --- loading {in_file} as dataframe ")
    df = pd.read_csv(in_file, sep = separator, header = None)

    print(f"{datetime.now()} --- clearing data \n")
    df = df_clear(df)

    print(f"{datetime.now()} --- num of values: \n{df.room.value_counts()} ")
    print(f"{datetime.now()} --- shape of df: {df.shape} ")
    
    print(f"{datetime.now()} --- Saving dataframe as {out_file}")
    df.to_csv(out_file)

if __name__ == "__main__":
    main()