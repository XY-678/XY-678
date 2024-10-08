import pandas as pd

# 生成数据
data = [
    "0001 2323 0202 3 0202 12 14.400000",
    "0002 23 0202 6 0203 12 48.000000",
    "0003 0303 0202 12 0204 1 59.200000",
    "0003 02 0202 1 0202 4 4.800000",
    "$001 1 0202 1 0206 10 189.000000",
    "0004 023 0202 3 0202 21 28.800000",
    "$003 111 0203 3 0203 21 32.400000",
    "0005 123 0303 12 0303 21 14.400000",
    "0006 223 0303 6 0303 15 19.200000",
    "0007 2323 0202 3 0202 12 14.400000",
    "$002 33 0202 6 0203 12 48.000000",
    "0008 0303 0202 12 0204 1 59.200000",
    "0009 03 0202 1 0202 4 4.800000",
    "$004 2 0202 1 0206 10 189.000000",
    "0010 023 0202 3 0202 21 28.800000",
    "$005 11 0203 3 0203 21 32.400000",
    "0011 123 0303 12 0303 21 14.400000",
    "$006 2323 0202 3 0202 12 14.400000",
    "0012 33 0202 6 0203 12 48.000000",
    "$007 0303 0202 12 0204 1 59.200000",
    "0013 03 0202 1 0202 4 4.800000",
    "$008 2 0202 1 0206 10 189.000000",
    "0014 023 0202 3 0202 21 28.800000",
    "$009 11 0203 3 0203 21 32.400000",
    "0015 123 0303 12 0303 21 14.400000",
    "$010 223 0303 6 0303 15 19.200000",
    "0016 2323 0202 3 0202 12 14.400000",
    "$011 33 0202 6 0203 12 48.000000",
    "0017 0303 0202 12 0204 1 59.200000",
    "0018 03 0202 1 0202 4 4.800000",
    "$012 2 0202 1 0206 10 189.000000",
    "0019 023 0202 3 0202 21 28.800000",
    "$013 11 0203 3 0203 21 32.400000",
    "0020 123 0303 12 0303 21 14.400000"
]

# 将数据整理为DataFrame
records = []
for line in data:
    parts = line.split()
    records.append({
        'Card Number': parts[0],
        'Location Code': parts[1],
        'Start Date': parts[2],
        'Start Time': parts[3],
        'End Date': parts[4],
        'End Time': parts[5],
        'Total Cost': parts[6]
    })

df = pd.DataFrame(records)

# 导出为Excel文件
excel_file = 'records.xlsx'
df.to_excel(excel_file, index=False)

print(f"数据已成功导出到 {excel_file}")