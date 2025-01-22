import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

np.set_printoptions(precision=6, suppress=True)

L_SUPERIOR = "Lake Superior"
R_ST_MARYS = "St. Mary's River"
L_MICHIGAN = "Lake Michigan and Lake Huron"
R_ST_CLAIR = "St. Clair River"
L_ST_CLAIR = "Lake St. Clair"
R_DETROIT = "Detroit River"
L_ERIE = "Lake Erie"
R_NIAGARA = "Niagara River"
L_ONTARIO = "Lake Ontario"
R_OTTAWA = "Ottawa River"
R_ST_LAWRENCE = "St. Lawrence River"

LAKES = [L_SUPERIOR, L_MICHIGAN, L_ST_CLAIR, L_ERIE, L_ONTARIO]
RIVERS = [R_ST_MARYS, R_ST_CLAIR, R_DETROIT, R_OTTAWA, R_ST_LAWRENCE]

linkage = [
    (L_SUPERIOR, R_ST_MARYS),
    (R_ST_MARYS, L_MICHIGAN),
    (L_MICHIGAN, R_ST_CLAIR),
    (R_ST_CLAIR, L_ST_CLAIR),
    (L_ST_CLAIR, R_DETROIT),
    (R_DETROIT, L_ERIE),
    (L_ERIE, R_NIAGARA),
    (R_NIAGARA, L_ONTARIO),
    (R_OTTAWA, L_ONTARIO),
    (R_ST_LAWRENCE, L_ONTARIO),
]

def solve_least_squares(A, b):
    x = np.linalg.lstsq(A, b, rcond=None)[0]
    return x

def is_demical(x:str):
    if str(x) == "nan":
        return False
    try:
        float(x)
        return True
    except ValueError:
        return False

def find_next_month(year, month):
    if month == 12:
        return year + 1, 1
    else:
        return year, month + 1

def find_data(year, month, water_area):
    if year not in data[water_area]:
        return None
    if month not in data[water_area][year]:
        return None
    return data[water_area][year][month]
    
def average_ignore_none(lst):
    # 使用 filter 和 lambda 函数过滤掉 None 值
    filtered_lst = list(filter(lambda x: x is not None, lst))
    if len(filtered_lst) == 0:
        return None
    return sum(filtered_lst) / len(filtered_lst)

# 该表格没有表头
excel = pd.ExcelFile('data.xlsx')
data_frames = {sheet_name: excel.parse(sheet_name) for sheet_name in excel.sheet_names}
data = {}

for sheet_name in excel.sheet_names:
    this_sheet = {}
    for index, row in data_frames[sheet_name].iterrows():
        if not is_demical(row[0]):
            continue
        year_data = {}
        have_record = True
        for i, col in enumerate(row[1:], start=1):
            if not is_demical(col):
                have_record = False
            year_data[i] = col
        if have_record:
            this_sheet[int(row[0])] = year_data
    data[sheet_name] = this_sheet
    
river_args = {}

def calc_river_args():
    for river in RIVERS:
        source, target = None, None
        for link in linkage:
            if river == link[0]:
                target = link[1]
            if river == link[1]:
                source = link[0]

        river_args[river] = {}
        
        print(river)
        for month in range(1, 13):
            if source is None or target is None:
                river_args[river][month] = [
                    average_ignore_none([find_data(year, month, river) for year in range(2000, 2022)]), 0
                ]
                continue
                
            available_years = [
                year for year in range(2000, 2022) if 
                    find_data(year, month, source) is not None and
                    find_data(year, month, target) is not None and
                    find_data(year, month, river) is not None
            ]
            source_data = [find_data(year, month, source) for year in available_years]
            target_data = [find_data(year, month, target) for year in available_years]
            flows = [find_data(year, month, river) for year in available_years]
            
            level_diff_sqrt = [(source - target) ** 0.5 for source, target, flow in zip(source_data, target_data, flows)]
            
            one_vector = np.ones((len(level_diff_sqrt), 1))
            diff_vector = np.array(level_diff_sqrt).reshape(-1, 1)
            res_vector = np.array(flows).reshape(-1, 1)
            
            A = np.hstack((one_vector, diff_vector))
                    
            x = solve_least_squares(A, res_vector)
            level_diff = [source - target for source, target in zip(source_data, target_data)]   
            river_args[river][month] = [x[0][0], x[1][0]] 
                
        print(river_args[river])        

def calc_lake_args():
    for lake in LAKES:
        print(lake)
        sources, targets = [], []
        for link in linkage:
            if lake == link[0]:
                targets += [link[1]]
            if lake == link[1]:
                sources += [link[0]]
        for month in range(1, 13):
            available_years = [year for year in range(2000, 2022) if all(
                [find_data(year, month, name) is not None for name in targets + sources + [lake]]
            )]
            
            sources_data = [[find_data(year, month, source) for year in available_years] for source in sources]
            targets_data = [[find_data(year, month, target) for year in available_years] for target in targets]
            water_levels = [find_data(year, month, lake) for year in available_years]
                        
            source_vector = np.array(sources_data).reshape(len(available_years), len(sources))
            target_vector = np.array(targets_data).reshape(len(available_years), len(targets))
            res_vector = np.array(water_levels).reshape(len(available_years), 1)
            one_vector = np.ones((len(available_years), 1))
            
            A = np.hstack((source_vector, target_vector, one_vector))
            x = solve_least_squares(A, res_vector)
            print(x)
            plt.scatter(target_vector.reshape(-1), res_vector.reshape(-1))
            plt.plot(target_vector.reshape(-1), x[0][0] * target_vector + x[1][0])
            plt.show()
            
        exit(0)

calc_lake_args()