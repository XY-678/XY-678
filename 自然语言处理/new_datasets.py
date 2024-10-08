import pandas as pd

# 示例数据（已调整为分隔单元格格式）
data = [
    ["一号", "男", "0101", "1010101", "0001", "1000", "0101", 985.6],
    ["二号", "男", "0101", "200021", "0002", "321", "0123", 1952.0],
    ["三号", "女", "0303", "1232931", "0003", "0203", "0303", 2936.0],
    ["四号", "男", "0202", "932123", "0004", "9999", "0101", 3971.2],
    ["五号", "女", "00909", "238123", "0005", "2039", "0292", 4985.6],
    ["六号", "女", "00303", "99231234", "0006", "29999", "0404", 6000.0],
    ["七号", "男", "0482", "92312", "0007", "19923", "0292", 7000.0],
    ["八号", "女", "0404", "882312", "0008", "8822", "0202", 800.0],
    ["九号", "女", "0606", "9232432", "0009", "2923", "2929", 900.0],
    ["十号", "女", "0505", "88232", "0010", "88232", "0202", 1000.0],
    ["十一号", "男", "8823", "23232421", "0011", "231", "9292", 1100.0],
    ["十二号", "女", "0707", "234234", "0012", "2392", "3232", 1200.0],
    ["十三号", "男", "0909", "992342", "0013", "0909", "9999", 1300.0],
    ["十四号", "男", "1111", "1111111", "0014", "1111", "1111", 1400.0],
    ["十五号", "女", "2222", "2222222", "0015", "2222", "2222", 1500.0],
    ["十六号", "男", "3333", "3333333", "0016", "3333", "3333", 1600.0],
    ["十七号", "女", "4444", "4444444", "0017", "4444", "4444", 1700.0],
    ["十八号", "男", "5555", "5555555", "0018", "5555", "5555", 1800.0],
    ["十九号", "女", "6666", "6666666", "0019", "6666", "6666", 1900.0],
    ["二十号", "男", "7777", "7777777", "0020", "7777", "7777", 2000.0],
    ["二十一号", "女", "8888", "8888888", "0021", "8888", "8888", 2100.0],
    ["二十二号", "男", "9999", "9999999", "0022", "9999", "9999", 2200.0],
    ["二十三号", "女", "1010", "1010101", "0023", "1010", "1010", 2300.0],
    ["二十四号", "男", "1111", "1111111", "0024", "1111", "1111", 2400.0],
    ["二十五号", "女", "1212", "1212121", "0025", "1212", "1212", 2500.0],
    ["二十六号", "男", "1313", "1313131", "0026", "1313", "1313", 2600.0],
    ["二十七号", "女", "1414", "1414141", "0027", "1414", "1414", 2700.0],
    ["二十八号", "男", "1515", "1515151", "0028", "1515", "1515", 2800.0],
    ["二十九号", "女", "1616", "1616161", "0029", "1616", "1616", 2900.0],
    ["三十号", "男", "1717", "1717171", "0030", "1717", "1717", 3000.0],
    ["三十一号", "女", "1818", "1818181", "0031", "1818", "1818", 3100.0],
    ["三十二号", "男", "1919", "1919191", "0032", "1919", "1919", 3200.0],
    ["三十三号", "女", "2020", "2020202", "0033", "2020", "2020", 3300.0]
]

# 创建DataFrame
df = pd.DataFrame(data, columns=["号码", "性别", "字段1_1", "字段1_2", "字段2_1", "字段2_2", "字段2_3", "金额"])

# 保存为Excel文件
excel_file = "data_with_spaces.xlsx"
df.to_excel(excel_file, index=False)

print(f"数据已保存到 {excel_file}")
