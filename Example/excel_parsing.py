import pandas as pd
from pandas import DataFrame

# 파일명
file_name = "../Example/test.xlsx"

# Daraframe형식으로 엑셀 파일 읽기
df = pd.read_excel(file_name, sheet_name=["Sheet1", "Sheet2", "Sheet3"])

sheet1 = df["Sheet1"]
sheet1 = sheet1.fillna("")  # 공백 문자(NaN)를 실제 공백으로 변경

sheet2 = df["Sheet2"]
sheet2 = sheet2.fillna("")

sheet3 = df["Sheet3"]
sheet3 = sheet3.fillna("")

print("\n\n\n=====================================================================================================")
print("\t엑셀 파싱 시작 :", file_name)
print("=====================================================================================================")
print(sheet1)



convert_sheet1 = pd.DataFrame(sheet1)
print(convert_sheet1)
write_sheet1 = convert_sheet1.to_csv("../Example/sheet1.txt", sep="	", index = False)






print("\n\n=====================================================================================================")
print(sheet2)


print("\n\n=====================================================================================================")
print(sheet3)


print("=====================================================================================================")
print("\t엑셀 파싱 완료")
print("\n\n")


print("=====================================================================================================")
print("\t데이터 변경")
print("=====================================================================================================")
sheet1.loc[6] =["KKH1", "", "", "KKH4", "KKH5"]
print(sheet1)
print("\n\n")


# test_list = ['one', 'two', 'three']
# for i in sheet1.loc[0]:
#     # print("len :", len(i), ", ", i)
#     print(i)



with pd.ExcelWriter("../Example/test_convert.xlsx") as w:
    sheet1.to_excel(w, sheet_name='Sheet1', index=False)
    sheet2.to_excel(w, sheet_name='Sheet2', index=False)
    sheet3.to_excel(w, sheet_name='Sheet3', index=False)
