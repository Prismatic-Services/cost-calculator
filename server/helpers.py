import pandas as pd


def csv_to_excel(cvs_path, excel_path) -> None:
    df = pd.read_csv(cvs_path)
    df.to_excel(excel_path)
